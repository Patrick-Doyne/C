
/* This is the only file you will be editing and submit.
 * - Copyright of Starter Code: Prof. Yutao Zhong, George Mason University. All
Rights Reserved
 * - Copyright of Student Code: You!
 */
 
#include <sys/wait.h>
#include "zaku.h"
#include "parse.h"
#include "util.h"
#include "logging.h"
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
//TASK LIST STRUCT
typedef struct taskStruct{
	int taskNumber;
	pid_t PID;
	int exitCode;
	int state;
	int fg_bg; // 0 for fg and 1 for bg
	char *cmd;
	char *args[MAXARGS+1];
	struct taskStruct *next;
} Task;
/* Constants */
#define DEBUG 0 /* You can set this to 0 to turn off the debug parse information */
#define STOP_SHELL 0
#define RUN_SHELL 1
#define NUM_PATHS 2
#define NUM_INSTRUCTIONS 13
//GLOBALS
static const char *instructions[] = { "quit", "help", "list", "delete", "start_fg",
		"start_bg", "kill", "suspend", "resume_fg", "resume_bg", "fg", "pipe", "chain",
		NULL};
Task *taskList;
struct sigaction sa;
struct sigaction sa2;
int taskListSize = 0;
//RETURN A TASK GIVEN ITS PID NUMBER
Task* getTaskFromPid(Task* taskList, pid_t PID){
	if(taskList == NULL) return NULL;;
	Task *walk = taskList;
	while(walk!=NULL){
		if(walk->PID == PID) return walk;
		walk = walk->next;
	}
	return NULL;
}
//RETURN A THE RUNNING FG TASK IF IT EXISTS
Task* getRunningFG(Task* taskList){
	if(taskList == NULL) return NULL;
	Task *walk = taskList;
	while(walk!=NULL){
		if(walk->state == LOG_STATE_RUNNING && walk->fg_bg == 0) return walk;
		walk = walk->next;
	}
	return NULL;
}
//CUSTOM SIG HANDLER
void handler(int sig){
	Task *currentFG = getRunningFG(taskList);
	//FG SIGNAL HANDLING
	if(currentFG!= NULL){
		//CTRL-C FOR FG
		if(sig == SIGINT){
			sa.sa_handler = SIG_IGN;
			sigaction(SIGCHLD, &sa, NULL);
			log_ctrl_c();
			currentFG->state = LOG_STATE_KILLED;
			kill(currentFG->PID,SIGINT);
			log_status_change(currentFG->taskNumber, currentFG->PID,
					currentFG->fg_bg, currentFG->cmd, LOG_TERM_SIG);
		}
		//CTRL-Z FOR FG
		if(sig == SIGTSTP){
			sa.sa_handler = SIG_IGN;
			sigaction(SIGCHLD, &sa, NULL);
			log_ctrl_z();
			currentFG->state = LOG_STATE_SUSPENDED;
			kill(currentFG->PID, SIGTSTP);
			log_status_change(currentFG->taskNumber, currentFG->PID,
					currentFG->fg_bg, currentFG->cmd, LOG_SUSPEND);
		}
	}
	//BG SIGNAL HANDLING
	else{
		int taskStatus = 0;
		pid_t currentPID = waitpid(-1,&taskStatus , WNOHANG|WUNTRACED|
				WCONTINUED);
		Task *current = getTaskFromPid(taskList,currentPID);
		//SAINTY CHECKS
		if(current == NULL) return;
		if(current->fg_bg == 0){
			return;
		}
		//CHANGES IN CHILD
		if(sig == SIGCHLD){
			if(WIFSTOPPED(taskStatus)){
				current->state = LOG_STATE_SUSPENDED;
				log_sig_sent(LOG_CMD_SUSPEND, current->taskNumber, current-
						>PID);
				log_status_change(current->taskNumber, current->PID,
						current->fg_bg, current->cmd, LOG_SUSPEND);
			}
			//RESUMED
			if(WIFCONTINUED(taskStatus) && current->state
					==LOG_STATE_SUSPENDED){
				current->state = LOG_STATE_RUNNING;
				log_sig_sent(LOG_CMD_RESUME,current->taskNumber, current-
						>PID);
				log_status_change(current->taskNumber, current->PID,
						current->fg_bg, current->cmd, LOG_RESUME);
			}
			//KILLED
			if(WIFSIGNALED(taskStatus) && current->state!= LOG_STATE_FINISHED
					&& current->state != LOG_STATE_KILLED && current->state !=LOG_STATE_SUSPENDED){
				current->exitCode = WEXITSTATUS(taskStatus);
				current->state = LOG_STATE_KILLED;
				log_sig_sent(LOG_CMD_KILL, current->taskNumber, current-
						>PID);
				log_status_change(current->taskNumber, current->PID,
						current->fg_bg, current->cmd, LOG_TERM_SIG);
			}
			//EXITED
			if(WIFEXITED(taskStatus) && currentFG == NULL && current->state!=
					LOG_STATE_FINISHED && current->state != LOG_STATE_KILLED && current->state !
					=LOG_STATE_SUSPENDED){
				usleep(1000);
				current->exitCode = WEXITSTATUS(taskStatus);
				current->state = LOG_STATE_FINISHED;
				log_status_change(current->taskNumber, current->PID,
						current->fg_bg, current->cmd, LOG_TERM);
			}
		}
	}
}
//CHECK USER INPUT TO DISCOVER IF WE CALL A FUNCTION OR ADD A TASK
int containsInstruction(const char *needle,const char *haystack[]){
	if(!needle || !haystack) return 0;
	for(int i =0; haystack[i]; i++){
		if(strcmp(needle, haystack[i]) == 0) return 1;
	}
	return 0;
}
//RETURNS LARGEST TASK NUMBER IN THE LIST
int biggestTaskNum (Task* taskList){
	int largestTaskNum = 0;
	if(taskList == NULL) return largestTaskNum;
	Task *walk = taskList;
	while(walk!=NULL){
		largestTaskNum = walk->taskNumber;
		walk = walk->next;
	}
	return largestTaskNum;
}
//RETURN A TASK GIVEN ITS TASK NUMBER
Task* getTask (Task* taskList, int taskNum){
	if(taskList == NULL) return NULL;;
	Task *walk = taskList;
	while(walk!=NULL){
		if(walk->taskNumber == taskNum) return walk;
		walk = walk->next;
	}
	return NULL;
}
//DELETE GIVEN TASK NUMBER FROM TASK LIST
void deleteTask(Task *taskList, int taskNum){
	//CHECK STATE
	if(taskList->taskNumber == taskNum){
		if(taskList->state == LOG_STATE_RUNNING || taskList->state ==
				LOG_STATE_SUSPENDED){
			log_status_error(taskNum, taskList->state);
			return;
		}
		//HEAD CASE
		*taskList = *taskList->next;
		taskListSize--;
		log_delete(taskNum);
		return;
	}
	//UPDATE LIST
	Task *walk = taskList;
	while(walk->next!=NULL){
		if(walk->next->taskNumber == taskNum){
			if(walk->next->state == LOG_STATE_RUNNING || walk->next->state ==
					LOG_STATE_SUSPENDED){
				log_status_error(taskNum, walk->next->state);
				return;
			}
			walk->next = walk->next->next;
			log_delete(taskNum);
			taskListSize--;
			return;
		}
		walk = walk->next;
	}
	//ERROR IF TASK NOT FOUND
	log_task_num_error(taskNum);
}
//CREATES A NEW TASK AND ADDS IT TO THE CURRENT TASK LIST
void createBasicTask (Task* taskList, char *cmd, char **argv){
	//INITIALIZE
	Task *newTask = malloc(sizeof(Task));
	newTask->taskNumber = biggestTaskNum(taskList)+1;
	newTask->state = LOG_STATE_READY;
	newTask->PID = 0;
	newTask->next = NULL;
	newTask->fg_bg = -1;
	newTask->cmd = malloc(sizeof(cmd));
	strcpy(newTask->cmd, cmd);
	//GET ARGS
	for(int i = 0; argv[i]!=NULL;i++){
		newTask->args[i]= string_copy(argv[i]);
		if(argv[i+1] == NULL) newTask->args[i+1] = NULL;
	}
	//EMPTY LIST CASE
	if(taskListSize == 0){
		*taskList = *newTask;
		taskListSize++;
		return;
	}
	//WALK TO END OF LIST, INSERT, AND INCREMENT
	Task *walk = taskList;
	while(walk->next!=NULL){
		walk=walk->next;
	}
	walk->next = newTask;
	taskListSize++;
}
//WALK DOWN LIST LOGGING THE INFORMATION OF EACH TASK
void listTasks(Task *taskList){
	if(taskList == NULL) return;
	Task *walk = taskList;
	while(walk != NULL){
		log_task_info(walk->taskNumber, walk->state, walk->exitCode, walk->PID,
				walk->cmd);
		walk = walk->next;
	}
}
//START EITHER A FG OR BG PROCESS
int start(Task *taskList, int taskNum, int fg_bg, char *in, char *out){
	//ERROR CHECKING
	Task *taskStart = getTask(taskList, taskNum);
	if(taskStart == NULL){
		log_task_num_error(taskNum);
		return 1;
	}
	if(taskStart->state == LOG_STATE_RUNNING || taskStart->state ==
			LOG_STATE_SUSPENDED){
		log_status_error(taskNum, taskStart->state);
		return 1;
	}
	//APPLY CUSTOM SIGNAL HANDLER
	//SET PATHS FOR EXECV AND SWITCH TO RUN STATE
	char path1[250] = "./";
	char path2[250] = "/usr/bin/";
	taskStart->state = LOG_STATE_RUNNING;
	//FORK, SIGNAL HANDLING
	taskStart->fg_bg = fg_bg;
	sa.sa_handler = handler;
	sigaction(SIGTSTP, &sa, NULL);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGCHLD, &sa, NULL);
	taskStart->PID = fork();
	//PARENT
	if(taskStart->PID!=0){
		if(fg_bg == 0) log_status_change(taskNum, taskStart->PID, LOG_FG,
				taskStart->cmd, LOG_START);
		else log_status_change(taskNum, taskStart->PID, LOG_BG, taskStart->cmd,
				LOG_START);
	}
	//CHILD
	if(taskStart->PID == 0){
		setpgid(0,0);
		strcat(path1, taskStart->args[0]);
		strcat(path2, taskStart->args[0]);
		//FILE REDIRECTION
		if(in != NULL){
			int fd = open(in, O_RDONLY,0644);
			if(fd == -1){
				log_file_error(taskNum,in);
				exit(1);
			}
			dup2(fd, STDIN_FILENO);
			log_redir(taskNum,LOG_REDIR_IN, in);
		}
		else if(out!=NULL){
			int fd = open(out, O_WRONLY|O_TRUNC|O_CREAT, 0644);
			if(fd == -1){
				log_file_error(taskNum,out);
				exit(1);
			}
			dup2(fd, STDOUT_FILENO);
			log_redir(taskNum,LOG_REDIR_OUT, out);
		}
		//EXECV
		if(execv(path1, taskStart->args)){
			if(execv(path2, taskStart->args)){
				log_start_error(taskStart->cmd);
				exit(1);
			}
		}
	}
	//WAIT IF FG
	if(fg_bg == 0){
		int status = 0;
		wait(&status);
		if(WIFEXITED(status) && taskStart->state == LOG_STATE_RUNNING){
			taskStart->exitCode = WEXITSTATUS(status);
			taskStart->state = LOG_STATE_FINISHED;
			log_status_change(taskStart->taskNumber, taskStart->PID,
					taskStart->fg_bg, taskStart->cmd, LOG_TERM);
		}
	}
	return 0;
}
//CHAIN TWO TASKS TOGETHER WITH FIRST BEING FG AND THE SECOND BG
void chain(Task *taskList, int taskNum1, int taskNum2){
	//ERROR CHECKING
	Task* task1 = getTask(taskList,taskNum1);
	Task* task2 = getTask(taskList,taskNum2);
	if(task1 == NULL){
		log_task_num_error(taskNum1);
		return;
	}
	if(task2 == NULL){
		log_task_num_error(taskNum2);
		return;
	}
	if(task1->state == LOG_STATE_RUNNING || task1->state == LOG_STATE_SUSPENDED){
		log_status_error(taskNum1, task1->state);
		return;
	}
	if(task2->state == LOG_STATE_RUNNING || task2->state == LOG_STATE_SUSPENDED){
		log_status_error(taskNum2, task2->state);
		return;
	}
	//START AND WAIT FOR FG TO FINISH
	log_chain(taskNum1, 0, taskNum2, LOG_CHAIN_START);
	start(task1, taskNum1, 0, NULL, NULL);
	//START BG ON SUCCESSFUL FG TERMINATION
	if(task1->exitCode == 0 && task1->state == LOG_STATE_FINISHED){
		log_chain(taskNum1, task1->exitCode, taskNum2, LOG_CHAIN_PASS);
		start(task2, taskNum2, 1, NULL, NULL);
	}
	//PRINT ERROR
	else{
		log_chain(taskNum2, task1->exitCode, taskNum2, LOG_CHAIN_FAIL);
	}
}
//KILL A TASK GIVN ITS TASK NUMBER
void killTask(Task* taskList, int taskNum){
	//ERROR CHECKING
	Task *killTask = getTask(taskList, taskNum);
	if(killTask == NULL){
		log_task_num_error(taskNum);
		return;
	}
	if(killTask->state!=LOG_STATE_RUNNING){
		log_status_error(taskNum, killTask->state);
		return;
	}
	if(killTask->fg_bg==0)return;
	//KILL TASK
	kill(killTask->PID,SIGINT);
}
//SWITCH TASK FROM BG TO FG
void fg(Task *taskList, int taskNum){
	//ERROR CHECKING
	Task *toFG = getTask(taskList, taskNum);
	if(toFG == NULL){
		log_task_num_error(taskNum);
		return;
	}
	if(toFG->state!=LOG_STATE_RUNNING){
		log_status_error(taskNum, toFG->state);
		return;
	}
	//UPDATE AND WAIT
	toFG->fg_bg = 0;
	log_fg(taskNum, toFG->PID, toFG->cmd);
	int status = 0;
	waitpid(-1,&status,0);
	if(WIFEXITED(status) && toFG->state && toFG->state == LOG_STATE_RUNNING){
		toFG->state = LOG_STATE_FINISHED;
		toFG->exitCode = WEXITSTATUS(status);
		log_status_change(toFG->taskNumber, toFG->PID, toFG->fg_bg, toFG->cmd,
				LOG_TERM);
	}
}
//SUSPEND A TASK GIVEN ITS TASK NUMBER
void suspend(Task *taskList, int taskNum){
	//ERROR CHECKING
	Task *susp = getTask(taskList,taskNum);
	if(susp == NULL){
		log_task_num_error(taskNum);
		return;
	}
	if(susp->state == LOG_STATE_READY || susp->state == LOG_STATE_FINISHED ||
			susp->state == LOG_STATE_KILLED){
		log_status_error(taskNum, susp->state);
		return;
	}
	if(susp->fg_bg == 0) return;
	//SUSPEND THE TASK
	kill(susp->PID,SIGTSTP);
}
//RESUME A TASK AS EITHER FG OR BG
void resume(Task *taskList, int taskNum, int fg_bg){
	//ERROR CHECKING
	Task *resumeTask = getTask(taskList, taskNum);
	if(resumeTask == NULL){
		log_task_num_error(taskNum);
		return;
	}
	if(resumeTask->state!=LOG_STATE_SUSPENDED){
		log_status_error(taskNum, resumeTask->state);
		return;
	}
	//UPDATE HANDLERS
	resumeTask->fg_bg = fg_bg;
	if(fg_bg == 0){
		resumeTask->state = LOG_STATE_RUNNING;
		log_status_change(resumeTask->taskNumber, resumeTask->PID, resumeTask-
				>fg_bg, resumeTask->cmd, LOG_RESUME);
		sa.sa_handler = SIG_IGN;
		sigaction(SIGCHLD, &sa, NULL);
		kill(resumeTask->PID, SIGCONT);
	}
	else{
		sa.sa_handler = handler;
		sigaction(SIGCHLD, &sa,NULL);
		kill(resumeTask->PID, SIGCONT);
	}
	int taskStatus = 0;
	//WAIT FOR FG
	if(resumeTask->fg_bg == 0){
		//POLL FOR STATUS CHANGES
		while(resumeTask->state != LOG_STATE_FINISHED){
			//CNTRL Z HIT
			waitpid(-1, &taskStatus,WUNTRACED);
			if(WIFSTOPPED(taskStatus)) return;
			//CNTRL C HIT
			waitpid(-1, &taskStatus,WNOHANG);
			if(resumeTask->state == LOG_STATE_KILLED) return;
			//NORMAL TERMINATION
			if(WIFEXITED(taskStatus) && LOG_STATE_RUNNING == resumeTask-
					>state){
				resumeTask->exitCode = WEXITSTATUS(taskStatus);
				resumeTask->state = LOG_STATE_FINISHED;
				log_status_change(resumeTask->taskNumber, resumeTask->PID,
						resumeTask->fg_bg, resumeTask->cmd, LOG_TERM);
				return;
			}
		}
	}
}
//PIPE OUTPUT OF TASK A AS INPUT FOR TASK B
void pipeTasks(Task* taskList, int taskNum1, int taskNum2){
	//ERROR CHECKING
	if(taskNum1 == taskNum2){
		log_pipe_error(taskNum1);
		return;
	}
	Task *task1 = getTask(taskList, taskNum1);
	Task *task2 = getTask(taskList, taskNum2);
	if(task1 == NULL){
		log_task_num_error(taskNum1);
		return;
	}
	if(task2 == NULL){
		log_task_num_error(taskNum2);
		return;
	}
	//SET VARIABLES
	char path1[250] = "./";
	char path2[250] = "/usr/bin/";
	int ary[2] = {0};
	//CHECK FOR PIPE ERROR ON OPEN AND SET REST OF VARIABLES
	if(pipe(ary)==-1){
		log_file_error(taskNum1, LOG_FILE_PIPE);
		return;
	}
	log_pipe(taskNum1,taskNum2);
	int in_p = ary[1];
	int out_p = ary[0];
	//FORK TO CREATE TASK 1
	task1->PID = fork();
	if(task1->PID!=0){
		log_status_change(taskNum1, task1->PID, LOG_BG, task1->cmd,LOG_START);
		task1->fg_bg = 1;
		task1->state = LOG_STATE_RUNNING;
		//CREATE TASK 2
		task2->PID = fork();
		if(task2->PID != 0) {
			log_status_change(taskNum2, task2->PID, LOG_FG, task2-
					>cmd,LOG_START);
			task2->fg_bg = 0;
			sa.sa_handler = handler;
			sigaction(SIGCHLD, &sa, NULL);
		}
	}
	//PIPE AND EXEC FOR TASK 1
	if(task1->PID == 0){
		setpgid(0,0);
		strcat(path1, task1->args[0]);
		strcat(path2, task1->args[0]);
		close(out_p);
		dup2(in_p, STDOUT_FILENO);
		close(in_p);
		if(execv(path1, task1->args)){
			if(execv(path2, task1->args)){
				log_start_error(task1->cmd);
				exit(1);
			}
		}
	}
	//PIPE AND EXEC FOR TASK 2
	if(task2->PID == 0 ){
		setpgid(0,0);
		strcat(path1, task2->args[0]);
		strcat(path2, task2->args[0]);
		close(in_p);
		dup2(out_p, STDIN_FILENO);
		close(out_p);
		if(execv(path1, task2->args)){
			if(execv(path2, task2->args)){
				log_start_error(task1->cmd);
				exit(1);
			}
		}
	}
	//SIGNAL HANDLER AND CLOSE PIPE
	int status=0;
	//WAIT FOR FG
	waitpid(task2->PID,&status,0);
	if(WIFEXITED(status)){
		task2->state=LOG_STATE_FINISHED;
		task2->exitCode = WEXITSTATUS(status);
		log_status_change(task2->taskNumber, task2->PID, task2->fg_bg, task2-
				>cmd, LOG_TERM);
	}
	//CLOSE PIPES
	close(out_p);
	close(in_p);
}
/*-------------------------------------------*/
/*-------------------------------------------*/
/* The entry of your task manager program */
int main() {
	char *cmd = NULL;
	int do_run_shell = RUN_SHELL;
	int piped = 0;
	taskList = malloc(sizeof(Task));
	/* Intial Prompt and Welcome */
	log_intro();
	log_help();
	/* Shell looping here to accept user command and execute */
	while (do_run_shell == RUN_SHELL) {
		char *argv[MAXARGS+1] = {0}; /* Argument list */
		Instruction inst = {0}; /* Instruction structure: check parse.h
		 */
		/* Print prompt */
		if(piped == 0)log_prompt();
		piped = 0;
		/* Get Input - Allocates memory for the cmd copy */
		cmd = get_input();
		/* If the input is whitespace/invalid, get new input from the user. */
		if(cmd == NULL) {
			continue;
		}
		/* Parse the Command and Populate the Instruction and Arguments */
		initialize_command(&inst, argv); /* initialize arg lists and instruction
		 */
		parse(cmd, &inst, argv); /* call provided parse() */
		if (DEBUG) { /* display parse result, redefine DEBUG to turn it off */
			debug_print_parse(cmd, &inst, argv, "main (after parse)");
		}
		/* After parsing: your code to continue from here */
		/*================================================
		 *| - The command has been parsed and you have cmd, inst, and argv filled
with data
		 *| - Very highly recommended to start calling your own functions after
this point.
		 *=+==============================================*/
		//INSTRUCTIONS LIST CONTAINS USER INPUT OR NOT
		if(!containsInstruction(inst.instruct, instructions)){
			createBasicTask(taskList, cmd, argv);
			log_task_init(biggestTaskNum(taskList),cmd);
		}
		/*==BUILT_IN: delete===*/
		else if (strcmp(inst.instruct, "delete")==0){
			if(inst.num == taskList->taskNumber && taskListSize == 1){
				taskList = malloc(sizeof(Task));
				taskListSize--;
				log_delete(inst.num);
			}
			else{
				deleteTask(taskList, inst.num);
			}
		}
		/*==BUILT_IN: list===*/
		else if (strcmp(inst.instruct, "list")==0){
			log_num_tasks(taskListSize);
			listTasks(taskList);
		}
		/*==BUILT_IN: help===*/
		else if (strcmp(inst.instruct, "help")==0){
			log_help();
		}
		/*==BUILT_IN: quit===*/
		else if (strcmp(inst.instruct, "quit")==0){
			do_run_shell = STOP_SHELL; /*set the main loop to exit when you finish
processing it */
			log_quit();
		}
		/*==BUILT_IN: start_fg===*/
		else if (strcmp(inst.instruct, "start_fg")==0){
			start(taskList, inst.num, 0, inst.infile, inst.outfile);
		}
		/*==BUILT_IN: start_bg===*/
		else if (strcmp(inst.instruct, "start_bg")==0){
			start(taskList, inst.num, 1, inst.infile, inst.outfile);
		}
		/*==BUILT_IN: chain===*/
		else if (strcmp(inst.instruct, "chain")==0){
			chain(taskList, inst.num, inst.num2);
		}
		/*==BUILT_IN: kill===*/
		else if (strcmp(inst.instruct, "kill")==0){
			killTask(taskList, inst.num);
		}
		/*==BUILT_IN: fg===*/
		else if (strcmp(inst.instruct, "fg")==0){
			fg(taskList, inst.num);
		}
		/*==BUILT_IN: suspend===*/
		else if (strcmp(inst.instruct, "suspend")==0){
			suspend(taskList, inst.num);
		}
		/*==BUILT_IN: resume_fg===*/
		else if (strcmp(inst.instruct, "resume_fg")==0){
			resume(taskList, inst.num, 0);
		}
		/*==BUILT_IN: resume_bg===*/
		else if (strcmp(inst.instruct, "resume_bg")==0){
			resume(taskList, inst.num, 1);
		}
		/*==BUILT_IN: pipe===*/
		else if (strcmp(inst.instruct, "pipe")==0){
			piped = 1;
			pipeTasks(taskList, inst.num, inst.num2);
		}
		/*.===============================================.
		 *| After your code: We cleanup before Looping to the next command.
		 *| free_command WILL free the cmd, inst and argv data.
		 *| Make sure you've copied any needed information to your Task first.
		 *| Hint: You can use the util.c functions for copying this information.
		 *+===============================================*/
		free_command(cmd, &inst, argv);
		cmd = NULL;
	} // end while
	return 0;
} // end main()
