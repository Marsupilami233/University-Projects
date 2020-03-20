/* task_list.c
 * Yulin Xiao 
 * Yulinx         
 * ECE 2230, Spring 2018
 * MP1
 * 
 * Purpose: This Mashine Problem 1 Focus On the tasks that are executing in parallel and 
 * prompt user to construct, destruct, insert, remove, schedule the QUEUED and BLOCKED 
 * tasks and have access to certain tasks blocks to see if there is identical data.
 *
 * Assumptions: Using the ADT and structure pointer to indirectly manipulate the task block
 * task_t and to do basic sequential list operation, ie. insert, delete, construct using 
 * another pointer, destruct the structure of outer pointer, get access to specific blocks
 * and basics of finding and locate the blocks in dynamic memory. 
 *
 * Bugs: None that I know.  
 * 
 * 
 * Must include a brief comment block at the top of each function
 * and structure definition stating its purpose:
 * task_count is the count , list_size is the size, should not mess up with these.        
 * For function, you must define the purpose of each input and the 
 * values that are valid for the inputs
 * 

 * you must also define the results that are returned, and provide
 * comments about any data structures that are changed by the function
 * Inline comments are always used whenever necessary to explain critical 
 * portions of the code sparingly.
 */


#include <stdlib.h>
#include <assert.h>

#include "task_list.h"
#define TRUE 1
#define FALSE 0

/* The malloc must not be used, because it have many uninitialized blocks
 */
struct task_list_t *task_list_construct(int size)
{
    
    struct task_list_t* list = NULL; // header pointer for all tasks, initialize to NULL 
    
    list = (struct task_list_t *) calloc(1, sizeof(struct task_list_t));// alloc mem for list
	
	list->task_ptr = (struct task_t **) calloc(size, sizeof(struct task_t *));// the same

	list->task_size = size; //size is the capacity of the sequential list
			
 	for (int i = 0; i < size; i ++) {// alloc for array pointers point to memory block
 	
 		list->task_ptr[i] = NULL; // Initializing the pointer in array to NULL
 		
	}
 	    
    return list; // return pointer
}

/* Free ONLY the list_ptr and the task_ptr array!
 */
void task_list_destruct(struct task_list_t *list_ptr)
{
    assert(list_ptr != NULL && "TASK LIST NULL IN DESTRUCT");
    
	free(list_ptr->task_ptr); // free the task_t double pointer points to pointer arrays
 
    free(list_ptr); // free the pointer my_list

}

/* Current length of task list, but not the whole list
 */
int task_list_number_entries(struct task_list_t *list_ptr) //current length is the task count
{
    assert(list_ptr != NULL); 

    return list_ptr->task_count; // return the size of the pointer 
}

/* First we need to reference InsertLastNode, then we will have to insert and connnect
 * Each node, final we have to consider the NULL, empty, and lastIdx 
 */
int task_list_add(struct task_list_t *list_ptr, struct task_t *rec_ptr)                     
                                                         

{
    assert(list_ptr != NULL);
    
    int lastIdx = list_ptr->task_count - 1;  // newest -> inserted as the lastIndex
    
	if (lastIdx < list_ptr->task_size - 1) { // if there is no elements and within the boundary
	   
		list_ptr->task_ptr[lastIdx + 1] = rec_ptr; // copy the content to the mem point to 	
		list_ptr->task_count ++;
		
	    return 1; // Insert successful                                                     
	} 
	
	return -1;// Insert failed
}

/* We just need to return the pointer that we want
 */ 
struct task_t *task_list_access(struct task_list_t *list_ptr, int index)
{
    assert(list_ptr != NULL && "TASK LIST NULL IN ACCESS\n");
    
    if (index < list_ptr->task_size) { 
/*task_id is the unique task ID in the list, which is the most important thing to find the priority*/
    	return list_ptr->task_ptr[index];
    } 
     
    return NULL;
}
/*Return the task_t pointers that includes the removed blocks*/
struct task_t *task_list_remove(struct task_list_t *list_ptr, int index) 
{
    assert(list_ptr != NULL && "LIST NULL IN REMOVE\n");
    struct task_t* task = NULL;
    
