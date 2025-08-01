/**
 * 
 * Patrick Doyne
 * 
 **/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

//Prototypes
char *getDateAndTime();
void Read_Data_File();
void Add_Address();
void Look_Up_Address();
void Display_List();
void Display_Locations();
void Sort();
int Save(char[], char[]);

//IPv4 address and alias as a node
typedef struct address_t{

	int octet[4];
	char alias[11];
	struct address_t *next;

}IPv4;

IPv4 *head = NULL;

//Main
int main(){
	//Get date
	char date[32];
	strcpy(date, getDateAndTime());
	//Get name
	char name[32];
	printf("Enter your name:");
	fgets(name, 32, stdin);
	name[strlen(name)-1] = '\0';

	//Input and loop control
    int flag = 1;
    char trash[10];
    
    //Read in text file
    Read_Data_File();
    
    //Loop for user input
    while(flag!=0){
        int response;
        printf("\n1) Add address\n");
        printf("2) Look up address\n");
        printf("3) Display list\n");
        printf("4) Display aliases for location\n");
        printf("5) Save to file\n");
        printf("6) Sort\n");
        printf("7) Quit\n");
        scanf("%d",&response);
        fgets(trash,sizeof(trash),stdin);
        
        switch(response){
            case 1:
                Add_Address();
                break;
            case 2:
                Look_Up_Address();
                break;
            case 3:
                Display_List();
                break;
            case 4:
                Display_Locations();
                break;
            case 5:
                Save(date,name);
                break;
            case 6:
            	Sort();
            	break;
            case 7:
            	printf("Goodbye!");
                flag = 0;
                break;
        }
    }
    return 0;
}

//Get Time and Date
char *getDateAndTime() {
    
    time_t t; 
    time(&t);
    return ctime(&t);
}

//Add an IPv4 address and alias
void Add_Address(){
    //Control and input variables
	char IPv4_Address[100];
	char IPv4_alias[100];
	int flag = 0;
	//Create new node in memory dynamically
	IPv4 *temp = (IPv4*) malloc(sizeof(IPv4));

	//Get Address
	printf("Enter an IPv4 Address: ");
	while(flag != 1){
		//Load address into array
		fgets(IPv4_Address,sizeof(IPv4_Address) , stdin);
		//Load address into node
		sscanf(IPv4_Address, "%d.%d.%d.%d", &temp->octet[0], &temp->octet[1], &temp->octet[2], &temp->octet[3]);
		//Test address for illegal values
		if( temp->octet[0] > 255 || temp->octet[0] < 0 ||
		    temp->octet[1] > 255 || temp->octet[1] < 0 ||
			temp->octet[2] > 255 || temp->octet[2] < 0 ||
			temp->octet[3] > 255 || temp->octet[3] < 0 ||
			temp->octet[0] == '\0' || temp->octet[1] == '\0' ||
			temp->octet[2] == '\0' || temp->octet[3] == '\0'){
			printf("Error! (Please enter a legal address [0-255].[0-255].[0-255].[0-255])\n");
			//Reset the node
			IPv4 *temp = (IPv4*) malloc(sizeof(IPv4));
			continue;
		}
		//Check to see if this address already exists
		IPv4 *ptr = head;
		while(memcmp(ptr->octet, temp->octet,sizeof(ptr->octet)) != 0 && ptr->next != NULL){
		    
		    ptr = ptr->next;
		    
		    if(memcmp(ptr->octet, temp->octet,sizeof(ptr->octet)) == 0){
		        printf("The address %d.%d.%d.%d already exists under the alias %s\nEnter an IPv4 Address: ",
		        ptr->octet[0],ptr->octet[1],ptr->octet[2],ptr->octet[3],ptr->alias);
		        flag = 2;
		    }
		}
		//Address already existed so we try again
		if(flag == 2){
		    flag = 0;
		    continue;
		}
		//Address was aviable and we exit the loop
		flag = 1;
	}

    //Get Alias
	printf("Enter an Alias: ");
	while(flag != -1){
		//Load alias into array
		fgets(IPv4_alias, sizeof(IPv4_alias) ,stdin);
		//Load alias into a node
		sscanf(IPv4_alias, "%s", temp->alias);
		//Test alias length
		if(strlen(temp->alias) > 10){
			printf("Error! (Please enter a legal alias [must be no more than 10 characters])\n");
			continue;
		}
		//Fill remaining indexes with space and insert null byte at max size.	
	    temp->alias[10] = '\0';
        for(int i = strlen(temp->alias); i < 10; i++){
            temp->alias[i] = ' ';
        }
        //Convert alias to all upper case letters
        for(int i = 0; i < strlen(temp->alias); i++){
            temp->alias[i] = toupper(temp->alias[i]);
        }
        //Check to make sure alias doesnt exist
		IPv4 *ptr = head;
		while(ptr!=NULL){
		    if(strcmp(ptr->alias, temp->alias)==0){
		        printf("The address %d.%d.%d.%d already exists under the alias %s\nEnter an Alias: ",ptr->octet[0],ptr->octet[1],ptr->octet[2],ptr->octet[3],ptr->alias);
		        break;
		    }
		    ptr=ptr->next;
		}
		//If pointer is null or alias available exit the loop
		if(ptr==NULL || strcmp(ptr->alias, temp->alias)!=0){
		    flag = -1;
		}
	}
    //Set the pointers
    temp->next = head;
    head = temp;
    return;
}
//Search for existing IPv4 by alias and print the IPv4
void Look_Up_Address(){
    
    //Allocate memory and get user input
    IPv4 *query = (IPv4*) malloc(sizeof(IPv4));
    printf("Enter an Alias: ");
    fgets(query->alias, sizeof(query->alias),stdin);
    
    //Convert alias to all upper case letters
    for(int i = 0; i < strlen(query->alias); i++){
        query->alias[i] = toupper(query->alias[i]);
    }
    //Add null byte at max length of alias
    query->alias[strlen(query->alias)-1] = '\0';
    //Loop through linked list to find match
    IPv4 *ptr = head;
    while(ptr!=NULL){
        if(strcmp(ptr->alias,query->alias) == 32){
            printf("The address of %s is: %d.%d.%d.%d\n", query->alias, ptr->octet[0], ptr->octet[1], ptr->octet[2], ptr->octet[3]);
            free(query);
            return;
        }
        ptr = ptr->next;
    }
    //Display error if alias doesn't exist
    printf("Error: This alias doesn't have an address.\n");
    free(query);
    return;
}

