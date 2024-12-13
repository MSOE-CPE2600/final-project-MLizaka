#ifndef DATA_H
#define DATA_H

#define MAX_VOTERS 100
#define MAX_USERNAME_LENGTH 50

//has shared mem for the program 
struct SharedData {
    int votes[2]; 
    char voters[MAX_VOTERS][MAX_USERNAME_LENGTH]; 
    int voter_count; 
};

int initialize_ipc();
int cleanup_ipc();

#endif
