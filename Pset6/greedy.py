import cs50

while True:
   print("O hai! How much change is owed?")
   change = cs50.get_float()
   change2 = round(100*change)
   counter = 0
   if change<0.0:
       break

   while change2 >= 25:
       change2 = change2 - 25
       counter+=1

   while change2 >= 10:
       change2 = change2 - 10
       counter+=1

   while change2 >= 5:
       change2 = change2 - 5
       counter+=1

   while change2 >= 1:
       change2 = change2 - 1
       counter+=1

   print("{}".format(counter))

