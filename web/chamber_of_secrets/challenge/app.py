from gevent import monkey
monkey.patch_all()

from flag import FLAG
import os
from tinymongo import TinyMongoClient
from flask import Flask, render_template
from flask_cors import CORS
from gevent.pywsgi import WSGIServer
from flask_sock import Sock
import json

connection = TinyMongoClient('.')
db = connection.maSQLsh
secrets = db.secrets  

def init_db():
    secrets.remove({})  
    secret = {'chamber': os.urandom(12).hex(), 'secret': FLAG}
    print(secret)
    secrets.insert_one(secret)


def create_app():
    app = Flask(__name__)
    cors = CORS(app, resources={'/*': {"origins": "*"}})
    init_db()
    return app

app = create_app()
sock = Sock(app)


@app.route('/', methods=['GET'])
def index():      
    return render_template('index.html')


@sock.route('/')
def login(sock):
    while True:
        data = sock.receive()
        creds = json.loads(data)
        chamber = creds['chamber']
        secret = creds['secret']

        if not chamber:
            msg = 'Chamber is required.'
        elif not secret:
            msg = 'Secret is required.'
        else:
            query = {"$and":[{'chamber': chamber, 'secret': secret}]}
            user = secrets.find_one(query)
            if user is None:
                msg = 'Sike! that\'s the wrong number!'
            else: 
                msg = 'Looks like you know your secret!'

        sock.send(msg)


if __name__ == "__main__":
    # Production
    print("starting the server ....")
    http_server = WSGIServer(('', 8000), app, spawn=100)
    print("Server is Up, enjoy :)")
    http_server.serve_forever()
