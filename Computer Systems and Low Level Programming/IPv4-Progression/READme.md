
This folder contains an ordered set of programs that piggy back off one another to advance utility in various ways. Note: all files/programs are seperate and implementation progression is conceptually drawn on to create different more interesting programs.
  Broadly Speaking: 
    -These programs represent the mutating, sorting, and storing of data pretaining to a typical internet I.P. address and username. 
    
Functionality of last program can be layered for a UI experience* (    Coming soon: Spring 2026 :)  )*

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Shared Logic:
  1)Variables and Methods
    -IPv4 address: a quartet of intergers ranged 0-255 with a period as a delimeter between each inclusive pair-wise pair acting as a typical I.P address.
    -Alias: a character respresentation of what would likely be a username for an employee, quest wifi user, etc
    -The functionality (semantics aside ofc) of many core functions remains the same, however, the implementation is vastly different
  2)Input/Out and Storage:
    - allows user to specify a file to store new information into a new file or import an existing file
    - allows user to save to a new file or override the current file 
    - allows user to discard any changes and exit gracefully
  3)Error Checking
    -Duplicates are ignored. Duplicates are classified as the following: matching alias [NO cap sensitivity] and/or matching Ipv4 addresses.
    -IPv4 addresses must be an integer value between 0 and 255 and be seperated by periods otherwise entire current line in file is skipped and we look for the next valid entry beginning at the start of the next line.
    -Similarly, aliases must be a string of characters length 1-10 inclusive, thats not a duplicate, and does not contain one of the follow characters: {'!', ... }
  4)Data Format
    -All alias under length ten are padded to length ten with spsces.
    -All functions store, read, or use pairs in the format <IPv4, alias>
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

1) IPv4.c:
     -A template conveying basic understanding of program flow, typeDef, and elementry I/O. Explores creating a basic terminal stdout UI with basic menu options allowing a user to retrieve information through a basic text file. This file can contain errors, however, only within limits: {DONT FORGET TO UPDATE THIS LATER} it is designed for finding pairs of IPv4 addresses and  aliases, as defined in the shared logic above, and then stored in a user-defined list. Offers user an option to sort the list in either Ascending or Decsending order and (2) save a current list to an existing or new file so long as there is memory available. 


