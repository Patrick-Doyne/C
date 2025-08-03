
/* This is the only file you will be editing.
 * - egul_sched.c (Egul Scheduler Library Code)
 * - Copyright of Starter Code: Prof. Kevin Andrea, George Mason University. All
Rights Reserved
 * - Copyright of Student Code: You!
 */

/* Standard Library Includes */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
/* Unix System Includes */
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <pthread.h>
#include <sched.h>
/* Local Includes */
#include "egul_sched.h"
#include "strawhat_scheduler.h"
#include "strawhat_support.h"
#include "strawhat_process.h"

/*** Egul Library API Functions to Complete ***/
/* Initializes the Egul_schedule_s Struct and all of the Egul_queue_s Structs
 * Follow the project documentation for this function.
 * Returns a pointer to the new Egul_schedule_s or NULL on any error.
 * - Hint: What does malloc return on an error?
 */
Egul_schedule_s *egul_initialize() {
	//Dynamic Init
	Egul_schedule_s* schedule = malloc(sizeof(Egul_schedule_s));
	schedule->ready_queue = malloc(sizeof(Egul_queue_s));
	schedule->suspended_queue = malloc(sizeof(Egul_queue_s));
	schedule->terminated_queue = malloc(sizeof(Egul_queue_s));
	//Check memory was avilable
	if(schedule == NULL || schedule->ready_queue == NULL || schedule-
			>suspended_queue == NULL || schedule->terminated_queue == NULL){
		return NULL;
	}
	//Set Fields
	schedule->ready_queue->count = 0;
	schedule->ready_queue->head = NULL;
	schedule->ready_queue->tail = NULL;
	schedule->suspended_queue->count = 0;
	schedule->suspended_queue->head = NULL;
	schedule->suspended_queue->tail = NULL;
	schedule->terminated_queue->count = 0;
	schedule->terminated_queue->head = NULL;
	schedule->terminated_queue->tail = NULL;
	return schedule;
}
/* Allocate and Initialize a new Egul_process_s with the given information.
 * - Malloc and copy the command string, don't just assign it!
 * Follow the project documentation for this function.
 * - You may assume all arguments within data are Legal and Correct for this
Function Only
 * Returns a pointer to the Egul_process_s on success or a NULL on any error.
 */
Egul_process_s *egul_create(Egul_create_data_s *data) {
	//Error checking
	if(data == NULL || data->original_cmd == NULL){
		return NULL;
	}
	//Create
	Egul_process_s* new = malloc(sizeof(Egul_process_s));
	if(new == NULL){
		return NULL;
	}
	//Set fields
	new->cmd =malloc(sizeof(data->original_cmd));
	strcpy(new->cmd , data->original_cmd);
	new->priority = data->priority_level;
	new->pid = data->pid;
	//Set mask
	unsigned short critical_ready_mask = 3<<14;
	unsigned short ready_mask = 1 << 14;
	if(data->is_critical==0){
		new->state = new->state | ready_mask;
	}
	else{
		new->state = new->state | critical_ready_mask;
	}
	//Set fields
	new->age = 0;
	new->next = NULL;
	return new;
}
/* Inserts a process into the Ready Queue (singly linked list).
 * Follow the project documentation for this function.
 * - Do not create a new process to insert, insert the SAME process passed in.
 * Returns a 0 on success or a -1 on any error.
 */
int egul_insert(Egul_schedule_s *schedule, Egul_process_s *process) {
	//Error check
	if(schedule == NULL || process == NULL || schedule->ready_queue == NULL){
		return -1;
	}
	//Set mask
	unsigned short mask = 15<<11;
	process->state = process->state & ~(mask);
	process->state = process->state | 1<<14;
	//NULL head insert
	if(schedule->ready_queue->head == NULL){
		schedule->ready_queue->head = process;
		schedule->ready_queue->count++;
		return 0;
	}
	//Insert
	Egul_process_s *trace = schedule->ready_queue->head;
	while(trace->next!=NULL){
		trace = trace->next;
	}
	trace->next = process;
	schedule->ready_queue->count++;
	return 0;
}
/* Returns the number of items in a given Egul Queue (singly linked list).
 * Follow the project documentation for this function.
 * Returns the number of processes in the list or -1 on any errors.
 */
