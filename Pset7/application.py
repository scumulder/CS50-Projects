from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp


from passlib.context import CryptContext

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd                      #a function to format values as US dollars

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

###################################################################################################################### INDEX #######
@app.route("/")
@login_required
def index():

    stocks = []
    t = 0

    un = db.execute("SELECT username FROM users WHERE id = :id", id = session["user_id"])
    un = un[0]["username"]

    hold = db.execute("""SELECT stock_name, symbol, Sum(quantity) AS quantity FROM transactions WHERE username=:uname
                        AND t_type = 'BUY' GROUP BY stock_name, symbol ORDER BY stock_name""", uname = un)

    # subtract the quantity of shares SOLD
    for i in hold:
        hold2 = db.execute("""SELECT Sum(quantity) AS quantity FROM transactions WHERE username=:uname
                            AND t_type = 'SELL' AND symbol = :sym""", uname = un, sym = i["symbol"])
        # hold2 = {quan2}

        if hold2[0]["quantity"] != None:
            q1 = i["quantity"]
            q2 =  hold2[0]["quantity"]
            q = abs(q1 - int(q2))

            i["quantity"] = q

    cash = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])
    cash = round(float(cash[0]["cash"]), 2)

    # get current price
    for dict in hold:

        name = str(dict["stock_name"])
        symbol = str(dict["symbol"])
        quantity = int(dict["quantity"])

        # change quantity to reflect difference in buy and sell

        cur_price = lookup(symbol)
        cur_price = cur_price['price']

        total = quantity * cur_price
        t += total

        stockdict = {"name": name, "symbol": symbol, "quantity": quantity, "current": usd(cur_price), "total": usd(total)}

        stocks.append(stockdict)

    grand = cash + t
    return render_template("index.html", stocks = stocks, cash = cash, grand = usd(grand))

######################################################################################################################## BUY #######
@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == "POST":

        stock = lookup(request.form.get("symbol"))
        quant = int(request.form.get("quantity"))

        if stock == None:
            return apology("Invalid symbol")

        nam = stock['name']
        price = stock['price']
        sym = stock['symbol']

        # get current cash balance
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])
        cash = cash[0]["cash"]

        # check to see if there's enough cash to make the purchase
        total_price = quant * price
        if total_price > float(cash):
            return apology("You're too broke to make this purchase")

        un = db.execute("SELECT username FROM users WHERE id = :id", id = session["user_id"])

        db.execute("""INSERT INTO transactions (username, stock_name, symbol, price, quantity, t_type)
                        VALUES(:username, :stockname, :symbol,:price, :quantity, 'BUY')""",
                        username=un[0]["username"], stockname = nam, symbol = sym,
                        price = usd(price), quantity= quant)

        # update cash balance
        db.execute("UPDATE users SET cash = :balance WHERE id = :id", balance = float(cash) - total_price, id = session["user_id"])

        return redirect(url_for("index"))

    else:
        return render_template("buy.html")

#################################################################################################################### HISTORY #######
@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    un = db.execute("SELECT username FROM users WHERE id = :id", id = session["user_id"])
    un = un[0]["username"]

    # pull the relevant data from transactions table
    data = db.execute("SELECT stock_name, symbol, price, quantity, t_type, timestamp FROM transactions WHERE username=:un", un = un)

    # add it to a list called stocks
    stocks = []
    for dict in data:

        name = str(dict["stock_name"])
        symbol = str(dict["symbol"])
        quantity = int(dict["quantity"])
        price = dict["price"]
        t_type = str(dict["t_type"])
        dt = dict["timestamp"]

        stockdict = {"name": name, "symbol": symbol, "quantity": quantity, "price": price, "type": t_type, "dt": dt}
        stocks.append(stockdict)

    return render_template("history.html", stocks = stocks)
###################################################################################################################### LOGIN #######
@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""
    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                            username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

##################################################################################################################### LOGOUT #######
@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

###################################################################################################################### QUOTE #######
@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":
        stock = lookup(request.form.get("symbol"))
        if stock == None:
            return apology("Invalid symbol")
        nam = stock['name']
        pri = stock['price']
        sym = stock['symbol']
        return render_template("quoted.html", name = nam, price = usd(pri), symbol = sym)

    else:
        return render_template("quote.html")
################################################################################################################### REGISTER #######
@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""

    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # ensure password was typed again
        elif not request.form.get("confirm password"):
            return apology("must confirm password")

        # do the passwords match?
        if request.form.get("password") != request.form.get("confirm password"):
            return apology("passwords do not match!")

        else:
            #if all the information is entered correctly

            # hash the password (using version 1.6 of Passlib)
            hash = pwd_context.encrypt(request.form.get("password"))
            # Version 1.7 method: hash = myctx.hash(request.form.get("password"))

            # query database to create field
            result = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)",
                                username=request.form.get("username"), hash=hash)

            # check for failure in username creation
            if not result:
                return apology("username already exists!")

            # # store the user id
            # session["user_id"] = result

            # redirect user to home page
            return redirect(url_for("login"))

    else:
        return render_template("register.html")

####################################################################################################################### SELL #######
@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    if request.method == "POST":

        # lookup current stock price and get the quantity user entered
        stock = lookup(request.form.get("symbol"))
        quant = int(request.form.get("quantity"))

        st = db.execute("SELECT * FROM transactions WHERE symbol=:sym", sym = request.form.get("symbol"))

        # ensure stock exists in portfolio
        if len(st) != 1:
            return apology("No such stocks found")

        # ensure symbol is valid
        if stock == None:
            return apology("Invalid symbol")

        # get necessary information regarding stock
        nam = stock['name']
        price = stock['price']
        sym = stock['symbol']

        # get user's name
        un = db.execute("SELECT username FROM users WHERE id = :id", id = session["user_id"])
        un = un[0]["username"]

        # get current number of stock owned
        own = db.execute("SELECT SUM(quantity) as quantity FROM transactions WHERE username=:un AND symbol = :sym",
                            un =un, sym = sym)
        own = own[0]["quantity"]

        # get current cash balance
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])
        cash = cash[0]["cash"]

        # check to see if there's enough stocks to sell
        if own < quant:
            return apology("You don't have enough stocks to sell")

        # make sale if there're enough stocks
        db.execute("""INSERT INTO transactions (username, stock_name, symbol, price, quantity, t_type)
                        VALUES(:username, :stockname, :symbol, :price, :quantity, 'SELL')""",
                        username=un, stockname = nam, symbol = sym, price = price, quantity= quant)

        # calculate the total of the sale
        total_price = quant * price

        # update cash balance
        db.execute("UPDATE users SET cash = :balance WHERE id = :id", balance = float(cash) + total_price, id = session["user_id"])

        return redirect(url_for("index"))

    else:
        return render_template("sell.html")

#################################################################################################################### BALANCE #######
@app.route("/balance", methods=["GET", "POST"])
@login_required
def balance():

    if request.method == "POST":
        # get current cash balance
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])
        cash = cash[0]["cash"]

        # get amount
        amount = float(request.form.get("amount"))

        # add it to current balance
        cash = float(amount) + float(cash)

        # update cash balance
        db.execute("UPDATE users SET cash = :balance WHERE id = :id", balance = cash, id = session["user_id"])

        return render_template("balance.html", cb = cash)

    else:
        return render_template("balance.html")
