#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUF_SIZE 64

char* NAME[32] ;

char* message1 = "Name succesfully stored" ;
char* message2 = "You name has been stored" ;
char* message3 = "Everything is done" ;

char* messages[3] ;

void setup(){
    setbuf(stdin,NULL);
    setbuf(stdout,NULL);
    setbuf(stderr,NULL);
    srand(time(NULL)) ;

    messages[0] = message1 ;
    messages[1] = message2 ;
    messages[2] = message3 ;
}

void* memdup(const void* src, size_t size) {
    void* dest = malloc(size);
    if (dest != NULL) {
        memcpy(dest, src, size);
    }
    return dest;
}

char* vuln(){
    char buf[BUF_SIZE] ;
    puts("Your Name:") ;
    read(0,buf,72) ;

    return memdup(buf,BUF_SIZE) ;
}

int main(int argc, char* argv[]){
    setup() ;
    char name[BUF_SIZE] ;
    while(1){
        memcpy(name,vuln(),BUF_SIZE) ;
        puts(messages[rand()%3]) ;
    }
}