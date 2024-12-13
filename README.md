# Voting Machine
This program allows to terminals to contribute to a vote. It enables users to log a vote with a name attached to it ensuring voters can only vote once each pull
it allso allows users to view the real time pull infomation wit the -r display command.
and finally users can destroy all the pull data from the machine with the -c command which cleans up all connections.


## ALLOWED FEATURES
- allows multiple terminals to be voting
- tracks votes using shared memory
- allows user to print rsults or clear the data from the program 

## HOW TO USE 
first initilize: ./voting_machine -i
make a vote:./voting_machine -v
view vote count:./votingmachine -r
cleanup:./voting_machine -c

### COMPILE INSTRUCTIONS
make 
make clean 
./voting_machine -(option)