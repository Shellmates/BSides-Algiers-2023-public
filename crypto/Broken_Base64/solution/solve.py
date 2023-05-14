from base64 import b64decode as b64dec

enc = "lbGxtYXRlc3tZMHVfaDRWM183MF91TmQzcjV0NG5EX0gwd19CNDUzNjRfdzBSazV9"

for i in range(10):
  try:
    d = b64dec("A"*i + enc)
    print(d)
    break
  except:
    pass
  
  # you just have to fix the flag prefix shellmates{}
