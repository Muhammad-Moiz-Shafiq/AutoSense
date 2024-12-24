from flask import Flask, request, jsonify
from flask_cors import CORS
from analysis import (classify_tweet_with_scores, log_likelihood_positive, 
                     log_likelihood_negative, log_likelihood_neutral,
                     log_prior_positive, log_prior_negative, log_prior_neutral)
import logging
logging.basicConfig(level=logging.ERROR)

app = Flask(__name__)
CORS(app)
@app.route('/')
def home():
    return "Server is running"

@app.route('/', methods=['POST'])
def analyze():
    try:
        data = request.json
        text = data.get('text', '')
        if not text:
            return jsonify({"error": "No text provided"}), 400

        predicted_sentiment, sentiment_scores = classify_tweet_with_scores(
            text, 
            log_likelihood_positive, 
            log_likelihood_negative, 
            log_likelihood_neutral,
            log_prior_positive, 
            log_prior_negative, 
            log_prior_neutral
        )
        
        return jsonify({
            'predicted_sentiment': predicted_sentiment,
            'sentiment_scores': sentiment_scores
        })
    except KeyError as e:
        logging.error(f"KeyError: {str(e)}")
        return jsonify({"error": f"Missing key: {str(e)}"}), 400
    except Exception as e:
        logging.error(f"Unhandled exception: {str(e)}")
        return jsonify({"error": str(e)}), 500

if __name__ == '__main__':
    app.run(debug=True)