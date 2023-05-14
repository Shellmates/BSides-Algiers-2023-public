from pwn import *

# p = process("./challenge/script.py")

p = remote("localhost", 1337)

i = 3

print(p.recvuntil(b"this: "))
print('-----------------------------------------------------')
i += 1
nums = p.recvuntil(b".")[:-1].split(b', ')
i += len(nums)

print(p.recvuntil(b"this: "))
print('-----------------------------------------------------')
i += 1
pad = p.recvuntil(b".")[:-1].decode()
i += len(pad)

print(p.recvuntil(b"this: "))
print('-----------------------------------------------------')
cipher = p.recvuntil(b".")[:-1].decode()

print('i: ',i)

p.close()

class Spooderx:
    def __init__(self):
         self.rand, self.i = 0, 2
         self.generate_random()

    def generate_random(self, m: int = 0x10ffff) -> int:
        self.rand = pow(self.i, self.rand, m)
        self.i = self.i + 1
        return self.rand

    def generate_padding(self, l: int = 0x101) -> str:
        padding = ''
        for i in range(self.generate_random(l)):
            padding += chr(self.generate_random(0xd7fb))
        return padding

spooderx = Spooderx()

def spooder_decryption(cipher: str) -> str:
    # s = int(input())
    for _ in range(0xd7fb):
        print(_)
        spooderx.i = i
        spooderx.rand = _
        t = spooderx.generate_padding()
        x = cipher[len(t):]
        flag = ''.join([chr(ord(c) ^ spooderx.generate_random(0xd7fb)) for c in x])
        if 'shell' in flag:
            return flag   

cipher = bytes.fromhex(cipher).decode()
print(spooder_decryption(cipher))