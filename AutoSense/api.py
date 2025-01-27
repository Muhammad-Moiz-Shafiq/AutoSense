from flask import Flask, request, jsonify
from flask_cors import CORS
import logging
logging.basicConfig(level=logging.ERROR)
import analysis
app = Flask(__name__)
CORS(app)

# Initialize SentimentAnalyzer with your dataset
analyzer = analysis.SentimentAnalyzer('D:/Uni/Sem 3/DSA/Project/AutoSense/assets/data/train.csv')

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

        predicted_sentiment, sentiment_scores = analyzer.classify_text(
            text
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

@app.route('/feedback', methods=['POST'])
def receive_feedback():
    try:
        feedback_data = request.json
        text = feedback_data.get('text', '')
        correct_sentiment = feedback_data.get('correct_sentiment', '')

        if not text or not correct_sentiment:
            return jsonify({"error": "Invalid feedback data"}), 400

        # Assuming you have a SentimentAnalyzer instance
        analyzer.add_feedback(text, correct_sentiment)

        return jsonify({"message": "Feedback received and processed"}), 200
    except Exception as e:
        logging.error(f"Unhandled exception in feedback: {str(e)}")
        return jsonify({"error": str(e)}), 500

if __name__ == '__main__':
    app.run(debug=True)