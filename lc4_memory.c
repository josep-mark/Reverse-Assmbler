#include <stdio.h>
#include <stdlib.h>
#include "lc4_memory.h"
#define SHIFT_ADD(I) (I << 8)
#define GET_SUB_OP(I) ((I & 0x0038) >> 3)	/* get opcode = 0001 or 0101 's sub_opcode */ /* 0000 0000 0011 1000 */
#define ADD_HEX 0x1018
#define SUB_HEX 0x1018
#define MUL_HEX 0x1018
#define DIV_HEX 0x1018
#define ADD_I_HEX 0x1018

/* 
 * adds a new node to the end of a linked list pointed to by head 
 */
int add_to_list (row_of_memory** head, short unsigned int address, short unsigned int contents) {

	/* allocate memory for a single node */
	row_of_memory * new_node = NULL;
	row_of_memory * head_node = *head;
	new_node = malloc(sizeof(row_of_memory));

	if(new_node == NULL) return -1;
	
	/* populate fields in newly allocated node w/ address&contents */
	new_node->address = address;
	new_node->contents = contents;
	
	/* if head==NULL, node created is the new head of the list! */
	if(head_node == NULL) {
		head_node = new_node;
		return 0;
	}

	/* otherwise, traverse linked list until we reach the end */
	while(head_node->next){
		head_node = head_node->next;
	}
	
	/* add node to the end of list */
	head_node->next = new_node;					//set head_node next to new node
	new_node->next = NULL;
	
	/* return 0 for success, -1 if malloc fails */

	return 0 ;
}


/* 
 * search linked list by address field, returns node if found 
 */
row_of_memory* search_address (row_of_memory* head, short unsigned int address ){
	/* traverse linked list, searching each node for "address"  */
	if (head == NULL) return NULL;
	
	/* return pointer to node in the list if item is found */
	while (head){
		if(head->address == address){
			return head;
		}
		if(head->next != NULL){
			head = head->next;
		}
		else{
			break;
		}
	}
	
	/* return NULL if list is empty or if "address" isn't found */
	 
	return NULL ;
}

/* 
 * search linked list by opcode field, returns node if found 
 */
row_of_memory* search_opcode  (row_of_memory* head, short unsigned int opcode  ) {
	/* traverse linked list until node is found with matching opcode
	   AND "assembly" field of node is empty */
	short unsigned int contents;
	short unsigned int test;
	row_of_memory * head_node = head;
	if(head_node == NULL) return NULL;
	
	while (head_node){
		 contents = head_node->contents;
		 test = (contents & 0xf000) >> 12;

		 if (test == opcode) {
	 		return head_node;
		}

		if(head_node->next != NULL){
			head_node = head_node->next;
		}		
		else{
			break;
		}
	}

	/* return pointer to node in the list if item is found */

	/* return NULL if list is empty or if no matching nodes */
	 
	return NULL ;
}


void print_list (row_of_memory* head ) 
{
	/* make sure head isn't NULL */
	if (head == NULL) printf("\tYour head is null\n");;

	/* print out a header */
	printf("\tYour Object File Contents:\n");
	printf("<label>\t\t<Address>\t<Contents>\t<Assembly>\n");
	
	while  (head){	
		printf("%s\t\t%x\t\t%x\t\t%s\n", head->label, head->address, head->contents, head->assembly);
		if (head->next){
		head = head->next;
		}
		else {
			break;
		}

	}

	/* traverse linked list, print contents of each node */	

	return ;
}

/* 
 * delete entire linked list
 */
int delete_list    (row_of_memory** head ) 
{
	/* delete entire list node by node */
	/* if no errors, set head = NULL upon deletion */

	row_of_memory * head_node = *head;
	row_of_memory * holder = NULL;


	if(head_node == NULL) return 0;


	// while(head_node){
	// 	free(holder);
	// 	holder = head_node;
	// 	if(head_node->next){
	// 		head_node = head_node->next;
	// 	}
	// 	else{
	// 		break;
	// 	}
	// }

	// free(holder);
	// free(head_node);

	while(head_node){
		holder = head_node;
		head_node = head_node->next;
		if (holder == NULL){
			break;
		}

		if(holder->label){
			free(holder->label);
		}

		if(holder->assembly){
			free(holder->assembly);
		}
		free(holder);

	}

	/* return 0 if no error, -1 for any errors that may arise */
	return 0 ;
}
