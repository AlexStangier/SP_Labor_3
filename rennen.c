#include "rennen.h"

#define DISTANCE 80                             //amount of sectors to be cleared
#define SECTOR_LENGTH 10                        //sector length in m

void bsortAsc(struct runner list[], int size) {
    struct runner temp;

    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size - 1 - i; j++) {
            if(list[j].position > list[j+1].position) {
                temp = list[j+1];
                list[j+1] = list[j];
                list[j] = temp;
            }
        }
    }
}

int main() {
    time_t start = time(NULL);                  //baseline time

    int participants;
    printf("Enter the amount of Participants: \n");
    scanf("%d", &participants);

    int vmax;
    printf("Enter the VMAX in m/s: \n");
    scanf("%d", &vmax);


    pid_t runnerPids[participants];             //persist runner pids
    short runnersInField = participants;        //runners currently running

    struct runner runningField[participants];
    char participantsIds[participants];

    //create runner childs
    int i;
    for(i = 0; i < participants; i++)
    {
        //create runner struct and add to collection
        //start assinging chars to runners starting with 'A'
        participantsIds[i] = 'A' + i;

        //fork and persist pid
        if((runnerPids[i] = fork()) < 0) {
            //error during fork
            perror("Failed to fork child\n");
            return EXIT_FAILURE;
        } else {
            //LÄUFER

            //only do printout if process is currently running
            if(runnerPids[i] == 0) {
                //init srand
                srand(time(NULL) + getpid());
                //loop over sectors
                int j;
                for(j = 0; j <= DISTANCE; j++) {
                    //start running sector
                    //calc random vmax and remove 0's
                    int currentvmax = rand() % vmax +1;

                    //divide sectorlength by vmax
                    //and usleep the amount
                    //keine ganzzahl div
                    usleep((SECTOR_LENGTH/currentvmax)*1000000);

                    int k;
                    for(k = 0; k <= j; k++) {
                        printf("-");
                    }
                    //print progress
                    printf("%c \n", participantsIds[i]);
                }
                //last sector has been cleared by runner
                exit(0);
            }
        }
    }

    //SCHIEDSRICHTER

    //while runners still in field loop over waitpid
    pid_t currentPid;
    int status;
    int position = 1;
    while(runnersInField > 0) {
        currentPid = wait(&status);

        int currentRunner;
        for(currentRunner = 0; currentRunner <= participants; currentRunner++) {
            if(currentPid == runnerPids[currentRunner]) {
                //runner has finished
                //persist data for scoreboard
                runningField[currentRunner].name = participantsIds[currentRunner];
                runningField[currentRunner].runningTime = difftime(time(NULL), start);
                runningField[currentRunner].position = position++;
                runnersInField--;
            }
        }
    }



    //last has finished print scoreboard
    if(runnersInField == 0) {
        bsortAsc(runningField, participants);
        printf("POSITION RUNNER TIME\n");
        for(int l = 0; l < participants; l++) {
            printf("%2i %8c %6li s\n", runningField[l].position, runningField[l].name, runningField[l].runningTime);
        }
    }

    return EXIT_SUCCESS;
}
