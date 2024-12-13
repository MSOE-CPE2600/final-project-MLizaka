// data.c
#include "data.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


int shm_id;
int sem_id;

int initialize_ipc() {

    FILE *key_file = fopen("voting_machine_key", "a");
    if (!key_file) {
        perror("failed to create file key");
        return -1;
    }
    fclose(key_file);

   key_t key = ftok("voting_machine_key", 65);
    if (key == -1) {
        perror("ftok failed");
        return -1;
    }

    shm_id = shmget(key, sizeof(struct SharedData), IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("shmget  fail");
        return -1;
    }

    sem_id = semget(key, 1, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("semget fail");
        return -1;
    }

    
    if (semctl(sem_id, 0, SETVAL, 1) == -1) {
        perror("semctl failed");
        return -1;
    }

    //init the shared mem
    struct SharedData *data = shmat(shm_id, NULL, 0);
    if (data == (void *)-1) {
        perror("shmat");
        return -1;
    }

    data->votes[0] = 0;
    data->votes[1] = 0;
    data->voter_count = 0;

    if (shmdt(data) == -1) {
        perror("shmdt fail");
        return -1;
    }

    printf("INITIALIZED!.\n");
    return 0;
}

int cleanup_ipc() {
    
    FILE *key_file = fopen("voting_machine_key", "a");
    if (!key_file) {
        perror("failed to create the key");
        return -1;
    }
    fclose(key_file);

    // Generates a key
    key_t key = ftok("voting_machine_key", 65);
    if (key == -1) {
        perror("ftok failed");
        return -1;
    }

    // gets shared mem
    shm_id = shmget(key, sizeof(struct SharedData), 0666);
    sem_id = semget(key, 1, 0666);

    // removes shared mem
    if (shm_id != -1) {
        if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
            perror("shmctl failed");
            return -1;
        }
    }

    // remove sem
    if (sem_id != -1) {
        if (semctl(sem_id, 0, IPC_RMID) == -1) {
            perror("semctl failed");
            return -1;
        }
    }

    printf("Cleaned up shared memory and semaphores.\n");
    return 0;
}

