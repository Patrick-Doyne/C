/*
 * Description: C source file containing the functions add new pairs to the global BST
 * And also contains helper functions used to check formatting and legality
 * Author: Patrick Doyne
 * Last Updated: 03/18/2025
 * G#: G01355212
 */
#include "add.h"


node* findSpot(node *new, node *tmp){
	if(tmp == NULL) return new;
	//Check less than current subtree root
	if(strcmp(tmp->alias , new->alias) > 0){
		tmp->left = findSpot(new, tmp->left);
	}
	//Check greater than current subtree root
	else if(strcmp(tmp->alias , new->alias) < 0){
		tmp->right = findSpot(new, tmp->right);
	}
	return tmp;
}

int add(){

	//Continue until getting a valid input
	while(1){	
	
		//Variables
		node *new = (node*)malloc(sizeof(node));
		char pair[100];
		
		//Get user input
		printf("Please enter an IPv4 address & alias pair.\n");
		fgets(pair, sizeof(pair) , stdin);
		int fields = sscanf(pair, "%d.%d.%d.%d %s", &new->octet[0], &new->octet[1], &new->octet[2], &new->octet[3], new->alias);
		
		//Make sure there are no NULL fields
		if(fields!=5){
			printf("ERROR: BAD FORMATTING\n");
			free(new);
			continue;
		}

		node *tmp = head;

		//Cross check legality
		//Format
		if(format(tmp, new)) continue;
		if(illegal(tmp, new)) continue;

		//Dont waste resources using recursion for this base case
		if(head == NULL) {
			head = new;
			return 1;
		}

		new->left = NULL;
		new->right = NULL;

		//Adjust pointers
		head = findSpot(new,tmp);
		
		break;
	}

	return 1;
}

//FORMATTING
////Uniform spacing and uppercasing for alias
int format(node* tmp, node *new){	
		if(illegal(tmp, new))return 1;
		new->alias[10] = '\0';
		for(int i = (int)strlen(new->alias); i <10; i++) new->alias[i] = ' ';
		for(int i = 0 ; i < (int)strlen(new->alias) ; i++) new->alias[i] = toupper(new->alias[i]);
		char *nLine = strchr(new->alias, '\n');
		nLine = " ";
		return 0;

}
//Check length of alias and values of address within octet
int illegal(node *ptr, node *new){
	//Check if address or alias already exist
	if(contains(ptr,new, 0)){
		printf("Please enter a NEW alias AND NEW address\n");
		free(new);
		return 1;
	}
	//Check that address is within legal range
	else if(new->octet[0] <  0 || new->octet[1] < 0 || new->octet[2] < 0 || new->octet[3] < 0 || new->octet[0] > 255 || new->octet[1] > 255 || new->octet[2] > 255 || new->octet[3] > 255){
		printf("\nAddress error: Please enter an address made up of 4 intergers in the range 0-255, inclusive, and seperated by periods\n");
		free(new);
		return 1;
	}
	//Check to ensure alias is in legal range
	else if(strlen(new->alias) > 10 || new->alias[0] == ' ' || new->alias[0] == '\0'){
		printf("Alias error: Please enter an alias that is at most 10 characters long, at least one character long, and does NOT contain a space. \n");
		free(new);
		return 1;
		}
	return 0;
}

//MODE 0: Valid Entry Search
//MODE 1: Update Existing Entry
//Mode 2: Check if IPv4 address is already in use when updating
//Searches to see if the list already contains the node in question
int contains(node *temp, node *new, int mode){
	int x = 0;
	//Loop through list
	if(temp!=NULL){
		//Update: looks for the duplicate
		if(mode == 1 && strcmp(new->alias, temp->alias) == 0 && (new->octet[0] != temp->octet[0] || new->octet[1] != temp->octet[1] || new->octet[2] != temp->octet[2] || new->octet[3] != temp->octet[3])){
			printf("Updating address...\n");
			return 0;
		}
		if(mode == 2 && (new->octet[0] == temp->octet[0] && new->octet[1] == temp->octet[1] && new->octet[2] == temp->octet[2] && new->octet[3] == temp->octet[3])){
			printf("UPDATE FAILED! IPv4 IS ALREADY IN USE\n");
			return 2;
		}
		//Add: protection against duplicates
		if(mode == 0 && (strcmp(new->alias,temp->alias) == 0 || (new->octet[0] == temp->octet[0] && new->octet[1] == temp->octet[1] && new->octet[2] == temp->octet[2] && new->octet[3] == temp->octet[3]))){
			printf("ERROR: DUPLICATE DETECTED\n");
			return 1;
		}
		
		//Pre order search because IMO it make this method easier to follow 
		x += contains(temp->left, new, mode);
		x += contains(temp->right, new, mode);

	}
	return x;
}
