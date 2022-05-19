#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>


#include "ai.h"
#include "utils.h"
#include "list.h"
#include "hashtable.h"
#include "stack.h"


void copy_state(state_t* dst, state_t* src){
	
	//Copy field
	memcpy( dst->field, src->field, SIZE*SIZE*sizeof(int8_t) );

	dst->cursor = src->cursor;
	dst->selected = src->selected;
}

/**
 * Saves the path up to the node as the best solution found so far
*/
void save_solution( node_t* solution_node ){
	node_t* n = solution_node;
	while( n->parent != NULL ){
		copy_state( &(solution[n->depth]), &(n->state) );
		solution_moves[n->depth-1] = n->move;

		n = n->parent;
	}
	solution_size = solution_node->depth;
}


node_t* create_init_node( state_t* init_state ){
	node_t * new_n = (node_t *) malloc(sizeof(node_t));
	new_n->parent = NULL;	
	new_n->depth = 0;
	copy_state(&(new_n->state), init_state);
	return new_n;
}

/**
 * Apply an action to node n and return a new node resulting from executing the action
*/
node_t* applyAction(node_t* n, position_s* selected_peg, move_t action ){
	// printf("Apply action x:%d y:%d move:%d\n",selected_peg->x,selected_peg->y,action);
    node_t* new_node = (node_t*)malloc(sizeof(node_t));

	//FILL IN MISSING CODE
	//new node should be able to 
	//1. points to the parent
	new_node->parent = n;
	//2. updates the state with the action chosen
	for(int i=0;i<SIZE;i++) {
		for(int j=0;j<SIZE;j++) {
			new_node->state.field[i][j] = n->state.field[i][j];
		}
	}
	//3. updates the depth of the node
	new_node->depth = n->depth+1;
	//4. updates the action used to create the node 
	new_node->move = action;
	
    execute_move_t( &(new_node->state), selected_peg, action );
	
	return new_node;

}

/**
 * Find a solution path as per algorithm description in the handout
 */

void find_solution( state_t* init_state){

	HashTable table;

	
	
	// Choose initial capacity of PRIME NUMBER 
	// Specify the size of the keys and values you want to store once 
	ht_setup( &table, sizeof(char) * (SIZE*SIZE)+(SIZE+1), 2, 16769023);

	// Initialize Stack
	initialize_stack();

	//Add the initial node
	node_t* n = create_init_node( init_state );
	//FILL IN THE GRAPH ALGORITHM
	//Use DFS by using stack 
	stack_push(n);
	node_l* head_list = NULL;
	
	int remaining_peg= num_pegs(&(n->state));
	while(!is_stack_empty()){
		// printf("STAck:\n");
		n = stack_top();
		
		stack_pop();
		head_list = insertNode(head_list,n);
		expanded_nodes++;
		int noPegs = num_pegs(&(n->state));
		//Found a better solution
		if(noPegs< remaining_peg){
			save_solution(n);
			remaining_peg = noPegs;
		}
		
		for(int i=0;i<SIZE;i++) {
			for(int j=0;j<SIZE;j++) {
				position_s pos;
				pos.x = j;
				pos.y = i;
				for(int k=0;k<4;k++) {
					bool result  = can_apply(&(n->state), &pos, k);
					if(result) {
						//Create Child node
						node_t* new_node = applyAction(n,&pos, k);
						
						generated_nodes++;
						//Peg Solitaire solved
						if(won(&(new_node->state))){
							save_solution(new_node);
							remaining_peg = num_pegs(&(new_node->state));
						}
						
						//Create key for Hash Table using str representing the board state
						char *str = (char*) malloc(sizeof(char)*((SIZE*SIZE)+(SIZE+1)));
						int idx= 0;
							for(int i=0;i<SIZE;i++) {
								for(int j=0;j<SIZE;j++) {
									int8_t c  = new_node->state.field[i][j];
									if(c==111){
										str[idx++] = 'o';
									} else if(c==46) {
										str[idx++] = '.';
									} else {
										str[idx++] = ' ';
									}
								}
								str[idx++] = '\n';
							}
						str[idx]='\0';
						
						//Avoid duplicates: NewNode.state.board seen for the first time 
						if(!ht_contains(&table, str)){
								//Set the Value 
								char* val = (char*)malloc(sizeof(char)*2);
								val[0]='1';
								val[1]='\0';
								ht_insert(&table, str, val);
								stack_push(new_node);
								free(str);
								free(val);
						}else{
							//Free the new_node immediately if not used anymore.
							free(str);
							free(new_node);
						}
					}
				
					

				}
					
			}
		
		}
		//Budget exhausted
		if(expanded_nodes >= budget){
			break;
		}
		
	}
	
	//Free the n
	freeList(head_list);
	//Free memory space for Hash Table
	ht_destroy(&table);
}
