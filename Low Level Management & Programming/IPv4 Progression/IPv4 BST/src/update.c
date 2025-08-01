/*
 * Description: C source file containing the function to update a pair in the
 * global BST
 * Author: Patrick Doyne
 * Last Updated: 03/18/2025
 * G#: G01355212
 */

#include "update.h"
#include "lookup.h"
int update(){

	//Get node we want to update
	node* tmp = look();
	if(!tmp) return 0;

	node* ptr = head;

	//Create a copy in reserved memory
	node* copy = malloc(sizeof(node));

	//Variables
	char update[100];

	//Get user input
	printf("Please enter the new address for the alias: %s\n", tmp->alias);
	fgets(update, sizeof(update), stdin);
	
	//Copy info into new node
	sscanf(update,"%d.%d.%d.%d", &copy->octet[0], &copy->octet[1], &copy->octet[2], &copy->octet[3]);
	strcpy(copy->alias, tmp->alias);

	//Check if this new node already exists in the list
	if(contains(ptr, copy, 2)){
		free(copy);
		return 1;
	}
	if(contains(ptr, copy, 1)){
		free(copy);
		return 1;
	}


	//Update and free copy
	tmp->octet[0] = copy->octet[0];
	tmp->octet[1] = copy->octet[1];
	tmp->octet[2] = copy->octet[2];
	tmp->octet[3] = copy->octet[3];
	free(copy);

	return 0;
}
