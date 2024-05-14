// Asad Zangi - 2546042 //

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


//DO NOT CHANGE DATA STRUCTURE
struct Customer {
    char name[81];
    int transactions;
    int items_purchased;
    float amount_paid;
};

typedef struct Customer customer;

//DO NOT CHANGE FUNCTION PROTOTYPES
void menu();
void printCustomers(customer *, int);
int countCustomers(FILE*);
void readTransactions(FILE*, customer* ,int);
customer* createHashTable();
customer* addCustomer(customer*, customer, int, int*, int);
customer* rehash(customer*, int*, int);
void printTable(customer*, int);
void searchTable(customer*, int, char[], int);

//DO NOT CHANGE THE main FUNCTION
int main() {

    FILE *inFile;
    int no_of_customers = 0;
    customer *customers;
    customer *hashTable;

    inFile = fopen("transactions.txt", "r");
    if (inFile == NULL){
        printf("File could not be opened.\n");
        exit(1);
    }
    no_of_customers = countCustomers(inFile);
    printf("There are %d unique customers\n", no_of_customers);

    customers = (customer*) malloc(no_of_customers * sizeof (customer));
    fclose(inFile);
    inFile = fopen("transactions.txt", "r");
    if (inFile == NULL){
        printf("File could not be opened.\n");
        exit(1);
    }

    readTransactions(inFile, customers, no_of_customers);
    if (customers == NULL) {
        printf("\nCustomers have NOT been read successfully!\n");
        exit(1);
    }
    printCustomers(customers, no_of_customers);

    hashTable = createHashTable();
    int hashTableSize = 11;
    char criteria;

    printf("Enter your hashing criteria: ");
    fflush(stdin);
    scanf("%c", &criteria);

    while (criteria != '1' && criteria != '2' && criteria != '3'){
        printf("Error! Please enter a valid hashing criteria: ");
        fflush(stdin);
        scanf("%c", &criteria);
    }
    fflush(stdin);

    int i;
    for (i = 0; i < no_of_customers; i++){
        hashTable = addCustomer(hashTable, customers[i], i, &hashTableSize, criteria - '0');
        printf("%s has been added to the table, the hash table now can be seen below:", customers[i].name);
        printTable(hashTable, hashTableSize);
    }

    char command = 0;
    int exit = 0;
    char searchName[81];
    while (exit != 1) {
        menu();
        printf("\nCommand: ");
        fflush(stdin);
        scanf("%c", &command);

        if (command == '1') {
            printf("Enter the customer's name: ");
            fflush(stdin);
            scanf("%s", searchName);
            searchTable(hashTable, hashTableSize, searchName, criteria - '0');
        }
        else if (command == '2'){
            printTable(hashTable, hashTableSize);
        }
        else if (command == '3'){
            exit = 1;
            printf("Goodbye!\n");
        }
        else{
            printf("Please enter a valid command!\n");
        }
    }
    free(customers);
    free(hashTable);
    return 0;
}

//DO NOT CHANGE THE menu FUNCTION
void menu () {
    printf("Please choose one of the following options:\n"
           "(1) Search a customer\n"
           "(2) Display hashtable\n"
           "(3) Exit\n");
}

//DO NOT CHANGE THE printCustomers FUNCTION
void printCustomers (customer* customers, int no_of_customers) {

    printf("List of customers:\n");
    int i;
    for (i = 0; i < no_of_customers; ++i) {
        printf("Name: %s, number of transactions = %d, number of items = %d, total amount paid = %.2f,"
               " average amount paid per transaction = %.2f\n",
               customers[i].name, customers[i].transactions, customers[i].items_purchased, customers[i].amount_paid,
               customers[i].amount_paid/customers[i].transactions);
    }

}

