from flask import Flask, request
import os

#shellmates{it5_l1k3_5q1i73_0n_573r01d5}

flagger = Flask(__name__)

SECRET = os.getenv("SECRET")

@flagger.route('/getFlag/<secret>', methods=['GET'])
def getFlag(secret):
    if secret == SECRET:
        return os.popen("/flag").read()
    return "Wrong secret"

@flagger.route('/fileReader/', methods=['GET'])
def getFile():
    file = request.args.get("file","")
    if os.path.exists(file):
        return open(file).read()
    return "File doesn't exist"