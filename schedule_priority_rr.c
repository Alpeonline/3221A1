/**
 * Alp Duras 220603346
 * This file contains the methods for a round priority robin schedule. The add method inserts the nodes in the right spot by their priority order
 * (higher better)
 * the schedule method handles the execution of  the in order SLL, the q value is taken from cpu.h
 * see the methods for detailed comments
 */
#include "task.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"
#include "schedulers.h"

//head tail
static struct node *head = NULL;   
static struct node *tail = NULL;
static int next_tid = 1;

//the idea is to use the sll insertion method from  priority scheduling. Then Handle the execution in the schedule method 
//this method inserts a given task to the SLL and keeps it in order by its priority, where higher priority is better
void add(char *name, int priority, int burst){
   
   //block to create a task with given info, allocate memory in the size of a Task
    Task *t = malloc(sizeof(Task));
    /**
     * Ai assisted section for if statement below
     * Purpose: Debugging Help: Using AI to identify syntax errors or logical issues in your code, i asked for any memory leak issues as I am not familiar with malloc that much
     * 
     * Date: 2026-02-25
     * 
     */
    //if the memory alloc fails exit
    if (t == NULL) {
        return;
    }

    //alocate space for string +1 for \0
    t->name = malloc(strlen(name) + 1);
     //if the memory alloc fails exit
    if (t->name == NULL) {
         free(t);
          return;
         }
    //ai assistance section end

    //fill in the fields for the Task struct
    strcpy(t->name, name);

    t->priority = priority;
    t->burst = burst;
    t->tid = next_tid++;
    //fields added by me
    t->isFirstRun=1;
    t->originalBurst=burst;

      
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
    //if the SLL is empty the tail and head is the  same node
    if(head==NULL){
        head=newNode;
        tail=newNode; 
        return;
    }

    //if this current nodes priority is strictly bigger than the head, insert to head
    if(priority > head->task->priority){
        tail=head;
        newNode->next=head;
        head=newNode;
        
    }
    //the case that the insertion is somwhere else, we have to iterate the SLL to find the right spot
    else{
    //pointer
    struct node *curr = head;
    //moves the pointer until we find the right spot to insert
    while(curr->next!= NULL && curr->next->task->priority >= priority){
        curr = curr->next;
    }
    //insert the node at the right spot to keep the SLL in order
    newNode->next = curr->next;
    curr->next=newNode;

    //if the next node after the node we insert is null, then it must be the tail
    if(newNode->next==NULL){
        tail=newNode;
    }
}
}

//this method handles the execution of the SLL which is in order, if tasks have the same priority it executes that group of priority first before moving to different priorities
//the logic and idea in this method is to each time we iterate, if there is a node with same priority after the node we pop the node and insert at the end of the same priroiryt group
//if there no is no same priority pop the head and insert at the end of Sll unless burst=0 (just like in the normal rr scheduler)
void schedule(){
    
    //time calulcation variables
    int totalWait=0,totalTurnAround=0,totalResponse=0;
    
    int size=0;
    int elapsedTime=0;
    
   //traverse the sll
    //the loop runs until the SLL is empty
    while(head!=NULL){
        //pointer node
         struct node *curr=head;
         //time slice which the current task will run for
         int timeSlice =0;
         //this block handles the first respoonse calculation, if its the first time this task is runing add to thesu
         if(curr->task->isFirstRun==1){
            totalResponse+= elapsedTime;
            //flip the firstrun flag
            curr->task->isFirstRun=0;
        }
        //this block calculates the time slice,  if burst<q then slice =burst, otherwise q and if there is no same priority task in the list
        if(curr->task->burst < QUANTUM|| curr->next==NULL|| (curr->next->task->priority!= curr->task->priority)){
            timeSlice= curr->task->burst;
            
        }
        else{
            timeSlice=QUANTUM;
            
        }

        //increment elapsed time by the time slice
        elapsedTime+=timeSlice;
        //execute the task with the time slice
        run(curr->task,timeSlice);
        //subtract the time slice from the time burst the task has
        curr->task->burst-=timeSlice;

        //if the task is finished treat as same as normal round robin, delete from the SLL
        if(curr->task->burst==0){ //dont use delete from list.c its o(N) since were deleting head we can do it in o(1)
            //move the head
            head=head->next;
            //size calculation by incerementing at every node deletion
            size++;
            //if there was only 1 node before deletion sets tail to null
            if(curr==tail){
                tail=NULL;
            }
            //the turnaround time is the time task fiinshed calculation
            totalTurnAround += elapsedTime;
            //use the formula turnaround time - task execution time, to find wait time
            totalWait+= elapsedTime - (curr->task->originalBurst);
            
            //free memory from the node and task
            free(curr->task->name);
            free(curr->task);
            free(curr);
            
            
        }
        //the case that the task is longer than q, we have to take in to account the node we are popping might have to be inserted after a priority group not to the end of the sll
        else{
            if(head==tail){
                //do nothing if there is 1 node in the sll
            }
            else{
                //to node were going to pop from the head
               struct node *toInsert = head;
               //move the head to the next node
                head = head->next;
                //detach the next of the old head
                toInsert->next = NULL;

                //checks if the next task in the SLL has the same priority as the head
                //if there is, we need to traverse the SLl so that we can insert at the right priority group 
                if (head != NULL && head->task->priority == toInsert->task->priority) {
                    //pointer node
                    struct node *pointer = head;
                    //increment the pointer until the next node is not of the same priority
                    while (pointer->next != NULL && pointer->next->task->priority == toInsert->task->priority) {
                        pointer = pointer->next;
                    }
                    //inserts the node after the spot where node which has the same priority ends
                    toInsert->next = pointer->next;
                    pointer->next = toInsert;
                    //if the next node after inserting is null, then the inserted node must be the tail
                    if (toInsert->next == NULL) {
                        tail = toInsert;
                    }
                    
                }
                //if the next task is not of the same priority, we just pop the head and insert at the end of SLL
                else{
                    //insert after the tail
                    tail->next = toInsert;
                     tail = toInsert; 
                     tail->next = NULL;
                }
            }
        }

        
    }
    //stop divison by 0
    if(size==0){
        puts("EMPTY SCHEDULE");
        return;
    }
    printf("\n");
    //print the time calculation in 2 decimal
    printf("Average waiting time = %.2f\nAverage turnaround time = %.2f\nAverage response time = %.2f",
        (double)totalWait /size,(double)totalTurnAround /size,(double)totalResponse/size);


}