//Read in an existing data file
void Read_Data_File(){

	//Temporary vars and index counter to generate alias and IPv4 list
	char IPv4_Temp[100];
	int size = 0;
	//File name;
	char filename[35];
	//Clear keybaord
	char trash[2];
    //Declare file pointer
	FILE *IPv4_Txt = NULL;
    
    //Get file name
	while(IPv4_Txt == NULL){
		printf("Please enter file name:");
		scanf("%s",filename);
		fgets(trash, 2, stdin);
		IPv4_Txt = fopen(filename, "r");
		if(IPv4_Txt == NULL){
			printf("Error: %sCannot Be Found\n", filename);
			continue;
		}
	}
    //Loop and generate upto 100 IPv4 and alias entries into list
	while(fgets(IPv4_Temp,100,IPv4_Txt)!=NULL && size < 100){
	    IPv4 *temp = (IPv4*) malloc(sizeof(IPv4));
		sscanf(IPv4_Temp, "%d.%d.%d.%d %s", &temp->octet[0], &temp->octet[1], &temp->octet[2], 
			&temp->octet[3], temp->alias);
        //Exclude IPv4 with an int larger than 255 or with an alias longer than 10 chars
        if(strlen(temp->alias) > 10 || temp->octet[0] > 255 || temp->octet[1] > 255 
        || temp->octet[2] > 255 || temp->octet[3] > 255){
            continue;
        }
        //Convert alias to all upper case letters
        for(int i = 0; i < strlen(temp->alias); i++){
            temp->alias[i] = toupper(temp->alias[i]);
        }
        //Check for sentinel address
		if(strcmp(temp->alias, "NONE") == 0 && temp->octet[0] == 0 && temp->octet[1] == 0 
		    && temp->octet[2] == 0 && temp->octet[3] == 0){
		        //Free malloc to new link
		        free(temp);
				return;
			}
			
		//Fill remaining indexes with space and insert null byte	
		temp->alias[10] = '\0';
        for(int i = strlen(temp->alias); i < 10; i++){
            temp->alias[i] = ' ';
        }
        //Adjust pointers
        temp->next = head;
        head = temp;
        
        //Increment size
		size++;
	}
	return;
	
}
//Display linked list
void Display_List(){
    //Return if the list is empty
    if(head == NULL){
        printf("Error: The list is empty.\n");
        return;
    }
    //Counter for node being displayed
    int counter=0;
    //Display each node in the linked list to user
    IPv4 *ptr = head;
	while(ptr!=NULL){
	    printf("%s %d.%d.%d.%d [NODE #%d]\n",ptr->alias, ptr->octet[0], ptr->octet[1],
	    ptr->octet[2], ptr->octet[3],counter++);
	    ptr = ptr->next;
	}
    return;
}
//Displays the alias based on the locality of the first two numbers in an IPv4
void Display_Locations(){
    //Loop control
    int flag = 0;
    //User input
    char locality[10];
    //Dynamically allocate memory to a node used for comparison
    IPv4 *loc = (IPv4*) malloc(sizeof(IPv4));
    //Ask for user input
    printf("Enter Locality: ");
    
    //Ensure the user input is valid
    while(flag == 0){
        fgets(locality, sizeof(locality),stdin);
        sscanf(locality, "%d.%d", &loc->octet[0],&loc->octet[1]);
        if( loc->octet[0] > 255 || loc->octet[0] < 0 ||
		    loc->octet[1] > 255 || loc->octet[1] < 0 ||
			loc->octet[0] == '\0' || loc->octet[1] == '\0'){
			    printf("Error! (Please enter a legal locality [0-255].[0-255])\n");
			    continue;
		}
	    flag =1;
    }
    
    //Print out all alias that have the location inputed by user
    IPv4 *ptr = head;
    while(ptr!=NULL){
        if(ptr->octet[0] == loc->octet[0] && ptr->octet[1] == loc->octet[1]){
            printf("%s\n",ptr->alias);
            flag = 0;
        }
        ptr = ptr->next;
    }
    //If there is no avilable data dsiplay error message
    if(flag != 0){
        printf("Error: No aliases exist with the specified location.\n");
    }
    return;
}
//Save updated list to a text file
int Save(char date[], char name[]){
    //File name
    char filename[32];
    //Clear keyboard
    char trash[10];
    //Get file name
    printf("Enter file name:");
    scanf("%s",filename);
    //Clear keyboard
    fgets(trash,sizeof(trash),stdin);
    
    //Create and open the file
    FILE *output;
	output = fopen(filename, "w");
	//Display error if file cannot be opened
	if (output == NULL) {
        printf("Error opening file: %s\n", filename); 
        return(1);
    }
    //Print name and date atop of the file
    fprintf(output, "%s %s", name, date);
    //Display the file name
    fprintf(output, "%s\n\n", filename);
    
    //Print linked list to the file
    IPv4 *ptr = head;
	while(ptr!=NULL){
	    fprintf(output,"%s %d.%d.%d.%d\n",ptr->alias, ptr->octet[0], ptr->octet[1],
	    ptr->octet[2], ptr->octet[3]);
	    ptr = ptr->next;
	}
	//Close file
	fclose(output);
	printf("%s Saved Successfully! \n", filename);

	return 0;
}


