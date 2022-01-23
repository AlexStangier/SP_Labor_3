#ifndef rennen
#define rennen

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

struct runner {
    char name;
    time_t runningTime;
    int position;
};

void bsortAsc(struct runner list[], int size);

#endif
