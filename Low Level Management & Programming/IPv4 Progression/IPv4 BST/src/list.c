/*
 * Description: C source file containing the function to print out the global BST
 * Author: Patrick Doyne
 * Last Updated: 03/18/2025
 * G#: G01355212
 */
#include "list.h"

//Print out alphabetically
void inOrder(node *tmp){
	if(tmp != NULL){
		inOrder(tmp->left);
		printf("%d.%d.%d.%d %s\n",tmp->octet[0], tmp->octet[1],tmp->octet[2], tmp->octet[3], tmp->alias);
		inOrder(tmp->right);
	}
}

void list(){
	node* tmp = head;

	//Empty list
	if(head==NULL){
		printf("The list is empty\n");
		return;
	}

	//Walk through the list and print 
	inOrder(tmp);
	return;
}

