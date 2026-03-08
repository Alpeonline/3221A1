/**
 * Alp Duras 220603346
 * This file contains the methods for a shortest job first schedule. The add method stores the tasks sorted by their burst time in a sll,
 * the schedule method executres the in order SLL 
 */
#include "task.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

//we wont need a tail in this as we have to traverse to insert in to the list

struct node *head= NULL;
static int next_tid = 1; 

 //the idea: on each add call, insert by scanning the list, insert the task at the right spot by comparing the burst time
//by doing this we will always have a SLL that is ready to execute
//tie breaking ; earlier call gets earlier spot

//this method inserts the given task to the SLL, the task is inserted by comparing the burst time of the tasks in the list
void add(char *name, int priority, int burst){
    
    //block create a task with given info, allocate memory in the size of a Task
    Task *t = malloc(sizeof(Task));
    /**
     * Ai assisted section for if statement below
     * Purpose: Debugging Help: Using AI to identify syntax errors or logical issues in your code, 
     * i asked for any memory leak issues as I am not familiar with malloc that much
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
        free(t)
        ; return; }
    //AI assisted section done

    //fill the fields for struct (we dont need the 2 attributes we created here)
    strcpy(t->name, name);
    
 
    t->priority = priority;
    t->burst = burst;
    t->tid = next_tid++;

    //create the node we are going to insert to the SLL
   struct node *newNode = malloc(sizeof(struct node));
   //if the memory alloc fails exit
    if (newNode == NULL) {
        free(t->name);
        free(t);
        return;
    }
    //assign the task to the new node
    newNode->task = t;
    newNode->next = NULL;

    //insertion logic for in order insertion to the SLL
    //if the current nodes burst time is strictly shorter than the head, insert to head
    if(head==NULL || burst < head->task->burst){
        //insert the node to the head and shift the sll 
        newNode->next=head;
        head=newNode;
    }
    //otherwise we have to figure out where we need to insert the node
    else{
    //pointer
    struct node *curr = head;
    //tie breaking using <=, at the end of the loop we find the node we want to insert after
    while(curr->next!= NULL && curr->next->task->burst <= burst){
        //move to next node
        curr = curr->next;
    }
    //insert the node at the right spot
    newNode->next = curr->next;
    curr->next=newNode;
}

}
//this method executes the sll in order, the sll is already in order 
void schedule(){
    //time calculation variables
    int totalWait=0,totalTurnAround=0,totalResponse=0;
    int size=0;
    int elapsedTime=0;
    
    //pointer 
    struct node *curr=head;
   //traverse the sll
    while(curr!=NULL){
        //store the burst for easier calculation
        int thisBurst=curr->task->burst;

        //the wait time and response is the same, turnaround = execution time
        totalTurnAround+= elapsedTime+thisBurst;
        totalWait+=elapsedTime;
        totalResponse+=elapsedTime;
        
        //incremenet time 
        elapsedTime+= thisBurst;
        size++;

        
        //call the cpu.c method to run the current task 
        run(curr->task,curr->task->burst);

        //free up allocated memory for nodes and task to avoid memory leak after execution
        free(curr->task->name);
        free(curr->task);

        //save current pointer node before clearing it
        struct node *next = curr->next;
        free(curr);
        curr=next;

    }
    //avoid 0 division
    if(size==0){
        puts("EMPTY SCHEDULE");
        return;
    }
    //print the time calculation to 2 decimal
    printf("\n");
    printf("Average waiting time = %.2f\nAverage turnaround time = %.2f\nAverage response time = %.2f",
        (double)totalWait /size,(double)totalTurnAround /size,(double)totalResponse/size);
    
}
