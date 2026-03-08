/**
 * Alp Duras 220603346
 * This file contains the methods for a round robin schedule. The add method inserts the nodes in the passed order and schsedule executes the SLL
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
//head and tail variables (we will need tail this time)
static struct node *head = NULL;   
static struct node *tail = NULL;
static int next_tid = 1;

//the major strugle in this scheduler was the time calculation, we need to keep track if the task is running for the first time,
//this made me edit the structure for TASK to add new fields


//the idea is the same as FCFS, we insert to the back of an SLL, we will handle the quantum in schedule()
//the method inserts the nodes to the SLL in a FCFS order, the execution order is handled in schedule()
void add(char *name, int priority, int burst) {
     //block to create a task with given info, allocate memory in the size of a Task
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
    if (t == NULL){ 
        return;}
    //alocate space for string +1 for \0
    t->name = malloc(strlen(name) + 1);
     //if the memory alloc fails exit
    if (t->name == NULL) 
    { 
    free(t);
    return; 
    }
    //ai assisted section end

    //fill in the fields for the Task struct
    strcpy(t->name, name);

    t->priority = priority;
    t->burst = burst;
    t->tid = next_tid++;
    //attention here 2 new fields are used
    t->originalBurst = burst;
    t->isFirstRun=1;

    //insertion to SLL logic

     //create the node we are going to insert to the SLL
    struct node *newNode = malloc(sizeof(struct node));
    //if the memory alloc fails exit
    if (newNode == NULL) {
        free(t->name);
        free(t);
        return;
    }
    //set the task to the node
    newNode->task = t;
    newNode->next = NULL;
    //if the SLL is empty the tail and the head is the node
    if (head == NULL) {
        head = newNode;
        tail = newNode;
    }
    //SLL is not empty insert to the end 
    else {
        tail->next = newNode;
        tail = newNode;
    }
}

//this method handles the round robin execution of the SLL, with the given q value. It is more complex compared to FCFS,SJF and priority scheduler
//the idea and the logic in this method is to if the q< the burst time, then pop the head and insert it to the back of the SLL
void schedule(){
    //time calculation variables
    int totalWait=0,totalTurnAround=0,totalResponse=0;

    int size=0;
    int elapsedTime=0;
    
   //traverse the SLL
   //pay attention, we run the loop until the whole SLL is empty
    while(head!=NULL){
        //pointer 
         struct node *curr=head;
         //the time slice that the exeuction actually runs for 
        int timeSlice =0;
        
        //response time calculation logic, if this is the first time the task is getting run add to the response time
        if(curr->task->isFirstRun==1){
            
            totalResponse+= elapsedTime;
            //switch the attribute as its been run
            curr->task->isFirstRun=0;
        }
        //we find the slice we will run the task for, decrease this slice from the burst. after running, check if burst is 0, if 0 delete the node from sll
        //if the burst less than q then the time slice is the burst
        if(curr->task->burst < QUANTUM){
            timeSlice= curr->task->burst;
            
        }
        else{
            //set the time slice to q otherwise
            timeSlice=QUANTUM;
            
        }
        //increase the elapsed time by the timeslice
        elapsedTime+=timeSlice;
        
        //run the current task for the calculated time slice
        run(curr->task,timeSlice);
        //decrease the burst time from the task
        curr->task->burst-=timeSlice;
        
        //if the burst in the head task is 0, then the task is done and  we need to delete the node from the SLL
        if(curr->task->burst==0){ //dont use delete from list.c its o(N) since were deleting head we can do it in o(1)
            //move the head to the next
            head=head->next;
            //every deletion increments size
            size++;
            //if there was only 1 node in the sll set the tail to null
            if(curr==tail){
                tail=NULL;
            }
            //the turn around time is the elapsed time for the task at the end of the exeuction
            totalTurnAround += elapsedTime;
            //uses the formula elapsedTime - original burst time to calculate the wait time
            totalWait+= elapsedTime - (curr->task->originalBurst);
            
            //frees the memoery for task and node
            free(curr->task->name);
            free(curr->task);
            free(curr);
            
            
        }
        //this is the case that burst time> q
        //if the task has remaining time left, move  the node to  tail
        else{
            //if there is only one node in the sll dont do anything
            if(head==tail){
                
            }
            else{

            //this pops the head node and inserts the node at the end of the sll, so puts it to the end of the queue
             head=head->next;
            tail->next=curr;
            tail=curr;
            tail->next=NULL;
            }
           
            

        }
     
        
    }
    //avoid 0 division
    if(size==0){
        puts("EMPTY SCHEDULE");
        return;
    }
    printf("\n");
    //print the time calculations to 2 decimal
    printf("Average waiting time = %.2f\nAverage turnaround time = %.2f\nAverage response time = %.2f",
        (double)totalWait/size,(double)totalTurnAround /size,(double)totalResponse/size);
    
}
