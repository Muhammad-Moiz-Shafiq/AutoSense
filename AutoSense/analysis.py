import pandas as pd
import nltk
import string
from nltk.corpus import stopwords
from nltk.stem import PorterStemmer
from collections import defaultdict
import math
import json
import os

class SentimentAnalyzer:
    def __init__(self, csv_file_path):
        self.original_df = pd.read_csv(csv_file_path)
        self.df = self.original_df.copy()
        
        # Check if 'selected_text' column exists before dropping
        if 'selected_text' in self.df.columns:
            self.df = self.df.drop(columns=['selected_text'])
        
        self.df["text"] = self.df["text"].astype(str)
        
        # Load or initialize feedback dictionary
        self.feedback_file = 'feedback_memory.json'
        self.feedback_memory = self.load_feedback_memory()
        
        self.negation_words = {'not', 'no', 'never', "n't", 'cannot', 'cant', "won't", 'wont'}
        self.initialize_model()
    
    def load_feedback_memory(self):
        if os.path.exists(self.feedback_file):
            with open(self.feedback_file, 'r') as f:
                return json.load(f)
        return {}
    
    def save_feedback_memory(self):
        with open(self.feedback_file, 'w') as f:
            json.dump(self.feedback_memory, f)

    def preprocess_tweet(self, tweet):
        """Preprocess tweet with negation handling"""
        tweet = tweet.lower().strip()
        tweet = tweet.translate(str.maketrans("", "", string.punctuation))
        
        # Check if this exact text has feedback
        if tweet in self.feedback_memory:
            return ['EXACT_MATCH']
        
        tokens = tweet.split()
        
        # Remove stopwords except negation words
        stopwords_set = set(stopwords.words("english")) - self.negation_words
        tokens = [token for token in tokens if token not in stopwords_set]
        
        # Handle negations
        processed_tokens = []
        negate = False
        
        for token in tokens:
            if token in self.negation_words:
                negate = True
                continue
            
            if negate:
                processed_tokens.append(f"NOT_{token}")
                negate = False
            else:
                processed_tokens.append(token)
        
        # Stem the tokens
        stemmer = PorterStemmer()
        processed_tokens = [stemmer.stem(token) for token in processed_tokens]
        
        return processed_tokens

    def initialize_model(self):
        self.sentiment_data = {
            'positive': defaultdict(int),
            'negative': defaultdict(int),
            'neutral': defaultdict(int)
        }
        
        # Process the original dataset
        for _, row in self.df.iterrows():
            tokens = self.preprocess_tweet(row['text'])
            sentiment = row['sentiment']
            for token in tokens:
                self.sentiment_data[sentiment][token] += 1
        
        # Calculate totals
        self.totals = {
            sentiment: sum(counts.values())
            for sentiment, counts in self.sentiment_data.items()
        }

    def classify_text(self, text):
        # Normalize the input text
        text = text.lower().strip()
        
        # Check feedback memory first
        if text in self.feedback_memory:
            remembered_sentiment = self.feedback_memory[text]
            # Create artificial scores to strongly favor the remembered sentiment
            scores = {
                'positive': -100.0,
                'negative': -100.0,
                'neutral': -100.0
            }
            scores[remembered_sentiment] = 0.0
            return remembered_sentiment, scores
        
        # If no exact match, proceed with normal classification
        tokens = self.preprocess_tweet(text)
        scores = {
            'positive': math.log(len(self.df[self.df['sentiment'] == 'positive']) / len(self.df)),
            'negative': math.log(len(self.df[self.df['sentiment'] == 'negative']) / len(self.df)),
            'neutral': math.log(len(self.df[self.df['sentiment'] == 'neutral']) / len(self.df))
        }
        
        for sentiment in ['positive', 'negative', 'neutral']:
            for token in tokens:
                count = self.sentiment_data[sentiment][token]
                # Add smoothing
                probability = (count + 0.1) / (self.totals[sentiment] + 0.1 * len(self.sentiment_data[sentiment]))
                scores[sentiment] += math.log(probability)
        
        predicted_sentiment = max(scores, key=scores.get)
        return predicted_sentiment, scores

    def add_feedback(self, text, correct_sentiment):
        # Normalize the text
        text = text.lower().strip()
        
        # Store the exact text and sentiment in feedback memory
        self.feedback_memory[text] = correct_sentiment
        
        # Save the updated feedback memory
        self.save_feedback_memory()
        
        # Add to training data
        new_row = {
            'textID': f'feedback_{len(self.df)}',
            'text': text,
            'sentiment': correct_sentiment,
        }
        self.df = pd.concat([self.df, pd.DataFrame([new_row])], ignore_index=True)
        
        # Update the model
        tokens = self.preprocess_tweet(text)
        for token in tokens:
            self.sentiment_data[correct_sentiment][token] += 5  # Give extra weight to feedback
        self.totals[correct_sentiment] = sum(self.sentiment_data[correct_sentiment].values())
        
        return len(self.df)

def main():
    analyzer = SentimentAnalyzer('./data/train.csv')
    print("\n=== Sentiment Analysis with Naïve Bayes ===\n")
    
    while True:
        user_input = input("\nEnter text to analyze (or 'quit' to exit): ")
        
        if user_input.lower() == 'quit':
            print("\nGoodbye!")
            break
            
        predicted_sentiment, sentiment_scores = analyzer.classify_text(user_input)
        
        print(f"\nPredicted Sentiment: {predicted_sentiment.upper()}")
        print("\nSentiment Scores:")
        for sentiment, score in sentiment_scores.items():
            print(f"{sentiment}: {score:.4f}")
            
        is_correct = input("\nWas this analysis correct? (y/n): ").lower()
        while is_correct not in ['y', 'n']:
            print("Please enter 'y' for yes or 'n' for no.")
            is_correct = input("Was this analysis correct? (y/n): ").lower()
        
        if is_correct == 'n':
            correct_sentiment = input("\nWhat is the correct sentiment? (positive/negative/neutral): ").lower()
            while correct_sentiment not in ['positive', 'negative', 'neutral']:
                print("Invalid sentiment. Please choose positive, negative, or neutral.")
                correct_sentiment = input("What is the correct sentiment? ").lower()
                
            total_rows = analyzer.add_feedback(user_input, correct_sentiment)
            print(f"Model retrained with new feedback! Dataset now has {total_rows} rows.")

if __name__ == "__main__":
    main()