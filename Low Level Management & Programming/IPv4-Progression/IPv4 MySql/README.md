*Program Strcuture & Design*:

   	- The user is promted to enter the name of an exisitng file they wish to read in from. They are then  
	given a menu with 8 options to choose from. The user can either look up, update, or delete a pair by selecting 
	the corresponding menu option and entering the respective alias. Similarly, the user can add pair by selecting the 
	corresponding option and then inputting when promted. We can also list, save, quit or even search for alias with 
	the same location.
 
*Files & Functions*: 
    
    - The project contains 9 C source files, 9 header files, and a makefile. Quick descriptions of functions defined in these files can
    be found clustered and numbered under each pair of header and C files.

    * add.h: contains all the function declarations used for adding new legal pairs to the global list
   	* add.c: prompts the user to enter a new IPv4 pair and re-prompts until completed legaly as defined by the shared logic. Also, offers user
    the option to update the currently connected global MySQL table in addition to the global list
           
           1) int format(node* tmp ,node* new): used to format an alias and address pair
           2) int illegal(node* tmp, node *new): used to check the legality of an alias and address pair
           3) int add(): used to prompt user input and create new pair that is inputed into the BST
           4) node* findSpot(node* tmp, node* new): used as a helper to recurively add to a BST
           5) int contains(node* tmp, node *node, int mode): used to check for duplicates in the BST or find a pair in the BST or update in BST.

    * aliasLocation.h: contains all the function declarations for finding alias tied to a location
   	* aliasLocation.c: prompts the user to enter the first two numbers of the IPv4 and then searches for exisitng matches and prints them out if they exist. 
           
           1) void aliasLocation(): used to find all alias with the same location stored in the address of their IPv4 address.
           2) void aliasSearch(node *tmp, int loc1, int loc2, int *count): used to print out aliases in inorder recursive manner.
           
    * delete.h: contains all the function declarations used for deleting exisitng pairs. 
    * delete.c: prompt the user for an alias to search for and delete the corresponding pair if it exists. Confirms a second time before deleting. Also, offers user
    the option to update the currently connected global MySQL table in addition to the global list.
    
           1) int delete(): used to delete a pair from the BST.
           2) node* inOrderReplacement(node* root, node *del): used to help recursively delete a node from BST.
           
    * list.h: contains the function declaration used for listing the current pairs
    * list.c: prints out, one per line, all exisitng pairs within the BST
    
           1) void list(): used to print out the pair BST to the user in terminal.
           2) void inOrder(node *tmp): print inorder traversal of BST.

    * lookup.h: contains all the function declarations needed to look for an address
    * lookup.c: prompt the user to enter an alias they wish to find the IPv4 address of.
    
           1) void* look(): used to find a user inputed pair and returns a pointer to it.
           2) node* singleSearch(node* tmp, char* alias): helper for look() to find specific node in BST.

    * menu.h: contains the function declarations for reading in input and displaying the UI menu
    * menu.c: contains UI menu and the function used to read in the input from a user's MySQL database
  	 
    * quit.h: contains the declaration for the function that exits the program safely
    * quit.c: exits in a memory safe manner
          
           1) void quit(): used to safely release heap and then exit the program
           2) void freeBST(node* tmp) Free malloc mem before exiting 

    * save.h contains the declaration of the function that saves the current BST to the user inputted MYSQL table 
    * save.c: saves the current BST to a user inputed MYSQL table
    
           1) void save(): used to save the pair BST to a user selected file
           2) void fInOrder(node* tmp, FILE* ptr) Helper to recursively save to file in inorder fashion

    * update.h contains the declaration of the function that is used to update an existing pair within the BST. 
    * update.c: prompts the user for the alias of a pair they want to update and then prompts for the new address and ensures its valid before updating. Also, offers user
    the option to update the currently connected global MySQL table in addition to the global list

           1) int update(): used to prompt the user to enter an alias then prompts them to update the pairs address field.

  	* makefile: automates the compilation process

*Compilation*:

    - The program is compiled on a Linux Server using gcc. The makefile provided automates the compilation and linking of the program. 
    First, unzip the file to see the directories and files. Copy the files from your local machine into your Linux server of choice. 
    To compile the program navigate to the src directory containing all the source code and makefile then run the following command:

	        	            [username@your-linux-server ~]$make

    		      This will create an executable file named test

    	Usage:

   			      To run the program, enter the following command:

	        	            [username@your-linux-server]$./BSTtest
   
   		Clean Up:

   			      To remove all generated files run the following command:

	        	            [username@your-linux-server]$make clean
