CC=gcc
CFLAGS=-no-pie
SOURCE=src/champions-heap.c
SECCOMP_LIB=seccomp
OUTFILE=challenge/champions-heap

build: $(SOURCE)
	$(CC) $(CFLAGS) -D BuiltWithMake -o $(OUTFILE) $(SOURCE) -l$(SECCOMP_LIB)
	chmod +x $(OUTFILE)
