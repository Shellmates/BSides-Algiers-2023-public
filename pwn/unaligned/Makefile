CC=gcc
CFLAGS=-fno-stack-protector
SOURCE=src/unaligned.c
OUTFILE=challenge/unaligned

build: $(SOURCE)
	$(CC) $(CFLAGS) -D BuiltWithMake -o $(OUTFILE) $(SOURCE)
	chmod +x $(OUTFILE)
