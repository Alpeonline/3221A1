
/**
 * Alp Duras 220603346
 * This file contains the methods for a priority que schedule. The add method inserts the tasks sorted by their priority,
 * the schedule method executres the in order SLL 
 */
#include "task.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

//idea: same logic as shortest job first, but this time we compare their priority number instead of burst time
//insert each task to the SLL in order, so each task is in the right spot after using add

struct node *head= NULL;
static int next_tid = 1; 


//this method inserts the given task to the SLL in the right spot, the method maintaints the order of priority in the SLL
void add(char *name, int priority, int burst){
    //block to create a task with given info, allocate memory in the size of a Task
    Task *t = malloc(sizeof(Task));
    /**
     * Ai assisted section for if statement below
     * Purpose: Debugging Help: Using AI to identify syntax errors or logical issues in your code
     * , i asked for any memory leak issues as I am not familiar with malloc that much
     * 
     * Date: 2026-02-25
     * 
     */
     //if the memory alloc fails exit
    if (t == NULL) {
        return;}
    //alocate space for string +1 for \0
    t->name = malloc(strlen(name) + 1);
        //if the memory alloc fails exit
    if (t->name == NULL) { 
        free(t);
         return; }
    //ai assisted section end

    //fill in the fields for the Task struct
    strcpy(t->name, name);

    t->priority = priority;
    t->burst = burst;
    t->tid = next_tid++;
    //create the node we are going to insert to the SLL
   struct node *newNode = malloc(sizeof(struct node));

    if (newNode == NULL) {
        //if the memory alloc fails exit
        free(t->name);
        free(t);
        return;
    }
    //assign the task to the node
    newNode->task = t;
    newNode->next = NULL;

    //insertion logic for in order insertion to the SLL
    //if the  nodes priority is strictly higher than the head node insert to head
    if(head==NULL || priority > head->task->priority){
        //insert node to head
        newNode->next=head;
        head=newNode;
    }
    //case that the head node is in the right place
    //we need to traverse the sll
    else{
    //the pointer node
    struct node *curr = head;
    //tie breaking using >=, at the end of the loop we find the node we want to insert after
    while(curr->next!= NULL && curr->next->task->priority >= priority){
        curr = curr->next;
    }
    //insert the node at the right spot
    newNode->next = curr->next;
    curr->next=newNode;
}
}
//this method executes the SLL we created above in order
void schedule(){
    //time calculation variables
    int totalWait=0,totalTurnAround=0,totalResponse=0;
    
    int size=0;
    int elapsedTime=0;
    
    //pointer node
    struct node *curr=head;
   //traverse the sll 
    while(curr!=NULL){
        //store the burst for easier calculation
        int thisBurst=curr->task->burst;
        // the wait time is the same response time, turnaround is elapsed + burst time
        totalTurnAround+= elapsedTime+thisBurst;
        totalWait+=elapsedTime;
        totalResponse+=elapsedTime;
        
        //increment time
        elapsedTime+= thisBurst;
        size++;

        
        //call the cpu.c method to show execution of task
        run(curr->task,curr->task->burst);

        //free up allocated memory for nodes and task to avoid memory leak
        free(curr->task->name);
        free(curr->task);
        
        //save current pointer node before sclearing it
        struct node *next = curr->next;
        free(curr);
        curr=next;

    }
    //avoid 0 division
    if(size==0){
        puts("EMPTY SCHEDULE");
        return;
    }
    //print the time calulcations to 2 decimal
    printf("\n");
    printf("Average waiting time = %.2f\nAverage turnaround time = %.2f\nAverage response time = %.2f",
        (double)totalWait /size,(double)totalTurnAround /size,(double)totalResponse/size);
    
}
