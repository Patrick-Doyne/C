/*
 * Description: C source file containing the function to save pairs in the global BST
 * to a user specified file
 * Author: Patrick Doyne
 * Last Updated: 03/18/2025
 * G#: G01355212
 */
#include "save.h"

//Print everything to a text file in alphabetical order
void fInOrder(node* tmp, FILE* ptr){
	if(tmp!=NULL){
		fInOrder(tmp->left, ptr);
		fprintf(ptr, "%d.%d.%d.%d %s\n",tmp->octet[0],tmp->octet[1],tmp->octet[2],tmp->octet[3], tmp->alias);
		fInOrder(tmp->right, ptr);
	}
}

void save(){

	//File name
	char fileName[25];
	
	//Get file name from user
	printf("Please enter name of the file you want to save to: \n");
	fgets(fileName, 25, stdin);
	fileName[strlen(fileName)-1] = '\0';

	//Open the file
	FILE* ptr = fopen(fileName, "w");
	if(!ptr){
	 perror("fopen() error");
	 return;
	}

	//Read through and print to file the entire list
	node *tmp = head;
	if(head == NULL){
		printf("List is empty\n");
		fclose(ptr);
		return;
	}
	fInOrder(tmp,ptr);
	//Close file
	fclose(ptr);
	printf("File has been saved!\n");

	return;

}