int egul_count(Egul_queue_s *queue) {
	//Error check
	if(queue == NULL){
		return -1;
	}
	return queue->count;
}
/* Selects the best process to run from the Ready Queue (singly linked list).
 * Follow the project documentation for this function.
 * Returns a pointer to the process selected or NULL if none available or on any
errors.
 * - Do not create a new process to return, return a pointer to the SAME process
selected.
 * - Return NULL if the ready queue was empty OR there were any errors.
 */
Egul_process_s *egul_select(Egul_schedule_s *schedule) {
	//Error check
	if(schedule->ready_queue->head == NULL || schedule == NULL || schedule-
			>ready_queue == NULL){
		return NULL;
	}
	//Pointers
	Egul_process_s *pointer = schedule->ready_queue->head;
	Egul_process_s *priority_pointer = schedule->ready_queue->head;
	//Mask vars
	unsigned short critical_mask = 1 << 15;
	unsigned short ready_mask = 1 << 14;
	unsigned short running_mask = 1 << 13;
	//If else control
	char found = '0';
	//Select
	while(pointer != NULL){
		if(found =='1'){
			pointer->age++;
			pointer = pointer->next;
			continue;
		}
		else if((pointer->state & critical_mask) != 0){
			priority_pointer = pointer;
			found = '1';
		}
		else if(pointer->age >= STARVING_AGE && found == '0'){
			priority_pointer = pointer;
			found = '2';
		}
		else if(pointer->priority < priority_pointer->priority && found =='0'){
			priority_pointer = pointer;
		}
		pointer->age++;
		if(pointer->next!=NULL){
			pointer = pointer->next;
			continue;
		}
		break;
	}
	//Update selected process
	priority_pointer->state = priority_pointer->state & ~ready_mask;
	priority_pointer->state = priority_pointer->state | running_mask;
	priority_pointer->age = 0;
	//Remove from ready
	pointer = schedule->ready_queue->head;
	if(pointer == priority_pointer){
		schedule->ready_queue->head = pointer->next;
	}
	while(pointer->next != NULL){
		if(pointer->next == priority_pointer){
			pointer->next = pointer->next->next;
			break;
		}
		pointer = pointer->next;
	}
	priority_pointer->next = NULL;
	schedule->ready_queue->count--;
	return priority_pointer;
}
/* Move the process with matching pid from Ready to Suspended Queue.
 * Follow the specification for this function.
 * Returns a 0 on success or a -1 on any error (such as process not found).
 */
int egul_suspend(Egul_schedule_s *schedule, pid_t pid) {
	//Error check
	if(schedule == NULL || schedule->ready_queue == NULL || schedule-
			>suspended_queue == NULL || schedule->ready_queue->head == NULL){
		return -1;
	}
	//Pointers
	Egul_process_s *pointer_ready = schedule->ready_queue->head;
	Egul_process_s *pointer_susp = schedule->suspended_queue->head;
	Egul_process_s *found = NULL;
	//Remove from ready queue
	if(pointer_ready == NULL){//check null case
		return -1;
	}
	if(pid == pointer_ready->pid||pid == 0){//check first process and pid = zero
	case
	schedule->ready_queue->head = schedule->ready_queue->head->next;
	schedule->ready_queue->count--;
	found = pointer_ready;
	}
	else{//check all other processes
		while(pointer_ready->next!=NULL){
			if(pointer_ready->next->pid == pid){//found target process
				found = pointer_ready->next;
				pointer_ready->next = pointer_ready->next->next;//remove
				from ready queue
				found->next = NULL;
				schedule->ready_queue->count--;
				break;
			}
			pointer_ready = pointer_ready->next;
		}
		if(pointer_ready->next == NULL && pointer_ready->pid == pid){
			found = pointer_ready;
		}
	}
	//Return if not found
	if(found == NULL){
		return -1;
	}
	found->next = NULL;
	//Set state
	unsigned short ready_mask = 1<<14;
	unsigned short susp_mask = 1<<12;
	found->state = found->state & ~(ready_mask);
	found->state = found->state | susp_mask;
	//Add to end of suspended queue
	if(schedule->suspended_queue->head ==NULL){//NULL case
		schedule->suspended_queue->head = found;
	}
	else{//all other cases
		while(pointer_susp->next!=NULL){
			pointer_susp = pointer_susp->next;
		}
		pointer_susp->next = found;//add target process at the end of suspended
		queue
	}
	schedule->suspended_queue->count++;
	return 0;
}
/* Move the process with matching pid from Suspended to Ready Queue.
 * Follow the specification for this function.
 * Returns a 0 on success or a -1 on any error (such as process not found).
 */
