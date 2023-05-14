from flask import Flask, request, render_template
import subprocess
import shlex
import json

curlCommand = ['curl', '-i']
errorMessage = {'passed': False, 'message':"Couldn't connect to the website"}

app = Flask(__name__)

@app.route('/', methods=['GET'])
def index():
    return render_template('index.html')

@app.route('/', methods=['POST'])
def executeCurl(): 
    try : 
        host = [request.json['host']]
        assert host[0].startswith("http")
        curlOptions = request.json['options']
        command = curlCommand + host    
        for option in curlOptions :
            command.append(shlex.quote(option))
            command.append(shlex.quote(curlOptions[option]))
            
        result = subprocess.run(command, capture_output=True)
        statusCode = result.stdout.decode().split()[1]
        response = {'statusCode': statusCode, 'passed': True}
        return json.dumps(response)
    except :
        return errorMessage
    

if __name__ == '__main__':
    app.run()
