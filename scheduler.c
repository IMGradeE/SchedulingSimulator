#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

struct process{
    int pid;
    _Bool active;
    int arrivalTime;
    int remainingCPUTime;
    int turnaroundTime;
    int totalCPUTime;
};
long random_at_most_from_StackOverflow(long max);
void sim(struct process table[], int n, int function_id);
int first_in_first_out(struct process readyList[], struct process runningProc[], int count, int pid);
struct process * create_table(int n, int k, int min_time, int max_time);
int help(int pArgC, const char* badArg, int badArgPos);

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
 * */

int main(const int argc,const char* argv[]) {
    if (argc <= 1){ // only the name of the program is passed
        for (int i = 0; i < 3; ++i) {
            struct process *table = create_table(10, 100, 7, 15);
            if (i == 0){
                printf("%-10s %-10s %-10s %-10s %-10s %-10s\n", "PID", "Active", "Arrival", "Total", "Remaining", "Turnaround");
                for (int j = 0; j < 10; ++j) {
                    struct process *p = &table[j];
                    // print row
                    printf("%-10d %-10d %-10d %-10d %-10d %-10d\n", p->pid, p->active, p->arrivalTime, p->totalCPUTime, p->remainingCPUTime, p->turnaroundTime);
                }
            }
            sim(table, 10, i);
            free(table);
        }
    }else if (argc == 5){
        int n, k, min_time, max_time;
        int *simArgs[] = {&n, &k, &min_time, &max_time};
        char *end = " ";
        for(int i = 1; i < argc; ++i){
            // Using strol because atoi does not provide any error feedback
            *simArgs[i-1] = (int) strtol(argv[i], &end,10); // assign values to n, k, min_time, max_time iteratively.
            if(simArgs[i-1] == 0){ // check if conversion to long integer was successful
                // print help dialogue
                return help(0, argv[i], i); // return after printing help dialogue
            }
        }
        for (int i = 0; i < 3; ++i) {
            struct process *table = create_table(n, k, min_time, max_time);
            if (i == 0){
                printf("%-10s %-10s %-10s %-10s %-10s %-10s\n", "PID", "Active", "Arrival", "Total", "Remaining", "Turnaround");
                for (int j = 0; j < n; ++j) {
                    struct process *p = &table[j];
                    // print row
                        printf("%-10d %-10d %-10d %-10d %-10d %-10d\n", p->pid, p->active, p->arrivalTime, p->totalCPUTime, p->remainingCPUTime, p->turnaroundTime);
                }
            }
            sim(table, n, i);
            free(table);
        }
        return 0; // return 0 after calling the simulation successfully.

    }else{
        // Either:
            // print help dialogue for too few or too many arguments
            // print help dialogue for explicit help call if(argv[1] == "--help" || argv[1] == "-h")
        return help(argc-5, argv[1], 0);
    }

}


// This function is from StackOverflow @ https://stackoverflow.com/a/6852396
// I do understand the function, but I did not write it myself.
// Assumes 0 <= max <= RAND_MAX
// Returns in the closed interval [0, max]
long random_at_most_from_StackOverflow(long max) {
    unsigned long
    // max <= RAND_MAX < ULONG_MAX, so this is okay.
    num_bins = (unsigned long) max + 1,
            num_rand = (unsigned long) RAND_MAX + 1,
            bin_size = num_rand / num_bins,
            defect   = num_rand % num_bins;

    long x;
    do {
        x = random();
    }
        // This is carefully written not to overflow
    while (num_rand - defect <= (unsigned long)x);

    // Truncated division is intentional
    return x/bin_size;
}
// End of StackOverflow source

