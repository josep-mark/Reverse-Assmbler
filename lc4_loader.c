#include <stdio.h>
#include <stdlib.h>
#include "lc4_memory.h"
 

/* declarations of functions that must defined in lc4_loader.c */

/*Function to open a file returns a pointer to a file*/
FILE* open_file(char* file_name)
{
	FILE * file_ptr; // create file pointer
	file_ptr = fopen(file_name, "rb"); 		//Open file passed in

	if (file_ptr == NULL) perror("Problem opening the file"); 	//Check for errors

	return file_ptr ;
}


/*function to parse an open file*/
/*Reads in the file a byte at a time*/
/*Gets header and determines next step*/
/*If code or data reads in the specified words and adds to the linked list*/
/*If symbol it finds the corresponding address reads in the character array and assigns the char array to the specified node*/
int parse_file (FILE* my_obj_file, row_of_memory** memory)
{
	/*Declare variables for use*/
	row_of_memory * memory_ptr = * memory;
	row_of_memory * test;
	int * first_header;
	int * content_holder;
	char * label;
	char * character;
	int address;
	int words;
	int contents;
	int count;
	int add;
	int header;
	int first_term;
	int second_term;
	int repeat = 1;

	/*While loop to keep reading the file until end of file*/
	while (repeat == 1){


		/*Get header from the file*/
		
		first_term = fgetc(my_obj_file);		//Get first byte
		if (first_term == EOF) {
			repeat = -1;
			break;
		}
		first_term = first_term << 8;			//Shift first term by 8 bits
		second_term = fgetc(my_obj_file);		//Get second byte
		header = first_term + second_term;		//combine the two bytes store in header


		if(header == 0xCADE){					//test if header is code

			first_term = fgetc(my_obj_file);	//if code read in address using same method as above
			first_term = first_term << 8;
			second_term = fgetc(my_obj_file);
			address = first_term + second_term;
			

			first_term = fgetc(my_obj_file);	//read in words to determine how many words in the code section
			first_term = first_term << 8;		//Use the same method as above
			second_term = fgetc(my_obj_file);
			words = first_term + second_term;
			

			count = 0;
			while (count < words){					//loop for the number of words in the section
				first_term = fgetc(my_obj_file);	//get one word at a time by reading two bytes, same method as above
				first_term = first_term << 8;
				second_term = fgetc(my_obj_file);
				contents = first_term + second_term;
				test = search_address(*memory, address);		//check if there is a node associated with this address
				if (test != NULL){
					test->contents = contents;					//if there is a node with the address update the contents
					address++;									//update address and count
					count++;
				}
				else{
					add = add_to_list(memory, address, contents);						//if there is no node with the address create a new node in the list
					if (add == -1) perror("There was an error adding to the list");		//check for errors
					address++;															//Update address and contents
					count++;
				}		
			}

		}

		/*If header is equal to data header*/
		if (header == 0xDADA){						
			
			first_term = fgetc(my_obj_file);		//same operation as for code above
			first_term = first_term << 8;
			second_term = fgetc(my_obj_file);
			address = first_term + second_term;

			first_term = fgetc(my_obj_file);
			first_term = first_term << 8;
			second_term = fgetc(my_obj_file);
			words = first_term + second_term;


			count = 0;
			while (count < words){
				first_term = fgetc(my_obj_file);
				first_term = first_term << 8;
				second_term = fgetc(my_obj_file);
				contents = first_term + second_term;
				test = search_address(*memory, address);
				if (test != NULL){
					test->contents = contents;
					address++;
					contents++;
				}
				else{
					add = add_to_list(memory, address, contents);
					if (add == -1) perror("There was an error adding to the list");
					address++;
					count++;
				}		
			}
		}

		/*Check if header is equal to symbol*/
		if (header == 0xc3b7){					

			first_term = fgetc(my_obj_file);		//get address using the same methods as before
			first_term = first_term << 8;
			second_term = fgetc(my_obj_file);
			address = first_term + second_term;

			first_term = fgetc(my_obj_file);		//get words using the same methods as before
			first_term = first_term << 8;
			second_term = fgetc(my_obj_file);
			words = first_term + second_term;

			count = 0;
			label = malloc(sizeof(char)*(words+1));		//create char array to hold the label, +1 to make room for the null character at the end of the array
			while(count < words){						//read in bytes for char array and store them in label array
				first_term = fgetc(my_obj_file);
				label[count] = first_term;				//label at count index equal to byte read in
				count++;								//update count
			}
			label[words] = '\0';								//append null character to the char array
			memory_ptr = search_address(*memory, address);		//find node with specified address
			if (memory_ptr != NULL){							//make sure mem-ptr exists
				memory_ptr->label = label;						//if it does exist update label field
			}
			else{
				add = add_to_list(memory, address, 0);			//if it does not exist create a new node with empty contents to hold label
				memory_ptr = search_address(*memory, address);

				if (memory_ptr == NULL){
					memory_ptr->label = label;
				}
				else{
					memory_ptr->label = label;
				}
			}

		}

	}

	/*Close file*/
	fclose(my_obj_file);				


	return 0 ;
}
