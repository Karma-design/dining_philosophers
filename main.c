#include "Philosophe.h"
#include "wait.h"
#include <unistd.h>
#include "sys/msg.h"
#include "sys/sem.h"

int main(int argc, char const *argv[])
{
    int mtx_fork[NB_FOURCHETTES];
    int i;
    for (i = 0; i < NB_FOURCHETTES; ++i)
    {
        mtx_fork[i] = semget ( ftok ( "./", i ), 1, IPC_CREAT | CREDENTIAL );
        semctl ( mtx_fork[i], 0, SETVAL, 1);
    }

    int time = 0;
    int step = 5;
    pid_t lol[NB_PHILOSOPHES];
    i = 0;
    if (lol[i++] = fork() == 0)
    {
        sleep(time);
        philosophe(i - 1, mtx_fork);
    }
    else if (lol[i++] = fork() == 0)
    {
        step = 8 - step;
        time += step;
        sleep(time);
        philosophe(i - 1, mtx_fork);
    }
    else if (lol[i++] = fork() == 0)
    {
        step = 8 - step;
        time += step;
        sleep(time);
        philosophe(i - 1, mtx_fork);
    }
    else if (lol[i++] = fork() == 0)
    {
        step = 8 - step;
        time += step;
        sleep(time);
        philosophe(i - 1, mtx_fork);
    }
    else if (lol[i++] = fork() == 0)
    {
        step = 8 - step;
        time += step;
        sleep(time);
        philosophe(i - 1, mtx_fork);
    }
    else
    {
        for (i = 0; i < NB_PHILOSOPHES; ++i)
        {
            waitpid(lol[i], NULL, 0);
        }
    }

    for (i = 0; i < NB_FOURCHETTES; ++i)
    {
        semctl ( mtx_fork[i], 0, IPC_RMID, 0 );
    }

    return 0;
}