int first_in_first_out(struct process readyList[], struct process table[], int count, int pid) {
    if(pid == -1 || table[pid].remainingCPUTime == 0) {
        int minArrival = INT_MAX;
        for (int j = 0; j < count; ++j) {
            if (minArrival > readyList[j].arrivalTime && readyList[j].remainingCPUTime > 0) {
                minArrival = readyList[j].arrivalTime;
                pid = readyList[j].pid;
            }
        }
    }
    return pid;
}
int shortest_job_first(struct process readyList[], struct process table[], int count, int pid) {
    if(pid == -1 || table[pid].remainingCPUTime == 0) {
        int minTime = INT_MAX;
        for (int j = 0; j < count; ++j) {
            if (minTime > readyList[j].totalCPUTime && readyList[j].totalCPUTime > 0) {
                minTime = readyList[j].totalCPUTime;
                pid = readyList[j].pid;
            }
        }
    }
    return pid;
}
int shortest_remaining_time(struct process readyList[], struct process table[], int count, int pid) {

    int minTime = INT_MAX;
    for (int j = 0; j < count; ++j) {
        if (minTime > readyList[j].remainingCPUTime && readyList[j].remainingCPUTime > 0) {
            minTime = readyList[j].remainingCPUTime;
            pid = readyList[j].pid;
        }
    }

    return pid;
}
void sim(struct process table[], int n, int function_id){
    int
            t = 0,
            runningPID = -1,
            count,
            lastIndex,
            allTerminated;
    struct process *p;
    do{
        allTerminated = n;
        count = 0;
        struct process readyList[n];
        for (int i = 0; i < n; ++i) {
            p = &table[i];
            // if all processes are terminated, this reaches 0 and exits.
            allTerminated -= (p->remainingCPUTime == 0)?1:0;
            // mark process as active when arrivalTime >= t and remainingCPUTime > 0.
            p->active = (p->arrivalTime <= t && p->remainingCPUTime > 0)?1:0;
            if (p->active == 1) {
                lastIndex = i;
                readyList[count] = *p;
                ++count;
            }
            // increment turnaround time while p is active
            p->turnaroundTime += (p->active == 1)?1:0;
        }
        char strComponent[64];
        sprintf(strComponent, "Time: %d", t);
        char strOut[128];
        if (count == 0 && allTerminated != 0){ // also catches runningPID == -1.
            sprintf(strOut, "%-13s - IDLE", strComponent);
        }else{
            // select process to run based on caller provided value
            if(function_id == 0){ //FIFO
                runningPID = first_in_first_out(readyList, table, count, runningPID);
            }else if(function_id == 1){ //SJF
                runningPID = shortest_job_first(readyList, table, count, runningPID);
            }else{ // SRT
                runningPID = shortest_remaining_time(readyList, table, count, runningPID);
            }
            --table[runningPID].remainingCPUTime;
            sprintf(strOut, "%-13s - PID: %d", strComponent, runningPID);
        }
        if(allTerminated != 0)
        printf("%-s\n",strOut);
        ++t;
    }while (allTerminated > 0);
    int sum = 0;
    char *functionName;
    if(function_id == 0){
        functionName = "FCFS";
    }else if(function_id == 1){
        functionName = "SJF";
    }else{
        functionName = "SRT";
    }
    for (int i = 0; i < n; ++i) {
        sum += table[i].turnaroundTime;
    }
    printf("%-4s - %.6f\n", functionName, (float)sum/(float)n);
}

struct process * create_table(int n, int k, int min_time, int max_time){
    //TODO implement simulation


    struct process *table = malloc(sizeof (struct process)*n);

    for (int i = 0; i < n; ++i) {
        // initialize row in table
        struct process *p = &table[i]; // this feels less cumbersome, but I am not sure if this is faster than accessing via [offset].member
        p->pid = i;
        p->turnaroundTime = 0;
        //arrival
        p->arrivalTime =  (int) random_at_most_from_StackOverflow(k);
        p->active = 0;
        // I took this part out because I assume t is considered NaN at this point.
        /*if(p->arrivalTime == 0){
            p->active = 1;
        }else{
            p->active = 0;
        }*/
        p->remainingCPUTime = min_time + (int) random_at_most_from_StackOverflow(max_time - min_time);
        p->totalCPUTime = p->remainingCPUTime;
    }
    // run the simulation until all processes have a remaining time == 0.

    return table;
}

int help(int pArgC, const char* badArg, int badArgPos){
    // print message for call with too few or too many arguments if no string is passed.
    if(badArg == NULL){
        if(pArgC < 0){
            // too few args, print abs(pArgC)
            printf("Too few arguments were provided.\n\tExpected: 4\n\tActual: %d\n", abs(pArgC));
        }else{
            // too many args, print pArgC
            printf("Too many arguments were provided.\n\tExpected: 4\n\tActual: %d\n", pArgC);
        }
        printf("run with --help for more information\n");
    }// check if explicit call to help was made.
    else if(strcmp(badArg, "--help") == 0 || strcmp(badArg, "-h") == 0){
        printf("Help for process scheduling simulator:\nThis program expects four or zero positional arguments when the user is not calling --help or -h for documentation."
               "\nThese arguments, if included, must be integers separated by spaces and are detailed below:\n"
               "\tn        - the number of processes to simulate\n"
               "\tk        - the maximum time step when a process can be created\n"
               "\tmin_time - the minimum time a process needs to run\n"
               "\tmax_time - the maximum time a process needs to run\n"
               "\nUsages:"
               "\n\tschedulingsim [n] [k] [min_time] [max_time]"
               "\n\tschedulingsim\truns with default parameters [n=10, k=100, min_time=7, max_time=15]\n");
    }// print error for bad argument
    else{
        printf("Invalid argument provided.\n\tPosition: %d\n\tValue: %s\n", badArgPos, badArg);
    }
    return -1;
}
