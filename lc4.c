#include <stdio.h>
#include <stdlib.h>
#include "lc4_memory.h"
#include "lc4_loader.h"
#include "lc4_disassembler.h"

/* program to mimic pennsim loader and disassemble object files */

int main (int argc, char** argv) {

	/**
	 * main() holds the linked list &
	 * only calls functions in other files 
	 */

	/* step 1: create head pointer to linked list: memory 	*/
	row_of_memory * memory = malloc(sizeof(row_of_memory));
	char * file_name = argv[argc-1];
	int run;
	int reverse;
	int delete;
	

	
	/* step 2: determine filename, then open it		*/
	/*   TODO: extract filename from argv, pass it to open_file() */
	FILE* my_file = open_file(file_name);


	/* step 3: call function: parse_file() in lc4_loader.c 	*/
	/*   TODO: call function & check for errors		*/
	run = parse_file(my_file, &memory);
	printf("Finished parse\n");

	/* step 4: call function: reverse_assemble() in lc4_disassembler.c */
	/*   TODO: call function & check for errors		*/
	reverse = reverse_assemble(memory);
	if (reverse == -1){
		perror("There was an error recerse assemling");
	}

	/* step 5: call function: print_list() in lc4_memory.c 	*/
	/*   TODO: call function 				*/
	print_list(memory);


	/* step 6: call function: delete_list() in lc4_memory.c */
	/*   TODO: call function & check for errors		*/
	delete = delete_list(&memory);
	if (delete == -1){
		perror("There was an error deleting the list");
	}

	/* only return 0 if everything works properly */
	return 0 ;
}