//DO NOT CHANGE THE countCustomers FUNCTION
int countCustomers (FILE *inFile){
    int no_of_customers = 0;
    char **names;
    char **temp;
    int i;
    char userName[100];
    int transactionId;
    char transactionTime[100];
    char itemDescription[100];
    int numberOfItemsPurchased;
    float costPerItem;
    char country[100];
    int exists = 0;
    char c;
    while ((c = fgetc(inFile)) != EOF) {
        if (c == '\n') {
            break;
        }
    }
    fscanf(inFile, "%[^;];%d;%[^;];%[^;];%d;%f;%[^\n]\n",
           userName, &transactionId, transactionTime,  itemDescription,
           &numberOfItemsPurchased, &costPerItem, country);
    no_of_customers++;
    names = (char**) malloc (sizeof(char*) * (no_of_customers));
    names[no_of_customers-1] = (char*) malloc(100 * sizeof (char));
    strcpy(names[no_of_customers - 1], userName);

    while (fscanf(inFile, "%[^;];%d;%[^;];%[^;];%d;%f;%[^\n]\n",
                  userName, &transactionId, transactionTime, itemDescription,
                  &numberOfItemsPurchased, &costPerItem, country) != EOF) {
        exists = 0;
        for (i = 0; i < no_of_customers; ++i) {
            if (strncmp(names[i], userName, strlen(userName)) == 0){
                exists = 1;
                break;
            }
        }
        if (exists)
            continue;
        temp = (char**) malloc (sizeof(char*) * (no_of_customers));
        for (i = 0; i < no_of_customers; ++i) {
            temp[i] = (char*) malloc(100 * sizeof (char));
            strcpy(temp[i], names[i]);
        }
        free(names);

        names = (char**) malloc (sizeof(char*) * (no_of_customers + 1));
        for (i = 0; i < no_of_customers; ++i) {
            names[i] = (char*) malloc(100 * sizeof (char));
            strcpy(names[i], temp[i]);
        }
        free(temp);
        names[no_of_customers] = (char*) malloc(100 * sizeof (char));
        strcpy(names[no_of_customers], userName);
        no_of_customers++;

    }
    return no_of_customers;
}

//DO NOT CHANGE THE readTransactions FUNCTION
void readTransactions (FILE* inFile, customer* customers, int no_of_customers) {
    char userName[100];
    int transactionId;
    char transactionTime[100];
    char itemDescription[100];
    int numberOfItemsPurchased;
    float costPerItem;
    char country[100];
    int exists = 0;
    int i;
    int filled = 0;
    char c;

    for (i = 0; i < no_of_customers; ++i) {
        strcpy(customers[i].name, "unassigned");
        customers[i].amount_paid = 0;
        customers[i].items_purchased = 0;
        customers[i].transactions = 0;
    }

    while ((c = fgetc(inFile)) != EOF) {
        if (c == '\n') {
            break;
        }
    }

    while (fscanf(inFile, "%[^;];%d;%[^;];%[^;];%d;%f;%[^\n]\n",
                  userName, &transactionId, transactionTime, itemDescription,
                  &numberOfItemsPurchased, &costPerItem, country) != EOF) {
        exists = 0; // n
        int i; // n
        for (i = 0; i < no_of_customers; ++i) {
            if(strcmp(userName, customers[i].name) == 0){
                exists = 1;
                break;
            }
        }
        if (exists){
            customers[i].transactions ++;
            customers[i].items_purchased += numberOfItemsPurchased;
            customers[i].amount_paid += (numberOfItemsPurchased * costPerItem);
        }
        else{
            strcpy(customers[filled].name, userName);
            customers[filled].transactions ++;
            customers[filled].items_purchased += numberOfItemsPurchased;
            customers[filled].amount_paid += (numberOfItemsPurchased * costPerItem);
            filled++;
        }
    }
}