    if (index <= list_ptr->task_count - 1) {  
                                                                                           
    	task = list_ptr->task_ptr[index];   // give the address to the task to avoid leak
    	list_ptr->task_ptr[index] = NULL; 
    	
    	if (index < list_ptr->task_count - 1) {
    	
    		while (list_ptr->task_ptr[index + 1] != NULL) {
    		
    			list_ptr->task_ptr[index] = list_ptr->task_ptr[index + 1];// cover from the front
    			index ++;                                                                 
    		}
    		list_ptr->task_ptr[index] = NULL; // the pointer of last index to be set to NULL 
    		
    	}                                                                                 
    	list_ptr->task_count --;   // nearly forget to minus the task_count
    	
    } else {
    
    	return NULL; // the controversy
    	
    }  
                          
    return task;
}


/*Once hit at the priority, it returns
 */
int task_list_lookup_first_priority(struct task_list_t *list_ptr, int priority)
{
    assert(list_ptr != NULL);
    
    int index = 0;
    for (index = 0; index < list_ptr->task_count; index ++) {
    
    	if (list_ptr->task_ptr[index]->priority == priority) {
    	
    		return index;
    	}
    }
   
    return -1;
}
/*Look up for the id that matches*/
int task_list_lookup_id(struct task_list_t *list_ptr, int task_id)
{
    assert(list_ptr != NULL);
    
    int index;
    for (index = 0; index < list_ptr->task_count; index ++) {
    
    	if (list_ptr->task_ptr[index]->task_id == task_id) {
    	
    		return index;
    	}
    }
    
    return -1;
}

/*Return the task_ptr[index] to one that priority that matches
 */
struct task_t *task_list_access_priority(struct task_list_t *list_ptr, int priority)
{
    assert(list_ptr != NULL && "TASK LIST NULL IN ACCESS\n");
    
    int index;
    for (index = 0; index < list_ptr->task_count; index ++) {
    
    	if (list_ptr->task_ptr[index]->priority == priority) {
    	
    		return list_ptr->task_ptr[index];
    	}
    }
    
    return NULL;// Not Found
}

/*Remove from task_list_t and change the poiner pointing direction
 */
struct task_t *task_list_remove_priority(struct task_list_t *list_ptr, int priority)      
{
    assert(list_ptr != NULL && "LIST NULL IN REMOVE\n");
    struct task_t* task = NULL;
    
    int index;
    for (index = 0; index < list_ptr->task_count; index ++) {
    
    	if (list_ptr->task_ptr[index]->priority == priority) {
    	
    		break; // find the first memory block from the list with a matching priority
    	}
    }
    if (index <= list_ptr->task_count - 1) {  
                                                                                           
    	task = list_ptr->task_ptr[index];   // give the address to the task to avoid leak
    	list_ptr->task_ptr[index] = NULL; 
    	
    	if (index < list_ptr->task_count - 1) {
    	
    		while (list_ptr->task_ptr[index + 1] != NULL) {
    		
    			list_ptr->task_ptr[index] = list_ptr->task_ptr[index + 1];// cover from the front
    		}
    		list_ptr->task_ptr[index] = NULL; // the pointer of last index to be set to NULL 
    		
    	}
    	list_ptr->task_count --; // nearly forget to minus the task_count
    	
    } else {
    
    	return NULL; // the controversy
    	
    } 
    return task;
}
/*Find the id that matchs
 */
struct task_t *task_list_access_id(struct task_list_t *list_ptr, int id)
{
    assert(list_ptr != NULL && "TASK LIST NULL IN ACCESS\n");
    int index;
    for (index = 0; index < list_ptr->task_count; index ++) {
    	if (id == list_ptr->task_ptr[index]->task_id) {
    		return list_ptr->task_ptr[index];
    	}
    }
    
    return NULL;
}
/*First use the for loop to find the id that is requered for remove
 */