//Sort linked list of IPv4's
void Sort(){

    //Loop control
	int flag = -1;
	//User input for sorting methed
	char answer;
	//Clear leftover enter key from scanf
	char trash[10];

    //Get user input for type of sorting
	printf("This report will be generated alphabpetically by alias in all caps.\n");
	while(flag == -1){
		printf("\tSort in ascending (A-Z): [A]\n");
		printf("\tSort in descending (Z-A): [D]\n");
		scanf("%c", &answer);
		fgets(trash,10,stdin);
		if(answer != 65 && answer != 68){
			printf("Enter a legal value:\n");
			continue;
		}
		flag = 0;
	}
	//Count number of nodes
	IPv4 *curr = head;
	int counter;
	while(curr!=NULL){
	    counter++;
	    curr = curr->next;
	}
    //Sort through every node
    while(counter!=0){
        //Pointers used for swaps
	    IPv4 *ptr = head;
	    IPv4 *temp1 = head->next;
	    IPv4 *temp2 = head->next->next;

		//Sort ascending
	    if(answer == 65){
		    while(ptr!= NULL){
		    	//Base case
			    if(temp1 == NULL || temp2 == NULL){
			        counter--;
			    	break;
			    }
			    //Create memory for swap and load it with data
			    IPv4 *swap = (IPv4*)malloc(sizeof(IPv4));
			    strcpy(swap->alias, temp1->alias);
			    swap->octet[0]= temp1->octet[0];
			    swap->octet[1]= temp1->octet[1];
			    swap->octet[2]= temp1->octet[2];
			    swap->octet[3]= temp1->octet[3];

			    //Sort in ascending values
			    if(strcmp(temp1->alias ,temp2->alias) > 0){
			        swap->next = temp2->next;
			    	ptr->next = temp2;
			    	temp2->next =swap;
		    	}
			        
			    //Sort head and head.next if plausable    
			    else if(strcmp(ptr->alias ,temp1->alias) > 0){
                    ptr->next = ptr->next->next;
                    swap->next = ptr;
                    head = swap;
                    counter--;
                    break;
			    }
			    
				//Move pointers over by one
		    	ptr=ptr->next;
			    temp1=ptr->next;
			    temp2=ptr->next->next;
	    	}
	    }
	    //Sort descending
	    else if(answer == 68){
		    while(ptr!= NULL){
		    	//Base case
			    if(temp1 == NULL || temp2 == NULL){
			        counter--;
			    	break;
			    }
			    //Create memory for swap and load it with data
			    IPv4 *swap = (IPv4*)malloc(sizeof(IPv4));
			    strcpy(swap->alias, temp1->alias);
			    swap->octet[0]= temp1->octet[0];
			    swap->octet[1]= temp1->octet[1];
			    swap->octet[2]= temp1->octet[2];
			    swap->octet[3]= temp1->octet[3];

			    //Sort in ascending values
			    if(strcmp(temp1->alias ,temp2->alias) < 0){
			        swap->next = temp2->next;
			    	ptr->next = temp2;
			    	temp2->next =swap;
		    	}
			        
			    //Sort head and head.next if plausable    
			    else if(strcmp(ptr->alias ,temp1->alias) < 0){
                    ptr->next = ptr->next->next;
                    swap->next = ptr;
                    head = swap;
                    counter--;
                    break;
			    }
			    
				//Move pointers over by one
		    	ptr=ptr->next;
			    temp1=ptr->next;
			    temp2=ptr->next->next;
	    	}
	    }
    }	
	return;
}






	
	