customer* createHashTable(){
    customer *hashTable;
    int i;
    //Memory Allocation for hashTable;
    hashTable = malloc(sizeof(customer) * 11);
    if(hashTable == NULL){
    	printf("Allocation of memory Failed");
    	exit(1);
	}
	
	//Assigning float and integer with zero and string with unassigned;
	for (i = 0; i < 11; ++i) {

        strcpy(hashTable[i].name, "unassigned");
        hashTable[i].amount_paid = 0;
        hashTable[i].items_purchased = 0;
        hashTable[i].transactions = 0;
    }
	//returning the created hashTable to main menu;
 return hashTable;

}

customer* addCustomer(customer* hashTable, customer addedCustomer, int hashTableCapacity, int *hashTableSize, int criteria){

    // WRITE YOUR CODE HERE
    int key;
    
    float load_factor;
    int i,HashIndex;
    int empty_space =1;
    //Finding key of customer name by subtracting first and last letter ascii; 
    key = tolower(addedCustomer.name[strlen(addedCustomer.name) - 1]) - tolower(addedCustomer.name[0]);
    if(key < 0){
        key = -key;
    }
    //printf("key %d: ", key);
    
	//This for loop will help to find the correct hashindex and insert the data in hashTable;
    for(i=0;i<*hashTableSize; i++){
    //Finding the load factor;
	load_factor = (hashTableCapacity+1) / (float)*hashTableSize;
	//Incase, load factor is greater than 0.5 then progran will apply rehash to the old hashtable;
    if(load_factor>0.5){
         hashTable = rehash(hashTable, hashTableSize, criteria);
        }
	//Applying Linear Probing;
    if(criteria == 1){
    	//Linear probing equation to find the correct index to store the data;
        HashIndex =(key + i) % *hashTableSize;
        //If that particular index is empty then program will copy the data there;
        if(strcmp(hashTable[HashIndex].name, "unassigned") == 0){
            hashTable[HashIndex].amount_paid = addedCustomer.amount_paid;
            hashTable[HashIndex].items_purchased = addedCustomer.items_purchased;
            hashTable[HashIndex].transactions = addedCustomer.transactions;
            strcpy(hashTable[HashIndex].name, addedCustomer.name);
			//Return back the edited hashTable to maion menu;
            return hashTable;
        }

        } // End of Criteria 1;

		//Applying  quadratic probing;
        else if(criteria == 2){
        	//quadratic probing equation to find the correct index to store the data;
            HashIndex = (key + i * i) % *hashTableSize;
            //If that particular index is empty then program will copy the data there;
            if(strcmp(hashTable[HashIndex].name, "unassigned") == 0){
                hashTable[HashIndex].amount_paid = addedCustomer.amount_paid;
                hashTable[HashIndex].items_purchased = addedCustomer.items_purchased;
                hashTable[HashIndex].transactions = addedCustomer.transactions;
                strcpy(hashTable[HashIndex].name, addedCustomer.name);
                //Return back the edited hashTable to maion menu;
                return hashTable;
            }

        } // End of Criteria 2;

		//Applying  double hashing;
        else if(criteria == 3){
	
			//double hashing equation to find the correct index to store the data;
            HashIndex = (key + i * (7 - key%7) ) % *hashTableSize;
            //If that particular index is empty then program will copy the data there;
            if(strcmp(hashTable[HashIndex].name, "unassigned") == 0){
                hashTable[HashIndex].amount_paid = addedCustomer.amount_paid;
                hashTable[HashIndex].items_purchased = addedCustomer.items_purchased;
                hashTable[HashIndex].transactions = addedCustomer.transactions;
                strcpy(hashTable[HashIndex].name, addedCustomer.name);
                //Return back the edited hashTable to maion menu;
                return hashTable;
            }

        } // End of Criteria 3;



    } // End of for Loop


}

