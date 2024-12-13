#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "vote.h"
#include "data.h"

void print_usage() {
    printf("enter: -i | -v | -r | -c\n");
}

int main(int argc, char *argv[]) {
    int opt;
    int initialize = 0, vote = 0, result = 0, cleanup = 0;

    while ((opt = getopt(argc, argv, "ivrc")) != -1) {
        switch (opt) {
            case 'i':
                initialize = 1;
                break;
            case 'v':
                vote = 1;
                break;
            case 'r':
                result = 1;
                break;
            case 'c':
                cleanup = 1;
                break;
            default:
                print_usage();
                return EXIT_FAILURE;
        }
    }

    if (initialize) {
        printf("INITILIZING\n");
        if (initialize_ipc() != 0) {
            printf("failed to init\n");
            return EXIT_FAILURE;
        }
    } else if (vote) {
        char username[50];
        int candidate_id;
        printf("Enter a name for your vote: ");
        if (scanf("%49s", username) != 1) {
            printf("failed getting name\n");
            return EXIT_FAILURE;
        }
        printf("do you want to vote for canidite 1 or canadite 2: ");
        if (scanf("%d", &candidate_id) != 1) {
            printf("failed getting the canadite.\n");
            return EXIT_FAILURE;
        }
        if (cast_vote(username, candidate_id) != 0) {
            printf("failed to vote.\n");
            return EXIT_FAILURE;
        }
    } else if (result) {
        printf("returning the results of the pull\n");
        if (show_results() != 0) {
            printf("failed getting results\n");
            return EXIT_FAILURE;
        }
    } else if (cleanup) {
        printf("Cleaning up\n");
        if (cleanup_ipc() != 0) {
            printf("failed to clean up\n");
            return EXIT_FAILURE;
        }
    } else {
        print_usage();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
