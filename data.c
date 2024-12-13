/**
 * data.c
 * andrew lizak
 * 12/12/2024
 */
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
        printf("failed to create file key\n");
        return -1;
    }
    fclose(key_file);

   key_t key = ftok("voting_machine_key", 65);
    if (key == -1) {
        printf("ftok failed \n");
        return -1;
    }

    shm_id = shmget(key, sizeof(struct SharedData), IPC_CREAT | 0666);
    if (shm_id == -1) {
        printf("shmget  fail \n");
        return -1;
    }

    sem_id = semget(key, 1, IPC_CREAT | 0666);
    if (sem_id == -1) {
        printf("semget fail \n");
        return -1;
    }

    
    if (semctl(sem_id, 0, SETVAL, 1) == -1) {
        printf("semctl failed \n");
        return -1;
    }

    //init the shared mem
    struct SharedData *data = shmat(shm_id, NULL, 0);
    if (data == (void *)-1) {
        printf("shmat \n");
        return -1;
    }

    data->votes[0] = 0;
    data->votes[1] = 0;
    data->voter_count = 0;

    if (shmdt(data) == -1) {
            printf("shmdt fail \n");
        return -1;
    }

    printf("INITIALIZED!.\n");
    return 0;
}

int cleanup_ipc() {
    
    FILE *key_file = fopen("voting_machine_key", "a");
    if (!key_file) {
        printf("failed to create the key \n");
        return -1;
    }
    fclose(key_file);

    // Generates a key
    key_t key = ftok("voting_machine_key", 65);
    if (key == -1) {
        printf("ftok failed \n");
        return -1;
    }

    // gets shared mem
    shm_id = shmget(key, sizeof(struct SharedData), 0666);
    sem_id = semget(key, 1, 0666);

    // removes shared mem
    if (shm_id != -1) {
        if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
            printf("shmctl failed \n");
            return -1;
        }
    }

    // remove sem
    if (sem_id != -1) {
        if (semctl(sem_id, 0, IPC_RMID) == -1) {
            printf("semctl failed \n");
            return -1;
        }
    }

    printf("Cleaned up shared memory and semaphores.\n");
    return 0;
}