customer* rehash(customer* hashTable, int *hashTableSize, int criteria){

    // WRITE YOUR CODE HERE
    //Doubling the size of hashtable;
    int newsize = *hashTableSize * 2;
    int R,stop=0;
    int oldtablesize,HashIndex,key,copied = 0;

    oldtablesize = *hashTableSize;
    //Declaring the new hashTable;
    customer *newhashtable;
    //This particular program will find the nearest and greater prime number than new hash table size;
    int i,j,k,prime=1,count;
    for(i=3;i<2*newsize; i++){
        j=i;
        for(k=2;k<j;k++){
            if(i%k == 0){
                prime = 0;
            }
        }
        if(prime == 1 && j>newsize){
            if(stop == 0){
            //This is going to store the new size of hashTable;
            *hashTableSize = j;
            stop=1;
            }
        }
        prime=1;


    }

	//Allocating memory for new hash Table;
    newhashtable = malloc(sizeof(customer) * *hashTableSize);
    if(newhashtable == NULL){
    	printf("Allocation of memory Failed");
    	exit(1);
	}
	//Assigning float and integer with zero and string with unassigned;
	for (i = 0; i < *hashTableSize; ++i) {
        strcpy(newhashtable[i].name, "unassigned");
        newhashtable[i].amount_paid = 0;
        newhashtable[i].items_purchased = 0;
        newhashtable[i].transactions = 0;
    }
    //printf("new size %d %d\n", *hashTableSize,i);

	//This particular program will copy the data from old hashTable;
	//And find the best place where to store it according to all three criterias
	
    for(i=0;i<oldtablesize;i++){
        for(j=0;j<*hashTableSize;j++){
        	//Finding key of customer name by subtracting first and last letter ascii;
            key = tolower(hashTable[i].name[strlen(hashTable[i].name) - 1]) - tolower(hashTable[i].name[0]);
            if(key < 0){
                key = -key;
            }
            //This if statement means that old hash table have customer data at index i;
            if (strcmp(hashTable[i].name, "unassigned") != 0 ) {

            //printf("new size %d %d\n", *hashTableSize,i);
                if(copied == 0){
                	//Applying Linear Probing;
                    if(criteria == 1){
                    	//Linear probing equation to find the correct index to store the data;
                        HashIndex =(key + j) % *hashTableSize;
                     	//If that particular index is empty then program will copy the data there;
                        if(strcmp(newhashtable[HashIndex].name, "unassigned") == 0){

                            newhashtable[HashIndex].amount_paid = hashTable[i].amount_paid;
                            newhashtable[HashIndex].items_purchased = hashTable[i].items_purchased;
                            newhashtable[HashIndex].transactions = hashTable[i].transactions;
                            strcpy(newhashtable[HashIndex].name, hashTable[i].name);
                            copied =1;

                        }

                    } // End of Criteria 1;

					//Applying  quadratic probing;
                    else if(criteria == 2){
                    	//quadratic probing equation to find the correct index to store the data;
                        HashIndex = (key + (j * j)) % *hashTableSize;
                        //If that particular index is empty then program will copy the data there;
                        if(strcmp(newhashtable[HashIndex].name, "unassigned") == 0){
                            printf("I came here %d %s\n",HashIndex,hashTable[i].name);
                            newhashtable[HashIndex].amount_paid = hashTable[i].amount_paid;
                            newhashtable[HashIndex].items_purchased = hashTable[i].items_purchased;
                            newhashtable[HashIndex].transactions = hashTable[i].transactions;
                            strcpy(newhashtable[HashIndex].name, hashTable[i].name);
                            copied =1;


                        }

                    } // End of Criteria 2;
					//Applying  double hashing;
                    else if(criteria == 3){
                    	//double hashing equation to find the correct index to store the data;
                        HashIndex = (key + (j * (7- (key%7)))) % *hashTableSize;
                        //If that particular index is empty then program will copy the data there;
                        if(strcmp(newhashtable[HashIndex].name, "unassigned") == 0){
                           
                           newhashtable[HashIndex].amount_paid = hashTable[i].amount_paid;
                            newhashtable[HashIndex].items_purchased = hashTable[i].items_purchased;
                            newhashtable[HashIndex].transactions = hashTable[i].transactions;
                            strcpy(newhashtable[HashIndex].name, hashTable[i].name);
                            copied =1;


                        }
            }
            } // end of if copied == 0 condition
        } //end of if statement inside the nested for loop

    } // end of second for loop

        copied =0; //reseting the value for copied;

}// End of first for loop

 //Destroying the old hashTable;
 free(hashTable);
 //Returing the new hashTable to the addCustomer function;
 return newhashtable;
}

