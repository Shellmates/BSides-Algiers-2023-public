#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void disable_buffering(void);
void read_str(char *msg, char *buf, unsigned int size);

int main(int argc, char *argv[])
{
    char name[0x120] = { '\0' };

    disable_buffering();

    memset(name, (int)'A', 0x200);

    printf("Gift: %p\n", system);

    read_str("Name: ", name + 0x100, 0x40);

    __asm__("mov $1, %rcx;");

    return EXIT_SUCCESS;
}

void disable_buffering(void)
{
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
}

void read_str(char *msg, char *buf, unsigned int size)
{
    printf("%s", msg);
    if (read(STDIN_FILENO, buf, size) == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    return;
}
