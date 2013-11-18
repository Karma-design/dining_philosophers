#include "Philosophe.h"
#include "sys/wait.h"
#include <unistd.h>
#include "sys/sem.h"
//Include for rand
#include <stdlib.h>
#include <time.h>
//Include for sleep
#include <unistd.h>
//Include for signals
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#define PRINT
static struct sembuf Reserver = {0, -1, 0};
static struct sembuf Liberer = {0, 1, 0};


void my_handler(int s)
{
    printf("Caught SIGINT %d\n", s);
    exit(1);
}

void philosophe(int pPosition, int mtx_fork[])
{

    //SIGINT handler
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = my_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);

    //
    int gauche = pPosition;
    int droite = pPosition + 1 == NB_FOURCHETTES ? 0 : pPosition + 1;
    pPosition++; // Pour la lisibilité.

    state myState;
    int i;
    //pid_t myPid = getpid();
    while (1)
    {
        myState = THINK;
#ifdef PRINT
        printf("Process : %i Etat : %i.\n", pPosition, myState);
#endif
        //while (semop(semMemoire, &Reserver, 1) == -1 );
        sleep((int)rand() % 10);
#ifdef PRINT
        printf("Process : %i et j'ai fini de dormir.\n", pPosition);
#endif
        //starving, looking for forks
        myState = STARVE;
#ifdef PRINT
        printf("Process : %i Etat : %i.\n", pPosition, myState);
#endif
        for (i = 1; ; ++i)
        {
#ifdef PRINT
            printf("Process : %i et je demande ma fourchette gauche (%i).\n", pPosition, gauche);
#endif
            if (semop(mtx_fork[gauche], &Reserver, 1) != -1 )
            {
#ifdef PRINT
                printf("Process : %i et j'ai obtenu ma fourchette gauche (%i).\n", pPosition, gauche);
#endif
                int j;
                //sleep(1);
                for (j = 1; ; ++j)
                {
#ifdef PRINT
                    printf("Process : %i et je demande ma fourchette droite (%i).\n", pPosition, droite);
#endif
                    if (semop(mtx_fork[droite], &Reserver, 1) != -1 )
                    {
#ifdef PRINT
                        printf("Process : %i et j'ai obtenu ma fourchette droite (%i).\n", pPosition, droite);
#endif
                        break;
                    }
                    else
                    {
#ifdef PRINT
                        printf("Process : %i et je n'ai pas obtenu ma fourchette droite (%i)(%i essais).\n", pPosition, droite, j);
#endif
                        sleep(TIMESLICE_STARVE);
                    }
                }
                break;
            }
            else
            {
#ifdef PRINT
                printf("Process : %i et je n'ai pas obtenu ma fourchette gauche (%i)(%i essais).\n", pPosition, gauche, i);
#endif
                sleep((int)rand() % 2);
            }
        }
        myState = EAT;
#ifdef PRINT
        printf("Process : %i Etat : %i.\n", pPosition, myState);
#endif
        printf("Process : %i Etat : %i.\n", pPosition, myState);
        sleep((int)rand() % 15);
        semop(mtx_fork[gauche], &Liberer, 1);
        semop(mtx_fork[droite], &Liberer, 1);
#ifdef PRINT
        printf("Process : %i et je viens de libérer mes fourchettes.\n", pPosition);
#endif
    }
}