#include "add.h"
#include "aliasLocation.h"
#include "delete.h"
#include "list.h"
#include "lookup.h"
#include "quit.h"
#include "save.h"
#include "update.h"

node* head = NULL;

void readInData(){
	
	FILE *ptr;
	char fileName[25];
	char pairs[100];

	//Get UI file name
	while(1){
		printf("Please input a .txt file that contains address and alias pairs \n\n\t(FORMATTING: [0-255].[0-255].[0-255].[0-255] [alias <= 10 characters])\n\n\t\t (NOTE: ONE PAIR PER LINE WILL BE FOUND AND WILL STILL BE RECOGNIZED\n\t\t IF SURROUNDED BY OTHER CHARACTERS. IT JUST NEEDS TO FOLLOW\n\t\t THE FORMATTING.\n\n");
		fgets(fileName, sizeof(fileName), stdin);
		fileName[strlen(fileName)-1] = '\0';
		ptr = fopen(fileName, "r");
	
		if(ptr == NULL){
			printf("ERROR: CANT FIND OR OPEN INPUTED FILE TRY AGAIN :)\n\n");
		}
		else break;
	}
	
	//Parse the file
	while(fgets(pairs, 100, ptr)!= NULL){
		node *new = (node*)malloc(sizeof(node));
		int fields = sscanf(pairs, "%d.%d.%d.%d %s", &new->octet[0], &new->octet[1], &new->octet[2], &new->octet[3], new->alias);
		
		//Make sure there are no NULL fields
		if(fields!=5){
			printf("ERROR: BAD FORMATTING\n");
			free(new);
			continue;
		}

		//Make sure everything is intit
		new->left = NULL;
		new->right = NULL;

		node *tmp = head;

		//Cross check legality
		//Format
		if(format(tmp, new)) continue;
		if(illegal(tmp, new)) continue;

		//Dont waste resources using recursion for this base case
		if(head == NULL) {
			head = new;
			continue;
		}
		printf("NEWEST ALIAS: %s with length of %d\n" , new->alias, strlen(new->alias));

		//Adjust pointers
		head = findSpot(new,tmp);
	}
	printf("FILE UPLOADING...\n SUCCESS! :)\n");
	fclose(ptr);
	fflush(stdin);
}

int main(){

readInData();

while(1){

	printf("Please select an integer menu option in the range 1-8\n");
	printf("\t\tMenu:\n");
	printf("1) Add address\n");
	printf("2) Look up address\n");
	printf("3) Update address\n");
	printf("4) Delete address\n");
	printf("5) Display list\n");
	printf("6) Display alias for location\n");
	printf("7) Save to file\n");
	printf("8) Quit\n");
	
	//User input
	char option[100] = "\0";
	fgets(option, 100, stdin);
	//User entered too much input -> try again :)
	if(strlen(option) > 2) option[0] = '0';

	switch(option[0]){

		case '1':
			add();
			continue;

		case '2':
			look();
			continue;
		
		case '3':
			update();
			continue;

		case '4':
			delete();
			continue;

		case '5':
			list();
			continue;

		case '6':
			aliasLocation();
			continue;

		case '7':
			save();
			continue;

		case '8':
			quit();
			return 0;

		default:
			continue;
	}
	return -1;
}
}

