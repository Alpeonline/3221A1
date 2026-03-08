/**
 * Representation of a task in the system.
 * 
 * Alp Duras 220603346
 * 
 * I added 2 fields to the task struct,
 * OriginalBurst: used for time calculations, makes it easier and cheaper for calculations later on
 * isFirstRun: =1 if the task is going to be ran for the first time 0 otherwise
 */

#ifndef TASK_H
#define TASK_H

// representation of a task, i added two fields, original burst, and is run
typedef struct task {
    char *name;
    int tid;
    int priority;
    int burst;
    int originalBurst;
    //if 1 then its the first run, 0 otherwise
    int isFirstRun;
} Task;

#endif
