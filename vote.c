#include "data.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>



extern int shm_id;
extern int sem_id;

int cast_vote(const char *username, int candidate_id) {
    if (candidate_id < 1 || candidate_id > 2) {
        printf("either choos canadite 1 or 2.\n");
        return -1;
    }

    
    key_t key = ftok("voting_machine_key", 65);
    if (key == -1) {
        printf("ftok failed \n");
        return -1;
    }

    shm_id = shmget(key, sizeof(struct SharedData), 0666);
    if (shm_id == -1) {
        printf("shmget failed");
        return -1;
    }

    sem_id = semget(key, 1, 0666);
    if (sem_id == -1) {
        printf("semget failed \n");
        return -1;
    }

    

    
    struct SharedData *data = shmat(shm_id, NULL, 0);
    if (data == (void *)-1) {
        if (errno == EINVAL) {
            printf("initilize voting system mem first\n");
        } else {
            printf("shmat failed \n");
        }
        return -1;
    }

    
    struct sembuf lock = {0, -1, 0};
    if (semop(sem_id, &lock, 1) == -1) {
        if (errno == EINVAL) {
            printf("initilize first.\n");
        } else {
            printf("semop lock failed \n");
        }
        return -1;
    }

    
    for (int i = 0; i < data->voter_count; i++) {
        if (strcmp(data->voters[i], username) == 0) {
            printf("this user cannot vote again\n");
            struct sembuf unlock = {0, 1, 0};
            semop(sem_id, &unlock, 1);
            return -1;
        }
    }

    
    data->votes[candidate_id - 1]++;
    strncpy(data->voters[data->voter_count], username, MAX_USERNAME_LENGTH);
    data->voter_count++;

    
    struct sembuf unlock = {0, 1, 0};
    if (semop(sem_id, &unlock, 1) == -1) {
        printf("semop unlock failed \n");
        return -1;
    }

   
    if (shmdt(data) == -1) {
        printf("shmdt failed \n");
        return -1;
    }

    printf("voded for %d by  %s.\n", candidate_id, username);
    return 0;
}




int show_results() {
    
    key_t key = ftok("voting_machine_key", 65);
    if (key == -1) {
        printf("ftok failed \n");
        return -1;
    }

    
    shm_id = shmget(key, sizeof(struct SharedData), 0666);
    if (shm_id == -1) {
        printf("shmget failed \n");
        return -1;
    }

    
    sem_id = semget(key, 1, 0666);
    if (sem_id == -1) {
        printf("semget failed \n");
        return -1;
    }

    

    
    struct SharedData *data = shmat(shm_id, NULL, 0);
    if (data == (void *)-1) {
        if (errno == EINVAL) {
            printf("initilize first\n");
        } else {
            printf("shmat failed \n");
        }
        return -1;
    }

    
    struct sembuf lock = {0, -1, 0};
    if (semop(sem_id, &lock, 1) == -1) {
        if (errno == EINVAL) {
            printf("initilize first\n");
        } else {
            printf("semop lock failed\n");
        }
        return -1;
    }

     
    printf("Vote Results:\n");
    printf("Candidate 1: %d votes\n", data->votes[0]);
    printf("Candidate 2: %d votes\n", data->votes[1]);

    
    struct sembuf unlock = {0, 1, 0};
    if (semop(sem_id, &unlock, 1) == -1) {
        printf("semop unlock failed \n");
        return -1;
    }

    
    if (shmdt(data) == -1) {
        printf("shmdt failed \n");
        return -1;
    }

    return 0;
}