int egul_resume(Egul_schedule_s *schedule, pid_t pid) {
	//Error check
	if(schedule==NULL || schedule->suspended_queue == NULL || schedule-
			>ready_queue == NULL || schedule->suspended_queue->head == NULL){
		return -1;
	}
	//Pointers
	Egul_process_s *resume = schedule->suspended_queue->head;
	Egul_process_s *found = NULL;
	Egul_process_s *ready = schedule->ready_queue->head;
	//Head or PID == 0 case
	if(resume->pid == pid || pid == 0){
		found = resume;
		schedule->suspended_queue->head = schedule->suspended_queue->head-
				>next;
	}
	else{//all other cases
		while(resume->next != NULL){
			if(resume->next->pid == pid){
				found = resume->next;
				resume->next = resume->next->next;
				break;
			}
			resume = resume->next;
		}
	}
	//Return if not found
	if(found==NULL){
		return -1;
	}
	found->next = NULL;
	schedule->suspended_queue->count--;
	found->state = found->state & ~(1<<12);
	found->state = found->state | (1<<14);
	//Empty ready queue
	if(ready == NULL){
		schedule->ready_queue->head = found;
	}
	else{//all other cases
		while(ready->next!=NULL){
			ready = ready->next;
		}
		ready->next = found;
	}
	schedule->ready_queue->count++;
	return 0;
}
/* This is called when a process exits normally that was just Running.
 * Put the given node into the Terminated Queue and set the Exit Code
 * - Do not create a new process to insert, insert the SAME process passed in.
 * Follow the project documentation for this function.
 * Returns a 0 on success or a -1 on any error.
 */
int egul_exited(Egul_schedule_s *schedule, Egul_process_s *process, int exit_code)
{
	//Error check
	if(process == NULL||schedule == NULL||schedule->terminated_queue == NULL){
		return -1;
	}
	//Clear old mask
	unsigned short clear_state_mask= 31 << 11;
	unsigned short terminated_mask = 1 << 11;
	process->state = process->state & ~clear_state_mask;
	process->state = process->state | terminated_mask;
	//Set new mask
	unsigned short clear_old_ec = 2047;
	unsigned short ec = (unsigned short)exit_code;
	process->state =process->state & ~clear_old_ec;
	process->state = process->state | ec;
	//NULL head
	if(schedule->terminated_queue->head == NULL){
		schedule->terminated_queue->head = process;
		schedule->terminated_queue->count++;
		return 0;
	}
	//All other cases
	Egul_process_s *terminated_pointer = schedule->terminated_queue->head;
	while(terminated_pointer->next!=NULL){
		terminated_pointer = terminated_pointer->next;
	}
	terminated_pointer->next = process;
	schedule->terminated_queue->count++;
	return 0;
}
/* This is called when the OS terminates a process early.
 * - This will either be in your Ready or Suspended Queue.
 * - The difference with egul_exited is that this process is in one of your Queues
already.
 * Remove the process with matching pid from the Ready or Suspended Queue and add
the Exit Code to it.
 * - You have to check both since it could be in either queue.
 * Follow the project documentation for this function.
 * Returns a 0 on success or a -1 on any error.
 */
