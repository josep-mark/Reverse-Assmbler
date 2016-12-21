#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lc4_memory.h"
#define GET_SUB_OP(I) ((I & 0x0038) >> 3)
#define ADD_HEX 0x0000
#define SUB_HEX 0x0002
#define MUL_HEX 0x0001
#define DIV_HEX 0x0003
#define ADD_I_HEX 0x0004
#define OP_HEX  1
#define RD_VAL 0x0E00
#define RS_VAL 0x01C0
#define RT_VAL 0x0007
#define I_HEX 0x001f
//define constants and macros

/*Function to reverse assemble from machine language to ASM*/
/*Does so by searching for the given opcode, finding the memory pointer and updating assembly*/
int reverse_assemble (row_of_memory* memory) 
{
	short unsigned int opcode = OP_HEX;
	short unsigned int contents;
	short unsigned int sub_op;
	char * assembly;
	short unsigned int rd;
	short unsigned int rs;
	short unsigned int rt;
	short unsigned int op;

	row_of_memory * mem_ptr = memory;
	
	//While the mem_ptr is not null keep going
	while(mem_ptr){

		/*Search opcode using memory function, gets pointer to the first node with an empty label field matching opcode*/
		mem_ptr = search_opcode(mem_ptr, opcode);
		if (mem_ptr == NULL) {						//check if mem_ptr is null, if so break from loop
			break;
		}

		if(mem_ptr->contents){						//if mem_ptr contents 
			contents = mem_ptr->contents;			//get contents or machine language code
			sub_op = GET_SUB_OP(contents);			//get sub op using sub op macro
		}
		else{
			printf("No memory contents\n");			//check for errors getting sub op
			break;
		}

		if (mem_ptr == NULL){
			break;
		}
		else{


			if (sub_op == ADD_HEX){							//check sub op against add defined as a global
				assembly = malloc(sizeof(char)*18);			//create char array to hold assembly instruction
				contents = mem_ptr->contents;
				rd = (contents & RD_VAL) >> 9;				//find rd
				rs = (contents & RS_VAL) >> 6;				//find rs
				rt = contents & RT_VAL;						//find rt
				sprintf(assembly, "ADD R%d R%d R%d", rd, rs, rt);	//fill assembly instruction char array
				assembly[5] = rd;
				mem_ptr->assembly = assembly;				//assign assembly field

				if(mem_ptr->next){							//update mem_ptr location to re run search for opcode
					mem_ptr = mem_ptr->next;
				}
				else{
					break;
				}

				continue;
			}

			if (sub_op == SUB_HEX){						//check for subtraction sub op
				assembly = malloc(sizeof(char)*18);		//same methods as above
				contents = mem_ptr->contents;
				rd = (contents & RD_VAL) >> 9;
				rs = (contents & RS_VAL) >> 6;
				rt = contents & RT_VAL;
				sprintf(assembly, "SUB R%d R%d R%d", rd, rs, rt);
				mem_ptr->assembly = assembly;
				if(mem_ptr->next){
					mem_ptr = mem_ptr->next;
				}
				else{
					break;
				}

			}

			if (sub_op == MUL_HEX){					//check for multiply sub op
				assembly = malloc(sizeof(char)*18);	//follow same procedures
				contents = mem_ptr->contents;
				rd = (contents & RD_VAL) >> 9;
				rs = (contents & RS_VAL) >> 6;
				rt = contents & RT_VAL;
				sprintf(assembly, "MUL R%d R%d R%d", rd, rs, rt);
				mem_ptr->assembly = assembly;
				if(mem_ptr->next){
					mem_ptr = mem_ptr->next;
				}
				else{
					break;
				}

			}

			if (sub_op == DIV_HEX){						//check for divide sub op
				assembly = malloc(sizeof(char)*18);		//follow same procedure
				contents = mem_ptr->contents;
				rd = (contents & RD_VAL) >> 9;
				rs = (contents & RS_VAL) >> 6;
				rt = contents & RT_VAL;
				sprintf(assembly, "DIV R%d R%d R%d", rd, rs, rt);
				mem_ptr->assembly = assembly;
				if(mem_ptr->next){
					mem_ptr = mem_ptr->next;
				}
				else{
					break;
				}
			}

			// sub_op = ((contents & 0x0020) >> 3);

			if (sub_op == ADD_I_HEX){					//check for ass imm5 sub op
				assembly = malloc(sizeof(char)*18);		//follow the same procedure
				contents = mem_ptr->contents;
				rd = (contents & RD_VAL) >> 9;
				rs = (contents & RS_VAL) >> 6;
				rt = contents & I_HEX;
				sprintf(assembly, "ADD R%d R%d %d", rd, rs, rt);
				mem_ptr->assembly = assembly;
				if(mem_ptr->next){
					mem_ptr = mem_ptr->next;
				}
				else{
					break;
				}
			}

			if(mem_ptr->next){
					mem_ptr = mem_ptr->next;
			}
			else{
				break;
			}

			
		}
		
	
	}

	
	return 0 ;
}
