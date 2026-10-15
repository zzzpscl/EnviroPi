from flask import Flask, make_response, render_template

app = Flask(__name__)


@app.route("/")
def index():
    return render_template("index.html")


@app.route("/api/data", methods=["GET"])
def hello_world():
    headers = {
        "Content-Type": "application/json"
    }
    return make_response("gang gang", 200, headers)


@app.route("/api/history", methods=["GET"])
def history():
    headers = {
        "Content-Type": "application/json"
    }
    return make_response("gang gang", 200, headers)
