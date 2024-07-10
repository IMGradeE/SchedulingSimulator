#include <stdio.h>
#include <string.h>

/*
 *
Write a scheduling simulator in c with the following required commandline parameters:
n - the number of processes to simulate
k - the maximum time step when a process can be created
min_time - the minimum time a process needs to run
max_time - the maximum time a process needs to run
these can be positional parameters or you can use a more complex parameter parsing technique.
If the program is run with missing or incorrect parameters print a help message indicating the proper usage.
If the program is run without any parameters you should use the following default values:

n = 10
k = 100
min_time= 7
max_time= 15
 *
 ALSO:
 implement a --help command
 * */

int main(int argc, char* argv[]) {
    if (argc == 1){ // only the name of the program is passed
        //TODO run sim with defaults
    }
    else if (strcmp(argv[1],"--help")==1){
        // print help
    }else if (argc == 5){
        //TODO proper number of arguments were passed, next:
            //tokenize and attempt to cast to integers
            // if this fails for any argument:
                // show help dialogue.
        strtok_s()
    }

    printf("Hello, World!\n");
    return 0;
}

int sim(){
    //TODO implement simulation
}