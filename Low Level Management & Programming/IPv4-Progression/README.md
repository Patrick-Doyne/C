
*Directory Description*: 
	
 	- Contains an ordered set of programs that piggy back off one another to advance utility in various ways. Note: all files/programs are seperate and 		
 	implementation progression is conceptually drawn on to create different more interesting programs.
  
*Broadly Speaking*: 
   	
	- These programs successfully mutate, sort, and store data pretaining to a typical internet I.P. address and username. Extra data fields can easily be 		 
  	added.
    
*Shared Logic*:
  
  1) Variables and Methods
    
    - IPv4 address: a quartet of intergers ranged 0-255 with a period as a delimeter between each inclusive pair-wise pair acting as a typical I.P address.  
    - Alias: a character respresentation of what would likely be a username for an employee, quest wifi user, etc   
    - The functionality of many core functions remains relatively the same, however, the implementation is vastly different.
 
  2) Input/Out and Storage:
   
    - Allows user to specify a file to store new information into a new file or import an existing file   
    - Allows user to save to a new file or override the current file    
    - Allows user to discard any changes and exit gracefully
 
  3) Error Checking
   
    - Duplicates are ignored. Duplicates are classified as the following: matching alias [NO cap sensitivity] and/or matching Ipv4 addresses.   
    - IPv4 addresses must be an integer value between 0 and 255 and be seperated by periods otherwise entire current line in file is skipped and we look for the 
    next valid entry beginning at the start of the next line.   
    -Similarly, aliases must be a string of characters length 1-10 inclusive, thats not a duplicate, and does not contain one of the follow characters: {'!', ... }
  
  4) Data Format
    
    - All alias under length ten are padded to length ten with spsces.
    - All functions store, read, or use pairs in the format <IPv4, alias>

### NOTE: Functionality of last program can be layered for a UI experience* (    Coming soon: Spring 2026 :)    )* ###

*IPv4-List.c*:
     
     - A template conveying basic understanding of program flow, typeDef, and elementry I/O. Explores creating a basic terminal stdout UI with basic menu
     options allowing a user to retrieve information through a basic text file. This file can contain errors, however, only within limits: {DONT FORGET TO 
     UPDATE THIS LATER} it is designed for finding pairs of IPv4 addresses and  aliases, as defined in the shared logic above, and then stored in a 
     user-defined list. Offers user an option to sort the list in either Ascending or Decsending order and (2) save a current list to an existing or new file 
     so long as there is memory available.

 *IPv4-BST*:
     
     - This project-folder contains a program that reads in a text file and creates a singly linked BST from the contents within the file. 
     Searches for IPv4 address and alias pairs within aforementioned constraints. Allow the user to select from a menu of options to 
     alter, display, and even save the list after modification. Also offers memory leak protection.

     - Files: The project contains a makefile, 9 C source files, and 9 header files.
       	 * add.c: prompts the user to enter a new IPv4 pair and re-prompts until completed legaly as defined by the shared logic
       	 * aliasLocation.c: promts the user to enter the first two numbers of the IPv4 and then searches for exisitng matches and prints them out if they exist.
      	 * delete.c: prompt the user for an alias to search for and delete the corresponding pair if it exists. Confirms a second time before deleting.
     	 * list.c: prints out, one per line, all exisitng pairs within the BST
       	 * lookup.c: prompt the user to enter an alias they wish to find the IPv4 address of.
      	 * menu.c: contains UI menu and the function used to read in the input from a user's file
      	 * quit.c: exits in a memory safe manner
      	 * save.c: saves the current BST to a user inputed file
      	 * update.c: prompts the user for the alias of a pair they want to update and then prompts for the new address and ensures its valid before updating

      	 * add.h: contains all the function declarations used for adding new pairs
      	 * aliasLocation.h: contains all the function declarations for finding alias tied to a location
       	 * delete.h: contains all the function declarationd used for deleting exisitng pairs
       	 * list.h: contains the function declaration used for listing the current pairs
       	 * quit.h: contains the declaration for the function that exits the program safely
       	 * save.h contains the declarationf or the function that saves the current BST to the user inputted file
       	 * update.h contains the declaration of the function that is used to update an existing pair within the BST.

      	 * makefile: automates the compilation process

       - Program Strcuture & Design:

       		The user is promted to enter the name of an exisitng file they wish to read in from. They are then  
  		given a menu with 8 options to choose from. The user can either look up, update, or delete a pair by selecting 
  		the corresponding menu option and entering therespective alias. Similarly, the user can add pair by selecting the 
  		corresponding option and then inputting when promted. We can also list, save, quit or even search for alias with 
  		the same location.

       - Functions:
       	* int format(node* tmp ,node* new): used to format an alias and address pair
	 	* int illegal(node* tmp, node *new): used to check the legality of an alias and address pair
   		* int add(): used to prompt user input and create new pair that is inputed into the BST
     	* node* findSpot(node* tmp, node* new): used as a helper to recurively add to a BST
         	* int contains(node* tmp, node *node, int mode): used to check for duplicates in the BST or find a pair in the BST or update in BST.

		* void aliasLocation(): used to find all alias with the same location stored in the address of their IPv4 address.
	 	* void aliasSearch(node *tmp, int loc1, int loc2, int *count): used to print out aliases in inorder recursive manner.
	 
  		* int delete(): used to delete a pair from the BST.
    	* node* inOrderReplacement(node* root, node *del): used to help recursively delete a node from BST.
  	
   		* void list(): used to print out the pair BST to the user in terminal.

		* void inOrder(node *tmp): print inorder traversal of BST.
  		* void* look(): used to find a user inputed pair and returns a pointer to it.
    	* node* singleSearch(node* tmp, char* alias): helper for look() to find specific node in BST.

		* void save(): used to save the pair BST to a user selected file
  		* void fInOrder(node* tmp, FILE* ptr) Helper to recursively save to file in inorder fashion

		* void quit(): used to safely release heap and then exit the program
  		* void freeBST(node* tmp) Free malloc mem before exiting 

		* int update(): used to prompt the user to enter an alias then prompts them to update the pairs address field.

	    - Compilation:

          		The program is compiled on a Linux Server using gcc. The makefile provided automates the compilation and linking 
	   	of the program. First, unzip the file to see the directories and files. Copy the files from your local machine into 
     	your Linux server of choice. To compile the program navigate to the src directory containing all the source code and 
       	makefile then run the following command:

		        	[username@your-linux-server ~]$make

        		This will create an executable file named test

        		Usage:

       			To run the program, enter the following command:

		        	[username@your-linux-server]$./BSTtest
	   
       		Clean Up:

       			To remove all generated files run the following command:

		        	[username@your-linux-server]$make clean
*IPv4 MySql*:

 	- Description:


