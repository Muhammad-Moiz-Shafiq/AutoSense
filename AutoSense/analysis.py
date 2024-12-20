import pandas as pd
import nltk
import string
from nltk.corpus import stopwords
from nltk.stem import PorterStemmer
from collections import defaultdict
import math
from sklearn.model_selection import train_test_split
import base64
from io import BytesIO 

# Download required NLTK packages for the first time only
#nltk.download('punkt')
#nltk.download('stopwords')

# Load the dataset
csv_file_path = 'D:/Uni/Sem 3/DSA/Project/AutoSense/assets/data/train.csv'
df = pd.read_csv(csv_file_path)
df = df.drop(columns=['selected_text'])
df["text"] = df["text"].astype(str)
# Preprocessing function
def preprocess_tweet(tweet):
    tweet = tweet.lower()
    tweet = tweet.translate(str.maketrans("", "", string.punctuation))
    tokens = tweet.split()
    stemmer = PorterStemmer()
    stopwords_set = set(stopwords.words("english"))
    tokens = [stemmer.stem(token) for token in tokens if token not in stopwords_set]
    return tokens

# Step 1: Split data by sentiment
def split_data_by_sentiment(data, sentiment):
    return data[data['sentiment'] == sentiment]['text'].tolist()

positive_tweets = split_data_by_sentiment(df, 'positive')
negative_tweets = split_data_by_sentiment(df, 'negative')
neutral_tweets = split_data_by_sentiment(df, 'neutral')

# Split the data into train and test sets
train_df, test_df = train_test_split(df, test_size=0.1, random_state=42)

# Step 2: Calculate word counts for a given sentiment
def calculate_word_counts(tweets):
    word_count = defaultdict(int)
    for tweet in tweets:
        tokens = preprocess_tweet(tweet)
        for token in tokens:
            word_count[token] += 1
    return word_count

word_count_positive = calculate_word_counts(train_df[train_df['sentiment'] == 'positive']['text'])
word_count_negative = calculate_word_counts(train_df[train_df['sentiment'] == 'negative']['text'])
word_count_neutral = calculate_word_counts(train_df[train_df['sentiment'] == 'neutral']['text'])

# Display word counts
print("Word Counts - Positive Sentiment:")
print(word_count_positive)

print("\nWord Counts - Negative Sentiment:")
print(word_count_negative)

print("\nWord Counts - Neutral Sentiment:")
print(word_count_neutral)

# Step 3: Calculate likelihood using Laplacian smoothing
def calculate_likelihood(word_count, total_words, laplacian_smoothing=1):
    likelihood = {}
    vocabulary_size = len(word_count)

    for word, count in word_count.items():
        likelihood[word] = (count + laplacian_smoothing) / (total_words + laplacian_smoothing * vocabulary_size)

    return likelihood

total_positive_words = sum(word_count_positive.values())
total_negative_words = sum(word_count_negative.values())
total_neutral_words = sum(word_count_neutral.values())

likelihood_positive = calculate_likelihood(word_count_positive, total_positive_words)
likelihood_negative = calculate_likelihood(word_count_negative, total_negative_words)
likelihood_neutral = calculate_likelihood(word_count_neutral, total_neutral_words)

# Step 4: Calculate log likelihood
log_likelihood_positive = {word: math.log(prob) for word, prob in likelihood_positive.items()}
log_likelihood_negative = {word: math.log(prob) for word, prob in likelihood_negative.items()}
log_likelihood_neutral = {word: math.log(prob) for word, prob in likelihood_neutral.items()}

# Step 5: Calculate log prior
def calculate_log_prior(sentiment, data):
    return math.log(len(data[data['sentiment'] == sentiment]) / len(data))

log_prior_positive = calculate_log_prior('positive', df)
log_prior_negative = calculate_log_prior('negative', df)
log_prior_neutral = calculate_log_prior('neutral', df)

# Step 6: Classify a new tweet
def classify_tweet_with_scores(tweet, log_likelihood_positive, log_likelihood_negative, log_likelihood_neutral,
                   log_prior_positive, log_prior_negative, log_prior_neutral):
    tokens = preprocess_tweet(tweet)

    log_score_positive = log_prior_positive + sum([log_likelihood_positive.get(token, 0) for token in tokens])
    log_score_negative = log_prior_negative + sum([log_likelihood_negative.get(token, 0) for token in tokens])
    log_score_neutral = log_prior_neutral + sum([log_likelihood_neutral.get(token, 0) for token in tokens])

    sentiment_scores = {
        'positive': log_score_positive,
        'negative': log_score_negative,
        'neutral': log_score_neutral
    }

    predicted_sentiment = max(sentiment_scores, key=sentiment_scores.get)
    return predicted_sentiment, sentiment_scores

def main():
    # Get input from user through CLI
    print("\n=== Sentiment Analysis with Naïve Bayes ===\n")
    while True:
        user_input = input("\nEnter text to analyze (or 'quit' to exit): ")
        
        if user_input.lower() == 'quit':
            print("\nGoodbye!")
            break
            
        predicted_sentiment, sentiment_scores = classify_tweet_with_scores(
            user_input, 
            log_likelihood_positive, 
            log_likelihood_negative, 
            log_likelihood_neutral,
            log_prior_positive, 
            log_prior_negative, 
            log_prior_neutral
        )
        
        print(f"\nPredicted Sentiment: {predicted_sentiment.upper()}")
        print("\nSentiment Scores:")
        for sentiment, score in sentiment_scores.items():
            print(f"{sentiment}: {score:.4f}")

if __name__ == "__main__":
    main()