struct task_t *task_list_remove_id(struct task_list_t *list_ptr, int id)
{
    assert(list_ptr != NULL && "LIST NULL IN REMOVE\n");
    struct task_t* task = NULL;
    int index;
    for (index = 0; index < list_ptr->task_count; index ++) {
    	if (id == list_ptr->task_ptr[index]->task_id) {
    		break;
    	}
    }
     if (index <= list_ptr->task_count - 1) {  
                                                                                           
    	task = list_ptr->task_ptr[index];   // give the address to the task to avoid leak
    	list_ptr->task_ptr[index] = NULL; 
    	
    	if (index < list_ptr->task_count - 1) {
    	
    		while (list_ptr->task_ptr[index + 1] != NULL) {
    			list_ptr->task_ptr[index] = list_ptr->task_ptr[index + 1];// cover from the front
    		}
    		list_ptr->task_ptr[index] = NULL; // the pointer of last index to be set to NULL 
    		
    	}
    	list_ptr->task_count ++; // nearly forget to minus the task_count
    	
    } else {
    
    	return NULL; // the controversy
    	
    } 
    return task;
}
/*Just find the QUEUED and BLOCKED tasks*/
int task_list_determine_runable(struct task_list_t *list_ptr, int nargs, int *args)
{
    // first we should have access to the specific args[0],args[1],args[2] as index
    // then we should determine the times we determine runable
    // next we should access the QUEUED or BLOCKED things
    // if the same, we agree, if no agree, return -1
    int j = 0;
    for (int i = 0; i < nargs; i ++) {
    
    	if (list_ptr->task_ptr[args[i]]->state == 0 || list_ptr->task_ptr[args[i]]->state == 2) {
    	
    		while(j < nargs){
    		
    			if (args[i] == args[j]) {
    			
    				j ++;
    				return i;	 
    			} 
    		} 
    	}
    }	 
    return -1;
}
 
/*Just find the state that meets the need

 */
void task_list_set_state(struct task_list_t *list_ptr, int id, enum state state)
{
	int index;
	for (index = 0; index < list_ptr->task_count; index ++) {
	
		if (list_ptr->task_ptr[index]->task_id == id)
		
		list_ptr->task_ptr[index]->state = state;
		
	}
	
}
/*Remove the FINSHED*/
struct task_list_t* task_list_remove_finished(struct task_list_t *list_ptr)
{
    struct task_list_t* rm = task_list_construct(list_ptr->task_size);
    
    int offset = 0;
    int index;
    
    for (index = 0; index < list_ptr->task_count; index ++) {
    
    	if ((enum state) (list_ptr->task_ptr[index]->state) == FINISHED) {
    	
    		rm->task_ptr[offset] = list_ptr->task_ptr[index]; // point at the blocks that should be removed
    		
    		if (index <= list_ptr->task_count - 1) {  
                                                                                           
    			   // give the address to the task to avoid leak
    			list_ptr->task_ptr[index] = NULL; 
    	
    			if (index < list_ptr->task_count - 1) {
    	
    				while (list_ptr->task_ptr[index + 1] != NULL) {
    				
    					list_ptr->task_ptr[index] = list_ptr->task_ptr[index + 1];// cover from the front
    				}
    				list_ptr->task_ptr[index] = NULL; // the pointer of last index to be set to NULL 
    		
    			}
    	        
    		}
    		offset ++;
    	}
    }

    return rm; 
}
/*Schedule the task_id valid and the priority that matches
 */
struct task_t* task_list_schedule(struct task_list_t *list_ptr, int priority, int id)
{
    struct task_t* task = NULL;
    int index; // index of tasks

    for (index = 0; index < list_ptr->task_count; index ++) {
    
    	if (list_ptr->task_ptr[index]->task_id == id && 
    	list_ptr->task_ptr[index]->task_id > 0) {
    	
    		if ((enum state) (list_ptr->task_ptr[index]->state) == QUEUED || 
    		(enum state) (list_ptr->task_ptr[index]->state) == BLOCKED) {
    		
    			list_ptr->task_ptr[index]->state = 1;
    			task = list_ptr->task_ptr[index];
    			
    			return task;
    		}
    	} else if (list_ptr->task_ptr[index]->priority == priority) {
    		if ((enum state) (list_ptr->task_ptr[index]->state) == QUEUED || 
    		(enum state) (list_ptr->task_ptr[index]->state) == BLOCKED) {
    		
    			list_ptr->task_ptr[index]->state = 1;
    			task = list_ptr->task_ptr[index];
    			
    			return task;
    		}
    	} 
    }
    
    return NULL;
}
