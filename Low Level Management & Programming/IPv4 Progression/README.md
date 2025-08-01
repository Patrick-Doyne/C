
*Directory Description*: 
	
 	- Contains an ordered set of programs that piggy back off one another to advance utility in various ways. Note: all files/programs are seperate and 		
 	implementation progression is conceptually drawn on to create different more interesting programs.
  
*Broadly Speaking*: 
   	
	- These programs successfully mutate, sort, and store data pretaining to a typical internet I.P. address and username. Extra data fields can easily be 		 
  	added.
    
*Shared Logic* :
  
  	- Variables and Methods:
    	- IPv4 address: a quartet of intergers ranged 0-255 with a period as a delimeter between each inclusive pair-wise pair acting as a typical I.P address.  
   	 	- Alias: a character respresentation of what would likely be a username for an employee, quest wifi user, etc   
   	 	- The functionality of many core functions remains relatively the same, however, the implementation is vastly different.
 
  	- Input/Out and Storage:
    	- Allows user to specify a file to store new information into a new file or import an existing file   
    	- Allows user to save to a new file or override the current file    
    	- Allows user to discard any changes and exit gracefully
 
  	- Error Checking:
    	- Duplicates are ignored. Duplicates are classified as the following: matching alias [NO cap sensitivity] and/or matching Ipv4 addresses.   
    	- IPv4 addresses must be an integer value between 0 and 255 and be seperated by periods otherwise entire current line in file is skipped and 
     	we look for the next valid entry beginning at the start of the next line.   
   	 	-Similarly, aliases must be a string of characters length 1-10 inclusive, thats not a duplicate, and does not contain one of the follow 
     	characters: {'!', ... }
  
  	- Data Format:
    	- All alias under length ten are padded to length ten with spsces.
    	- All functions store, read, or use pairs in the format <IPv4, alias>

*IPv4 List*:
     
     - Contains a template conveying basic understanding of program flow, typeDef, and elementry I/O. Explores creating a basic terminal stdout UI with basic menu
     options allowing a user to retrieve information through a basic text file. This file can contain errors, however, only within limits: {DONT FORGET TO 
     UPDATE THIS LATER} it is designed for finding pairs of IPv4 addresses and  aliases, as defined in the shared logic above, and then stored in a 
     user-defined list. Offers user an option to sort the list in either Ascending or Decsending order and (2) save a current list to an existing or new file 
     so long as there is memory available.

 *IPv4 BST*:
     
     - This project-folder contains a program that reads in a text file and creates a doubly linked BST from the contents within the file. 
     Searches for IPv4 address and alias pairs within aforementioned constraints. Allows the user to select from a menu of options to 
     alter, display, and even save the list after modification. Also offers memory leak protection.

*IPv4 MySql*:

     - This project-folder contains a similar program to the one stored in IPv4 BST. Whats different? We no longer read in from a file to create the global list.
     Instead we use the MySql API to connect to the Helios via Linux and read or create a table of the users choice. After intial creation from an sql table,
     the global list can then be merged with other text files being read in. Additionally, anytime the global list is changed the user is given the option
     to also reflect this change in the table we originally connected to. To save the entire current list this program offers: the options to save via a file or
     to ANY sql table of choice. In both cases, creation is allowed.

     - Summary: Same great functionality as the BST predecessor, but adds database utility on top.

#### NOTE: Functionality of IPv4 MySql program can be layered for a UI experience* (  Coming soon: Spring 2026 :)  )* ####