void printTable(customer *hashTable, int hashTableSize){

	int i;
    printf("\n======================== Hash Table ===========================\n");
    
    printf("Index\t\tName\t\tTransactions\t\t\tItems\t\t\tPaid_Amount\n");
    for (i = 0; i < hashTableSize; i++) {
    	if(strcmp(hashTable[i].name, "unassigned") != 0){
    		printf("%d\t\t%s\t\t%d\t\t\t\t%d\t\t\t%.2f\n", i, hashTable[i].name, hashTable[i].transactions, hashTable[i].items_purchased, hashTable[i].amount_paid);
		}
		else{
			printf("%d\n", i);

		}
    }
    printf("===============================================================\n");
}

void searchTable(customer *hashTable, int hashTableSize, char searchName[], int criteria){

    // WRITE YOUR CODE HERE
    int i;
    int key,found=0,findindex,exit=0,HashIndex;
    //Finding key of csearchName by subtracting first and last letter ascii;
    key = tolower(searchName[strlen(searchName) - 1]) - tolower(searchName[0]);
    if(key < 0){
        key = -key;
    }
    
    for(i=0;i<hashTableSize;i++){
	    if(exit == 0){
	    	//Linear Probing
	    	if(criteria == 1){
	    		//Equation for linear probing to find the correct index;
		        HashIndex =(key + i) % hashTableSize;
		        //If this if statement is true then user is available;
		        if(strcmp(hashTable[HashIndex].name, searchName) == 0){
		            findindex = HashIndex;
		            found=1;
					exit=1;
		        }
		        //if this statment is true then data is not available;
		        else if(strcmp(hashTable[HashIndex].name, "unassigned") == 0){
		        	found = 0;
		        	exit =1;
				}
			} // End of Criteria 1;
			
			//Quadratic Probing 
	        else if(criteria == 2){
	        	//Equation for quadratic probing to find the correct index;
	            HashIndex = (key + i * i) % hashTableSize;
	            //If this if statement is true then user is available;
	            if(strcmp(hashTable[HashIndex].name, searchName) == 0){
	            	findindex = HashIndex;
	            	found=1;
					exit=1;
	       		}
	       		//if this statment is true then data is not available;
	        	else if(strcmp(hashTable[HashIndex].name, "unassigned") == 0){
	        		found = 0;
	        		exit =1;
			}
	
	        } // End of Criteria 2;
			// double hashing
	        else if(criteria == 3){
	        	//Equation for double hashing to find the correct index;
				HashIndex = (key + i * (7 - key%7) ) % hashTableSize;
				//If this if statement is true then user is available;
	            if(strcmp(hashTable[HashIndex].name, searchName) == 0){
		            findindex = HashIndex;
		            found=1;
					exit=1;
	        }
	        //if this statment is true then data is not available;
	        	else if(strcmp(hashTable[HashIndex].name, "unassigned") == 0){
		        	found = 0;
		        	exit =1;
			}
	
	        } // End of Criteria 3;
	
	    	
		}
	}	// End of for loop;
	//Found= 0 means data is not available;
	if(found == 0){
		printf("User does not exist in the table!\n");
	}
	else if(found == 1){
		printf("Information for customer %s:\n",hashTable[findindex].name);
		printf("\tNumber of transactions = %d\n",hashTable[findindex].transactions);
		printf("\tItems purchased = %d\n",hashTable[findindex].items_purchased);
		printf("\tTotal Amount Paid = %.2f\n",hashTable[findindex].amount_paid);
	}
	
    
    

}
