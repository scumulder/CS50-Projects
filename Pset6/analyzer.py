import nltk

#nltk.download()
from nltk.tokenize import TweetTokenizer

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer. Load positive and negative words into memory so that analyze can access them"""
        # TODO
        self.positives = set()
        self.negatives = set()
        file = open("positive-words.txt", "r")
        for line in file:
            if not  line.startswith(";"):
                self.positives.add(line.rstrip("\n"))
        file.close()
        file = open("negative-words.txt", "r")
        for line in file:
            if not  line.startswith(";"):
                self.negatives.add(line.rstrip("\n"))
        file.close()

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""

        score = 0
        #tokens = nltk.word_tokenize(text)
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)
        for token in tokens:
            if token.lower() in self.positives:
                score+=1
            elif token.lower() in self.negatives:
                score-=1
        return score
