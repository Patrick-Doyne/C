/*
 * Description: C source file containing the functions delete pairs in the global BST
 * Author: Patrick Doyne
 * Last Updated: 03/18/2025
 * G#: G01355212
 */
#include "delete.h"
#include "lookup.h"

node* inOrderReplacement(node* root, node *del){
		if(!root)return NULL;
		if(strcmp(root->alias , del->alias) > 0 ) root->left = inOrderReplacement(root->left, del);
		else if(strcmp(root->alias , del->alias) < 0) root->right = inOrderReplacement(root->right, del);
				/* No Child */
		else{
			if(!root->right && !root->left){
				free(root);
				return NULL;
			}
			/* Single Child (Right) */
			else if(root->right && !root->left){
				node* jump = root->right;
				free(root);
				return jump;
			}

			/* Single Child (Left) */
			else if(!root->right && root->left){
				node* jump = root->left;
				free(root);
				return jump;
			}

			/* Node we want to delete has 2 children*/
						/* Get Successor */
			node * succ = root->right;
			while(succ && succ->left) succ = succ->left;
			strcpy(root->alias, succ->alias);
			root->octet[0] = succ->alias[0];
			root->octet[1] = succ->alias[1];
			root->octet[2] = succ->alias[2];
			root->octet[3] = succ->alias[3];
			root->right = inOrderReplacement(root->right, succ);

		}
		return root;
	
}

int delete(){

	//User input vars
	node *tmp = head;
	char del[100];

	//Get user input
	node *delete = look();
 	if(!delete) return 1;

	//Double check with user
	printf("Are you sure you want to delete this pair?\n\t Press anything to confirm and enter to abort\n");
	fgets(del, 100, stdin);
	if(del[0] == '\n') {
		printf("RETURNING... \n");
		return 1;
	}
	head = inOrderReplacement(tmp,delete);
	printf("DELETING...\n\tENTRY DELETED :)\n");
	return 0;
}
