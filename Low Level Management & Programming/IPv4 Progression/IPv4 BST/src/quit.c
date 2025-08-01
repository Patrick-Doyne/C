/*
 * Description: C source file containing the functions delete pairs all in the 
 * global BST from memory and exit program
 * Author: Patrick Doyne
 * Last Updated: 03/18/2025
 * G#: G01355212
 */
#include "quit.h"

//Free all allocated mem before exiting
void freeBST(node* tmp){

	if(tmp!=NULL){
		freeBST(tmp->left);
		freeBST(tmp->right);
		free(tmp);
	}
}
void quit(){
	//NOT EMPTY BST
	if(head!= NULL) {
		node* tmp = head;
		freeBST(tmp);		
	}
		printf("NO MEMORY LEAKS\nSHUTTING DOWN...\nGOODBYE!\n");
	
}