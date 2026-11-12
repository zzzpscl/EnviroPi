import os
import json
from flask import Flask, make_response, render_template, jsonify
from flaskext.mysql import MySQL

app = Flask(__name__)

mysql = MySQL()
app.config['MYSQL_DATABASE_USER'] = 'root'
app.config['MYSQL_DATABASE_PASSWORD'] = 'password'
app.config['MYSQL_DATABASE_DB'] = 'EmpData'
app.config['MYSQL_DATABASE_HOST'] = 'localhost'
mysql.init_app(app)


def db_data():
    """
    cursor =mysql.connect().cursor()

    cursor.execute("SELECT * from messung")
    return cursor.fetchone()
    """

    file_path = os.path.join(app.static_folder, '../t1.json')
    with open(file_path, 'r', encoding='utf-8') as f:
        return json.load(f)

def db_room():

    file_path = os.path.join(app.static_folder, '../t2.json')
    with open(file_path, 'r', encoding='utf-8') as f:
        return json.load(f)

    """
    cursor =mysql.connect().cursor()

    cursor.execute("SELECT * from station")
    return cursor.fetchone()
    """



@app.route("/")
def index():
    return render_template("index.html")

@app.route("/api/room")
def current_room():
    headers = {
        "Content-Type": "application/json"
    }

    if not db_room():
        return make_response(503)

    return make_response(db_room(), 200, headers)

@app.route("/api/all", methods=["GET"])
def all_data():
    headers = {
        "Content-Type": "application/json"
    }

    if not db_data():
        return make_response(503)

    return make_response(db_data(), 200, headers)


@app.route("/api/current", methods=["GET"])
def current_data():
    headers = {
        "Content-Type": "application/json"
    }

    if not db_data():
        return make_response(503)


    latest = list(db_data().values())[-1]
    return make_response(jsonify(latest), 200, headers)
