global    _start

section   .data
        message:  db        "Enter message: ",0 
        bin_sh: db "/bin/sh",0

section   .text

        exit:   mov rax, 60
                mov rdi, 0
                syscall
        
        read:   mov rax, 0
                syscall
                ret
        
        write:  mov rax, 1
                syscall
                ret
        
        main:
                push rbp
                mov rbp, rsp
                mov rdi, 1
                mov rsi, message
                mov rdx, 15
                call write
                sub rsp, 0x50 
                lea rax, [rbp-0x50]
                mov rsi, rax
                mov rdi, 0
                mov rdx, 0x100
                call read
                sub rsp, 0x8
                mov [rbp-0x58], rax
                lea rax, [rbp-0x50]
                mov rsi, rax
                mov rdi, 1
                mov rdx, [rbp-0x58]
                call write
                mov rsp, rbp ; equivalent to "leave"
                pop rbp
                ret

        _start: 
                call main
                call exit
                pop rdi
                ret
                pop rsi
                ret
                pop rdx
                ret
                pop rax
                ret