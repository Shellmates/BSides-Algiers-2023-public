from flask import Flask, render_template, request
import sqlite3

app = Flask(__name__)

from db import *

init_db()

@app.route('/jokes', methods=['POST'])
def get_joke():
    try:
        data = request.get_json()
        key = list(data.keys())[0]
        id = data[key]
        assert isinstance(id,int)
        row = get_joke_column(key, id)
        assert len(row) and isinstance(row[0],str)

        res = dict(result= row[0])
    except Exception as e:
        print(e)
        res = dict(result="Couldn't retrieve results")
    return res

@app.route('/')
def home():
    return render_template("index.html")

if __name__ == '__main__':
    app.run(debug=True)
