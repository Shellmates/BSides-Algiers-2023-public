#!/bin/bash

nasm -f elf64 challenge.asm
ld -m elf_x86_64 -s -o chall challenge.o