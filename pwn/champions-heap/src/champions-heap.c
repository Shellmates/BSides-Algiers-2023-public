#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <assert.h>
#include <seccomp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NUM_TEAMS 9

void disable_buffering(void);
void read_str(char *msg, char *buf, unsigned int size);
long read_num(char *msg);
void menu(void);
void sandbox(void);
unsigned int randint(unsigned int min, unsigned int max);
void init_teams(void);
void add_team(void);
void print_teams(void);
int select_team(int avoid);
void simulate_game(void);
void reset_teams(void);

char *teams[NUM_TEAMS] = {
    "Real Madrid",
    "Manchester City",
    "A.C. Milan",
    "Inter Milan",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};

unsigned int num_teams = 4;
int myteam_idx = -1;

long *gd;
char name[0x20] = { '\0' };
unsigned int seed;

void sandbox(void) {
  scmp_filter_ctx ctx = seccomp_init(SCMP_ACT_ALLOW);
  if (ctx == NULL) {
    perror("seccomp_init");
    exit(EXIT_FAILURE);
  }
  
  if (seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(execve), 0) == -1 ||
      seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(execveat), 0) == -1) {
    perror("seccomp_rule_add");
    exit(EXIT_FAILURE);
  }
  
  if (seccomp_load(ctx) == -1) {
    perror("seccomp_load");
    exit(EXIT_FAILURE);
  }

  return;
}

unsigned int randint(unsigned int min, unsigned int max) {
    return (unsigned int)(rand() % (max - min + 1) + min);
}

void init_teams(void) {
    unsigned int i;

    gd = (long*)malloc(sizeof(long) * NUM_TEAMS);
    printf("Gift: %p\n", gd);

    for (i = 0; i < NUM_TEAMS; i++) {
        gd[i] = 0;
    }
    
    return;
}

void add_team(void) {
    size_t size = 0;
    char *team = NULL;

    if (num_teams == NUM_TEAMS) {
        fprintf(stderr, "Max number of teams reached!");
        return;
    }

    size = (size_t)read_num("Team name size: ");
    if (size == 0) {
        fprintf(stderr, "Invalid size!");
        return;
    }

    if ((team = (char*)malloc(size)) == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    read_str("Team name: ", team, size);

    teams[num_teams] = team;
    num_teams++;

    return;
}

void print_teams(void) {
    unsigned int i;

    printf("\n| %2s | %16s | %3s |\n", "No", "Team name", "GD");
    for (i = 0; i < NUM_TEAMS && teams[i] != NULL; i++) {
        printf("| %2d | %16s | %3ld |\n", i + 1, teams[i], gd[i]);
    }
    putchar('\n');

    return;
}

int select_team(int avoid) {
    int idx = -1;
    print_teams();

    while (idx < 0 || idx >= num_teams || idx == avoid) {
        idx = ((int)read_num("Select team: ")) - 1;
    }
    printf("Selecting '%s'\n", teams[idx]);

    return idx;
}

void simulate_game(void) {
    int opteam_idx = -1;
    char answer = '\0';
    unsigned long myscore = 0, opscore = 0;

    if (myteam_idx < 0 || myteam_idx >= num_teams) {
        myteam_idx = select_team(-1);
    }

    printf("Choose opponent randomly? [y/n] ");
    answer = getchar();
    while ((getchar()) != '\n');

    if (answer == 'y') {
        opteam_idx = myteam_idx;
        while (opteam_idx == myteam_idx) {
            opteam_idx = randint(0, NUM_TEAMS);
        }
    } else {
        opteam_idx = select_team(myteam_idx);
    }

    printf("%s vs. %s\n", teams[myteam_idx], teams[opteam_idx]);
    printf("Simulating...\n");
    // Real Madrid CL armour
    if (myteam_idx == 0) {
        myscore = 5;
    } else {
        myscore = randint(0, 4);
    }
    opscore = randint(0, 4);
    printf("%s %lu - %lu %s\n", teams[myteam_idx], myscore, opscore, teams[opteam_idx]);
    if (myscore == opscore) {
        printf("It'a draw!");
    } else if (myscore > opscore) {
        printf("Your team won!");
    } else {
        printf("Your team lost!");
    }
    putchar('\n');

    gd[myteam_idx] += myscore - opscore;
    gd[opteam_idx] += opscore - myscore;

    return;
}

void reset_teams(void) {
    unsigned int i;

    for (i = 0; i <= NUM_TEAMS; i++) {
        gd[i] = 0;
    }

    return;
}

void menu(void)
{
    puts("\n1) Add team");
    puts("2) Show teams");
    puts("3) Select my team");
    puts("4) Simulate a game");
    puts("5) Reset teams");
    puts("0) Exit");
}

int main(int argc, char *argv[])
{
    int fd;
    ssize_t bytes_read;

    disable_buffering();
    sandbox();
    init_teams();

    fd = open("/dev/urandom", O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    bytes_read = read(fd, &seed, sizeof(seed));
    close(fd);
    if (bytes_read != sizeof(seed)) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    srand(seed);

    read_str("Enter player name: ", name, sizeof(name));

    printf("Hi %s, welcome to the Champions Heap!", name);

    while (true)
    {
        menu();
        switch (read_num("Choice: "))
        {
        case 1:
            add_team();
            break;
        case 2:
            print_teams();
            break;
        case 3:
            myteam_idx = select_team(-1);
            break;
        case 4:
            simulate_game();
            break;
        case 5:
            reset_teams();
            print_teams();
            break;
        case 0:
            puts("Bye!");
            return EXIT_SUCCESS;
        default:
            fprintf(stderr, "Invalid option!\n");
            exit(EXIT_FAILURE);
            break;
        }
    }

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

long read_num(char *msg)
{
    char buf[0x20] = {'\0'};

    read_str(msg, buf, 0x1f);
    return atol(buf);
}
