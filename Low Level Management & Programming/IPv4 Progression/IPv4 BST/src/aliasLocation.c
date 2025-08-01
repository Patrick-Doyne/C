/*
 * Description: C source file containing the function to find pairs in the global BST
 * which have the same location
 * Author: Patrick Doyne
 * Last Updated: 03/18/2025
 * G#: G01355212
 */
#include "aliasLocation.h"

void aliasSearch(node *tmp, int loc1, int loc2, int *count){
	if(tmp!= NULL){
		//Search left
		aliasSearch(tmp->left,loc1,loc2, count);
		//Print
		if(tmp->octet[0] == loc1 && tmp->octet[1] == loc2){
			//Address found
			count++;
			printf("ALIAS FOUND: %s\n", tmp->alias);
		}
		
		//Search right
		aliasSearch(tmp->right, loc1, loc2, count);
	}
}

void aliasLocation(){

	//Empty list
	if(head == NULL){
		printf("The list is empty.\n");
		return;
	}
	
	//Location vars
	int loc1 = -1;
	int loc2 = -1;

	//User input var
	char filter[100];

	//Loop until valid input
	while(0>loc1 || loc1>255 || 0>loc2 || loc2>255){
		//Format warning
		printf("The address location consists of numbers between the values 0 and 255. If either number is out of this range this message will reappear.\n");
		
		//Get first loc value from user
		printf("Enter address location's first value (0-255 inclusive)\n");
		fgets(filter, sizeof(filter), stdin);
		sscanf(filter,"%d", &loc1);

		//Ensure input is legal and within range
		for(int i = 0 ; i < (int)strlen(filter); i++){
			if(filter[i] == '\n')break;
			if(filter[i] < '0' || filter[i] > '9'){
				loc1 = -1;
				break;
			}
		}
		//If not then loop
		if(loc1 <= -1 || loc1 >= 256) continue;

		//If so repeat process for second loc value
		printf("Enter address location's second value (0-255 inclusive)\n");
		fgets(filter, sizeof(filter), stdin);
		sscanf(filter,"%d", &loc2);
		for(int i = 0 ; i < (int)strlen(filter); i++){
			if(filter[i] == '\n')break;
			if(filter[i] < '0' || filter[i] > '9'){
				loc2 = -1;
				break;
			}
		}
	}
	
	int *count = 0;
	node* tmp = head;

	aliasSearch(tmp,loc1,loc2, count);
	
	//Search
	if(count == 0) printf("This location doesn't exist within the list.\n");
}
