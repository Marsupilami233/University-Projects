/* 
 *
 *  Writing and debugging MP2 is challenging, and it is particularly hard if you
 *  try and write all the code before you do any testing. It is much easier to
 *  break the problem into small pieces and test each piece. 

 *	For MP2, one approach is to write list_insert first, since it might be the
 *	easiest algorithm and it depends only on list_construct.

 *  For testing, we use a simple main function in this file.  This file is for
 *  testing only, and you will NOT submit it as it is for your personal testing
 *  during development.
 *
 *  Below are a few simple examples that allocates a few structures and inserts
 *  them into a list.

 *  With this file containing a temporary main section, you can compile and run
 *  list.c on its own using 
 *  gcc -Wall -g testlist.c list.c task_list_support.c -o testlist
 *
 *  The compling command is also found in the provided makefile.  Do
 *  make testlist

 *  Do this testing before you make ANY changes to task_list_support.c.  The only 
 *  reason we need to compile task_list_support.c is for the task_list_compare function.
 *  The linker needs to find the comp_proc function and it is found in the template
 *  task_list_support.c file (with no changes) 

 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "datatypes.h"
#include "list.h"
#include "task_support.h"

/* prototype for list debug print */
void list_debug_print(list_t *list_ptr);

int main(void)
{
    data_t *datap = NULL;
    list_t *Lptr = NULL;

    Lptr = list_construct(task_compare, task_rec_cleanup);

    // create one item to test list_insert
    datap = (data_t *) calloc(1,sizeof(data_t));
    datap->task_id = 55;
    datap->priority = 0;
    list_insert_sorted(Lptr, datap);
    datap = NULL;

    // test list_access with one item in list
    datap = list_access(Lptr, LISTPOS_HEAD);
    printf("Should find 55 and found (%d)\n\n", datap->task_id);
    datap = NULL;

    // add a second item to tail of the list
    datap = (data_t *) calloc(1,sizeof(data_t));
    datap->task_id = 44;
    datap->priority = 1;
    list_insert_sorted(Lptr, datap);
    datap = NULL;

    // add a third item
    datap = (data_t *) calloc(1,sizeof(data_t));
    datap->task_id = 33;
    datap->priority = 2;
    list_insert_sorted(Lptr, datap);
    datap = NULL;
    // add a fourth item
    datap = (data_t *) calloc(1,sizeof(data_t));
    datap->task_id = 22;
    datap->priority = 3;
    list_insert_sorted(Lptr, datap);
    datap = NULL; 

    // find all three and print 
    datap = list_access(Lptr, 0); 
    printf("Second test\nPosition 0 should find 55 and found (%d)\n", datap->task_id);
    datap = list_access(Lptr, 1);
    printf("Position 1 should find 22 and found (%d)\n", datap->task_id);
    datap = list_access(Lptr, 2);
    printf("Position 2 should find 44 and found (%d)\n", datap->task_id);
    datap = list_access(Lptr, 3);
    printf("Position 3 should find 33 and found (%d)\n", datap->task_id);
    

    //Next try to use list_debug_print 
    printf("\nTest of list print\n\n");
    list_debug_print(Lptr);

	list_reverse(Lptr);
	printf("\nTest of list reverse\n");
	list_debug_print(Lptr);

    data_t template;
    int my_index = -999;
    template.task_id = 44;
    template.priority = 1;
    data_t *foundp = list_elem_find(Lptr, &template, &my_index);
    printf("\nTest of list elem find\n");
    if (foundp != NULL)
        printf("looked for %d and found %d at index %d\n",
            template.task_id, foundp->task_id, my_index);
    else
        printf("looked for %d and did not find \n", template.task_id);
    foundp = NULL;
    
    my_index = -999;
    template.task_id = 22;
    template.priority = 3;
    foundp = list_elem_find(Lptr, &template, &my_index);
    printf("\nTest of list elem find\n");
    if (foundp != NULL)
        printf("looked for %d and found %d at index %d\n",
            template.task_id, foundp->task_id, my_index);
    else
        printf("looked for %d and did not find \n", template.task_id);
    foundp = NULL;
    
    my_index = -999;
    template.task_id = 33;
    template.priority = 2;
    foundp = list_elem_find(Lptr, &template, &my_index);
    printf("\nTest of list elem find\n");
    if (foundp != NULL)
        printf("looked for %d and found %d at index %d\n",
            template.task_id, foundp->task_id, my_index);
    else
        printf("looked for %d and did not find \n", template.task_id);
    foundp = NULL;
    
    
    
    printf("\nTest of list remove\n");
    
	data_t *remove = list_remove(Lptr, LISTPOS_HEAD);
	if (remove != NULL)
		printf("Delete the node in pos %d:\n", remove->task_id);
	else
		printf("Delete Failed\n");
	
	list_debug_print(Lptr);
	
	
	remove = list_remove(Lptr, 0);
	if (remove != NULL)
		printf("Delete the node in pos %d:\n", remove->task_id);
	else
		printf("Delete Failed\n");	
		
	list_debug_print(Lptr);
		
	remove = list_remove(Lptr, 1);
	if (remove != NULL)
		printf("Delete the node in pos %d:\n", remove->task_id);
	else
		printf("Delete Failed\n");
		
	list_debug_print(Lptr);		
		
	remove = list_remove(Lptr, 2);
	if (remove != NULL)
		printf("Delete the node in pos %d:\n", remove->task_id);
	else
		printf("Delete Failed\n");	

    list_debug_print(Lptr);		
	
	
	remove = list_remove(Lptr, 3);
	if (remove != NULL)
		printf("Delete the node in pos %d:\n", remove->task_id);
	else
		printf("Delete Failed\n");
		
	list_debug_print(Lptr);	
		
	remove = list_remove(Lptr, 4);
	if (remove != NULL)
		printf("Delete the node in pos %d:\n", remove->task_id);
	else
		printf("Delete Failed\n");

	list_debug_print(Lptr);	
	assert(Lptr->head != NULL);
	
    list_destruct(Lptr);
 	free(datap);

    return 0;
}

/*

Next you will want to write your own list_debug_print function to print a
list. Then you can do "before and after" testing. That is, print the list
before a change, and print the list after the change to verify that the
change worked.

Something like

*/
//<--------------------------------------------------------------------------------------- this may be change with my own function, using 1hour to finish, or go out for a run for 15min
void list_debug_print(list_t *list_ptr) {
    list_node_t *L;
    int count;
	
    printf("List size: %d\n", list_ptr->current_list_size);
    L = list_ptr->head;
    count = 0;
    while(L != NULL) {
        printf("Pos: %d contains task id: %d\n, with priority %d\n", count, 
        L->data_ptr->task_id, L->data_ptr->priority);
        L = L->next;
        count++;
    }
    if (count != list_ptr->current_list_size) {
        printf("Hey!! the list size is wrong! count is %d\n", count);
    } 
    
    
    
}

