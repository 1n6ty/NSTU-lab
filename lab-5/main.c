#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <time.h>

#define MAX_PROGS 3
#define PROG_LEN 50
#define NUM_CHILDREN 3
#define WORK_CYCLES 5

struct msgbuf {
    long mtype;
    char mtext[256];
};

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void child_process(int child_idx, int shmid, int semid, int msgid) {
    char (*shm_ptr)[PROG_LEN] = shmat(shmid, NULL, 0);
    srand(time(NULL));

    struct sembuf acquire = {child_idx, -1, IPC_NOWAIT};
    struct sembuf release = {child_idx, 1, 0};

    for (int i = 0; i < WORK_CYCLES; i++) {
        sleep(rand() % 3 + 1); // Just immitating work

        char *prog_name = shm_ptr[rand() % MAX_PROGS];

        if (semop(semid, &acquire, 1) == 0) {
            struct msgbuf msg;
            msg.mtype = 1;
            snprintf(msg.mtext, sizeof(msg.mtext), "Child process #%d (PID %d) started: %s", child_idx, getpid(), prog_name);
            msgsnd(msgid, &msg, sizeof(msg.mtext), 0);

            if (fork() == 0) {
                system(prog_name);

                semop(semid, &release, 1);
                exit(0);
            }
        }
    }

    while(wait(NULL) > 0);

    struct msgbuf exit_msg;
    exit_msg.mtype = 2;
    msgsnd(msgid, &exit_msg, sizeof(exit_msg.mtext), 0);

    shmdt(shm_ptr);
    exit(0);
}

int main() {
    key_t key = IPC_PRIVATE; 

    int shmid = shmget(key, MAX_PROGS * PROG_LEN, IPC_CREAT | 0666);
    char (*shm_ptr)[PROG_LEN] = shmat(shmid, NULL, 0);
    strcpy(shm_ptr[0], "./traverse .");
    strcpy(shm_ptr[1], "./ln");
    strcpy(shm_ptr[2], "./msg");

    int semid = semget(key, NUM_CHILDREN, IPC_CREAT | 0666);
    union semun arg;
    unsigned short sem_init_vals[NUM_CHILDREN];
    for (int i = 0; i < NUM_CHILDREN; i++) sem_init_vals[i] = 1;
    arg.array = sem_init_vals;
    semctl(semid, 0, SETALL, arg);

    int msgid = msgget(key, IPC_CREAT | 0666);

    printf("Parent process (PID %d) initialized IPC...\n\n", getpid());

    for (int i = 0; i < NUM_CHILDREN; i++) {
        if (fork() == 0) {
            child_process(i, shmid, semid, msgid);
        }
    }

    int exited_children = 0;
    struct msgbuf msg;

    while (exited_children < NUM_CHILDREN) {
        if (msgrcv(msgid, &msg, sizeof(msg.mtext), 0, 0) != -1) {
            if (msg.mtype == 1) {
                printf("[MSG queue]: %s\n", msg.mtext);
            } else if (msg.mtype == 2) {
                exited_children++;
            }
        }
    }

    while(wait(NULL) > 0);

    printf("\nAll children are dead. IPC is being deleted...\n");

    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID, arg);
    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}