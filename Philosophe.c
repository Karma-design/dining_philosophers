#include "Philosophe.h"
#include "wait.h"
#include <unistd.h>
#include "sys/sem.h"
//Include for rand
#include <stdlib.h>
#include <time.h>
//Include for sleep
#include <unistd.h>

static struct sembuf Reserver = {0, -1, 0};
static struct sembuf Liberer = {0, 1, 0};

void philosophe(int pPosition, int mtx_fork[])
{
    int gauche = pPosition;
    int droite = pPosition + 1 == NB_FOURCHETTES ? 0 : pPosition + 1;
    pPosition++; // Pour la lisibilité.

    state myState;
    int i;
    //pid_t myPid = getpid();
    while (1)
    {
        myState = THINK;
        printf("Je suis le process %i et j'entre en état %i.\n", pPosition, myState);
        //while (semop(semMemoire, &Reserver, 1) == -1 );
        sleep((int)rand() % 10);
        printf("Je suis le process %i et j'ai fini de dormir.\n", pPosition);
        //starving, looking for forks
        myState = STARVE;
        printf("Je suis le process %i et j'entre en état %i.\n", pPosition, myState);
        for (i = 1; ; ++i)
        {
            printf("Je suis le process %i et je demande ma fourchette gauche (%i).\n", pPosition, gauche);
            if (semop(mtx_fork[gauche], &Reserver, 1) != -1 )
            {
                printf("Je suis le process %i et j'ai obtenu ma fourchette gauche (%i).\n", pPosition, gauche);
                int j;
                for (j = 1; ; ++j)
                {
                    printf("Je suis le process %i et je demande ma fourchette droite (%i).\n", pPosition, droite);
                    if (semop(mtx_fork[droite], &Reserver, 1) != -1 )
                    {
                        printf("Je suis le process %i et j'ai obtenu ma fourchette droite (%i).\n", pPosition, droite);
                        break;
                    }
                    else
                    {
                        printf("Je suis le process %i et je n'ai pas obtenu ma fourchette droite (%i)(%i essais).\n", pPosition, droite, j);
                        sleep(TIMESLICE_STARVE);
                    }
                }
                break;
            }
            else
            {
                printf("Je suis le process %i et je n'ai pas obtenu ma fourchette gauche (%i)(%i essais).\n", pPosition, gauche, i);
                sleep((int)rand() % 2);
            }
        }
        myState = EAT;
        printf("Je suis le process %i et j'entre en état %i.\n", pPosition, myState);
        sleep((int)rand() % 5);
        semop(mtx_fork[gauche], &Liberer, 1);
        semop(mtx_fork[droite], &Liberer, 1);
        printf("Je suis le process %i et je viens de libérer mes fourchettes.\n", pPosition);
    }
}