int egul_killed(Egul_schedule_s *schedule, pid_t pid, int exit_code) {
	//Error check
	if(schedule == NULL || schedule->ready_queue == NULL || schedule-
			>suspended_queue == NULL || schedule->terminated_queue == NULL){
		return -1;
	}
	//Pointers
	Egul_process_s *found = NULL;
	Egul_process_s *ready = schedule->ready_queue->head;
	Egul_process_s *susp = schedule->suspended_queue->head;
	//Check ready queue
	if(ready!=NULL){
		if(ready->pid == pid){
			found = ready;
			schedule->ready_queue->head = schedule->ready_queue->head->next;
		}
		else{
			while(ready->next!=NULL){
				if(ready->next->pid == pid){
					found = ready->next;
					ready->next = ready->next->next;
					break;
				}
				if(found==NULL){
					ready = ready->next;
				}
			}
		}
		if(found!=NULL){
			schedule->ready_queue->count--;
		}
	}
	//If not in ready check suspended
	if(found == NULL){
		if(susp!=NULL){
			if(susp->pid == pid){
				found = susp;
				schedule->suspended_queue->head = schedule-
						>suspended_queue->head->next;
			}
			else{
				while(susp->next!=NULL){
					if(susp->next->pid == pid){
						found = susp->next;
						susp->next = susp->next->next;
						break;
					}
					if(found==NULL){
						susp = susp->next;
					}
				}
			}
			if(found!=NULL){
				schedule->suspended_queue->count--;
			}
		}
	}
	//If not found return
	if(found==NULL){
		return -1;
	}
	found->next = NULL;
	//Get ec
	int exit = egul_exited(schedule, found, exit_code);
	if(exit==0){
		return 0;
	}
	//Ec fail return
	return -1;
}
/* Gets the exit code from a terminated process.
 * (All Linux exit codes are between 0 and 255)
 * Follow the project documentation for this function.
 * Returns the exit code if the process is terminated.
 * If the process is not terminated, return -1.
 */
int egul_get_ec(Egul_process_s *process) {
	//Error check
	if(process == NULL || process->state != (process->state | (1<<11))){
		return -1;
	}
	//Get ec
	unsigned short ec = process->state & ~(11111<<11);
	int exit_code = (int)ec;
	return exit_code;
}
/* Frees all allocated memory in the Egul_schedule_s, all of the Queues, and all of
their Nodes.
 * Follow the project documentation for this function.
 * Returns void.
 */
void egul_deallocate(Egul_schedule_s *schedule) {
	//Error check
	if(schedule== NULL){
		return;
	}
	Egul_process_s *pointer1 = NULL;
	//Free ready queue
	if(schedule->ready_queue!=NULL && schedule->ready_queue->head == NULL){
		free(schedule->ready_queue->head);
	}
	if(schedule->ready_queue!=NULL && schedule->ready_queue->head!=NULL){
		pointer1 = schedule->ready_queue->head->next;
	}
	while(schedule->ready_queue!=NULL && schedule->ready_queue->head!=NULL){
		free(schedule->ready_queue->head->cmd);
		schedule->ready_queue->head->cmd = NULL;
		free(schedule->ready_queue->head);
		schedule->ready_queue->head = pointer1;
		if(pointer1!= NULL){
			pointer1 = pointer1->next;
		}
	}
	free(schedule->ready_queue->head);
	free(schedule->ready_queue);
	schedule->ready_queue = NULL;
	//Free suspended queue
	Egul_process_s *pointer2 = NULL;
	if(schedule->suspended_queue!=NULL && schedule->suspended_queue->head ==
			NULL){
		free(schedule->suspended_queue->head);
	}
	if(schedule->suspended_queue!=NULL && schedule->suspended_queue->head!=NULL){
		pointer2 = schedule->suspended_queue->head->next;
	}
	while(schedule->suspended_queue!=NULL && schedule->suspended_queue->head!
			=NULL){
		free(schedule->suspended_queue->head->cmd);
		schedule->suspended_queue->head->cmd = NULL;
		free(schedule->suspended_queue->head);
		schedule->suspended_queue->head = pointer2;
		if(pointer2!= NULL){
			pointer2 = pointer2->next;
		}
	}
	free(schedule->suspended_queue->head);
	free(schedule->suspended_queue);
	schedule->suspended_queue = NULL;
	//Free terminated queue
	Egul_process_s *pointer3 = NULL;
	if(schedule->terminated_queue!=NULL && schedule->terminated_queue->head ==
			NULL){
		free(schedule->terminated_queue->head);
	}
	if(schedule->terminated_queue!=NULL && schedule->terminated_queue->head!
			=NULL){
		pointer3 = schedule->terminated_queue->head->next;
	}
	while(schedule->terminated_queue!=NULL && schedule->terminated_queue->head!
			=NULL){
		free(schedule->terminated_queue->head->cmd);
		schedule->terminated_queue->head->cmd = NULL;
		free(schedule->terminated_queue->head);
		schedule->terminated_queue->head = pointer3;
		if(pointer3!= NULL){
			pointer3 = pointer3->next;
		}
	}
	free(schedule->terminated_queue->head);
	free(schedule->terminated_queue);
	//Free schedule
	free(schedule);
	schedule = NULL;
	return;
}
