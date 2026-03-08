/**
 * Alp Duras 220603346
 * 
 * This file contains the methods for scheduling a first come first serve scheduler. The add method inserts the tasks in a FCFS order, 
 *  schedule executes the SLL
 * 
 * AI assistance statement: (tool used chatgpt)
 * Date: 2026-02-25
 * purposes(Explaining Concepts:): Show me how Singlely linked list works, how to use -> operator with structs, how is a SLL structured in c,how to create a struct SLL using malloc
 * My Modifications: no generated code was used from AI, the sole use was to get started on the implementation of SLL and structure attributes in c
 * Ai assistance statement end
 */
#include "task.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"
#include "schedulers.h"

//keep track of head and tail
static struct node *head = NULL;   
static struct node *tail = NULL;
static int next_tid = 1;


//The idea when starting the implementation of this method was: Create a sll, everytime a new task is added, insert the task to the end of the SLL
//i didnt use the method from list.c as that method appends to the head not tail

//This method takes in the name, priority and burst of a task. Creates a task struct and adds it to the end of the SLL
//The purpose of this method is to create a ordered SLL to execute
void add(char *name, int priority, int burst) {
    
    //block tocreate a task with given info, allocate memory in the size of a Task
    Task *t = malloc(sizeof(Task));
    /**
     * Ai assisted section for if statement below
     * Purpose: Debugging Help: Using AI to identify syntax errors or logical issues in your code, 
     * i asked for any memory leak issues as I am not familiar with malloc that much
     * 
     * Date: 2026-02-25
     * 
     */
    //if malloc fails ,exit, to stop memoryt leaks
    if (t == NULL) {
        return;}
    
    //allocate memory in the memory adress for the name field in task, (+1 for \0)
    t->name = malloc(strlen(name) + 1);
   
    
    //if the malloc fails, exit without leaking memory
    if (t->name == NULL) 
    { 
        free(t); return; 
    }
    //ai assisted section end

    //copy the passed string to name of task
    strcpy(t->name, name);
    //fill the task fields, note; we do not need to assign isFirstRun and oriingalBurst as we wont use them
    t->priority = priority;
    t->burst = burst;
    t->tid = next_tid++;
    
    //code block for insertion to end logic to the SLL
    //use malloc to allocate memoery for the newNode were going to insert
    struct node *newNode = malloc(sizeof(struct node));
      //if the malloc fails exit without leaking memory
    if (newNode == NULL) {
        free(t->name);
        free(t);
        return;
    }
    //copy the task to this newNode, 
    newNode->task = t;
    newNode->next = NULL;
    
    //if the sll is empty, then this node is both the head and tail
    if (head == NULL) {
        head = newNode;
        tail = newNode;

    }
    //normal insertion to the tail 
    else {
        //the old tails next is the new node, and the new tail is the new node
        tail->next = newNode;
        tail = newNode;
    }
}

//this method handles the execution the tasks in normal order as the SLL is sorted
void schedule(){
    //time calculation variables
    int totalWait=0,totalTurnAround=0,totalResponse=0;
    
    int size=0;
    int elapsedTime=0;
    
    //this node will act as a pointer when were traversing the sll
    struct node *curr=head;
    //traverse until we reach end
    while(curr!=NULL){
        //store the current burst time
        int thisBurst=curr->task->burst;

        //the turn around time is the current time+ the burst time of the task
        totalTurnAround+= elapsedTime+thisBurst;
        
        //wait time and response is same
        totalWait+=elapsedTime;
        totalResponse+=elapsedTime;
        
        //use the run method from cpu to execute the current process fully
        run(curr->task,curr->task->burst);
        
        //increment the time and size
        elapsedTime+= thisBurst;
        size++;

        //free up the allocated memory for nodes and task to avoid memory leak after execution
        free(curr->task->name);
        free(curr->task);
        
        //save current pointer node before clearing it for traversing
        struct node *next = curr->next;
        free(curr);
        curr=next;
       
    }
    
    //avoid 0 division
    if(size==0){
        puts("EMPTY SCHEDULE");
        return;
    }
    printf("\n");
    //print to 2 decimal place
    printf("Average waiting time = %.2f\nAverage turnaround time = %.2f\nAverage response time = %.2f",
        (double)totalWait /size,(double)totalTurnAround /size,(double)totalResponse/size);
    
}

