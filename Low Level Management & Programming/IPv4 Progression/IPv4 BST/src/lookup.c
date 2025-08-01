/*
 * Description: C source file containing the function to find a pair in the global list
 * Author: Patrick Doyne
 * Last Updated: 03/18/2025
 * G#: G01355212
 */
#include "lookup.h"

node* singleSearch(node* tmp, char* alias){
	//Print out match and return it
	if(tmp == NULL) return tmp;
	if(strcmp(tmp->alias, alias) == 0){
		printf("For the alias: %s \nThe corresponding address is: %d.%d.%d.%d \n",tmp->alias, tmp->octet[0], tmp->octet[1], tmp->octet[2], tmp->octet[3]);
		return tmp;
	}
	if(strcmp(tmp->alias, alias) < 0)  return singleSearch(tmp->right,alias);
	else if(strcmp(tmp->alias, alias) > 0) return singleSearch(tmp->left,alias);

}

node* look(){

	//Empty list
	if(head == NULL){
		printf("The list is empty, nothing to look up\n");
		return NULL;
	}

	//Get UI
	char alias[11];
	printf("Please enter an existing alias to retrieve its address\n");
	fgets(alias,11,stdin);
	sscanf(alias, "%s", alias);

	//Format
	for(int i = (int)strlen(alias); i <10; i++) alias[i] = ' ';
	for(int i = 0 ; i < (int)strlen(alias) ; i++) alias[i] = toupper(alias[i]);
	char *nLine = strchr(alias, '\n');
	nLine = " ";
	alias[10] = '\0';

	node* tmp = head;
	tmp = singleSearch(tmp, alias);
	if(tmp == NULL) printf("No results: returning to the menu\n");
	return tmp;
}



