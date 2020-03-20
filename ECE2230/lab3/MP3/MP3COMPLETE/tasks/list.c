/* list.c                   
 * Student: Yulin Xiao           
 * yulinx
 * ECE 2230 Spring 2019
 * MP3
 *
 * Propose: A complete form for list.c. You will make many changes. 
 *
 * Assumptions: Many details are complete, the user will use the sort types to sort the 
 * based on priority. To test them, first [ADDTAIL priority], then doing [SORT x
 * order], where [SORT x 1] sorts the queue in decending order based on the task's priority, 
 * SORT x 2 is the same except in accending order. Here x is one of the five sorting 
 * algorithms defined below. Finally, PRINTMP3 prints the newly formed queue.  
 *
 * Bugs: None that I know, but please notice again that when type INSERT command, do not enter negative value!
 *
 * The interface definition for the two-way linked list ADT is based on one 
 * defined by OPNET, Inc. http://www.opnet.com/
 */

#include <stdlib.h>
#include <assert.h>
//#include <stdio.h>

#include "datatypes.h"   /* defines data_t */
#include "list.h"        /* defines public functions for list ADT */

/* definitions for private constants used in list.c only */ 

#define LIST_SORTED_ASCENDING   -29634
#define LIST_SORTED_DESCENDING  -61801
#define LIST_UNSORTED           -72467

/* prototypes for private functions used in list.c only */
void list_debug_validate(list_t *L);
void list_bubble_sort(list_t** L, int sort_order);
void list_insertion_sort(list_t** L, int sort_order);
void list_recursive_selection_sort(list_t** L, int sort_order);
void list_selection_sort(list_t** L, int sort_order);
void list_merge_sort(list_t** L, int sort_order);

/* sorting helper functions) */

/* Sorts a list used a specific sorting algorithm and direction.
 * 
 * list_ptr: pointer to list-of-interest.  A pointer to an empty list is
 *           obtained from list_construct.
 *
 * sort_type: sorting algorithm used during sorting: 1) Bubble Sort; 
 *            2)Insertion Sort; 3) Recursive Selection Sort; 
 *            4) Iterative Selection Sort; and 5) Merge Sort
 *
 * sort_order: if 1 then the items are to be sorted in descending order.
 *             if 2 then the items are to be sorted in ascending order.
 *
 * return value: None.
 */
void list_sort(list_t **list_ptr, int sort_type, int sort_order)
{
    /* TODO: Complete the function sorting the list */


    
    if (sort_type == 1) {
        list_bubble_sort(&(*list_ptr), sort_order);
    } else if (sort_type == 2) {
        list_insertion_sort(&(*list_ptr), sort_order);
    } else if (sort_type == 3) {
        list_recursive_selection_sort(&(*list_ptr), sort_order);
    } else if (sort_type == 4) {
        list_selection_sort(&(*list_ptr), sort_order);
    } else if (sort_type == 5) {
        list_merge_sort(&(*list_ptr), sort_order);
    }
    if (sort_order == 2) {
        (*list_ptr)->list_sorted_state = LIST_SORTED_ASCENDING;// ? is ASCENDING or DESCENDING++++++++++++*/
    } else if (sort_order == 1) {
        (*list_ptr)->list_sorted_state = LIST_SORTED_DESCENDING;// ? is ASCENDING or DESCENDING++++++++++++*/
    }
}

/***********Swap Function Designed For Selection Sort and Bubble Sort*******************/

void swap(data_t** A, data_t** B) {
    data_t* temp = NULL;
    temp = *A;
    *A = *B;
    *B = temp;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/* Sorting the list via the insertion sort algorithm
 * 
 * L: pointer to list-of-interest.  
 *
 * sort_order: 1 sort list in descending order 2 sort in ascending order
 *
 * Rover list_node_t pointer: iterative pointer.
 *
 * swap function: swap the data_poiner directly using double pointer, but sending the data must use dependent list_node_t pointer!
 * 
 * Notice that to improve the efficiency, I start with the head when moving the largest to the back
 * and the tail when moving the smallest to the tail, just as in the worst cases. 
 */
void list_bubble_sort(list_t** L, int sort_order)
{
    /* TODO: see MP3 document for implementation notes */
    //assert(*L != NULL && sort_order != 0);

    list_node_t* Rover = NULL;// iterate the list
    int swapped = 1;

    if ((*L)->head == NULL || (*L)->tail == NULL || (*L)->head == (*L)->tail) {
        return;// if the list is empty or only one element or without any list, then terminate the function
    } else {
        if (sort_order == 1) {// descending order

            // put the largest to the back
            do {
                Rover = (*L)->head;// start from the head
                swapped = 0;
                while(Rover->next != NULL) {
                    if ((*L)->comp_proc(Rover->data_ptr,Rover->next->data_ptr) == 1) { //compare with the next one: if the element is smaller than the next one, swap

                        swap(&(Rover->data_ptr), &(Rover->next->data_ptr));
                        swapped = 1;
                    }
                    Rover = Rover->next;
                }

            } while(swapped);// outer loop to do the finding edge, if do not swapped, the loop terminates in advance! <--------problem is that you exit imdiately
                
        } else if (sort_order == 2) {// ascending order
                        
            // put the smallest to the back
            do {
                swapped = 0;
                Rover = (*L)->tail;
                while(Rover->prev != NULL) {
                    if ((*L)->comp_proc(Rover->data_ptr, Rover->prev->data_ptr) == 1) {// compare with the prev one: if the element is smaller than the prev one, swap

                        swap(&(Rover->data_ptr), &(Rover->prev->data_ptr));
                        swapped = 1;
                    }
                    Rover = Rover->prev;
                }

            } while(swapped);
            
        }
    }
}

/* Sorting the list via the insertion sort algorithm
 * 
 * L: pointer to list-of-interest.  
 *
 * sort_order: 1 sort list in descending order 2 sort in ascending order
 *
 * Pre-condition: Unsorted
 *
 * Post-condition: The EST data is swapped to the beginning and considered as sorted
 * and iterate from the data after the sorted element  
 */

void list_insertion_sort(list_t** L, int sort_order)
{
    //assert(*L != NULL && sort_order != 0);
    data_t* rec_ptr = NULL;
    list_t* L_sorted = NULL;
    if ((*L)->head == NULL || (*L)->tail == NULL || (*L)->head == (*L)->tail) {
        return;
    } else {
        if (sort_order == 1) {
            L_sorted = list_construct((*L)->comp_proc, (*L)->data_clean); // Set up a new List_t
            L_sorted->list_sorted_state = LIST_SORTED_DESCENDING;
            while ((*L)->head != NULL) {
            
                rec_ptr = list_remove(*L, LISTPOS_HEAD);// Just remove from the Head first and insert it in the sequence
                list_insert_sorted(L_sorted, rec_ptr);
            }
                 
            list_destruct(*L);
            *L = L_sorted;
        } else if (sort_order == 2) {
            L_sorted = list_construct((*L)->comp_proc, (*L)->data_clean);// same
            L_sorted->list_sorted_state = LIST_SORTED_ASCENDING;
            while ((*L)->head != NULL) {
            
                rec_ptr = list_remove(*L, LISTPOS_HEAD);//same
                list_insert_sorted(L_sorted, rec_ptr);
            }
            list_destruct(*L);     //delete the original function
        
            *L = L_sorted;            
        }
    }
}
/* +++++++++FindMax, FindMin Function for Auxiliary function of Selection Sort+++++++++ 
 * EST: the data_ptr that points to either largEST or smallEST data
 */

list_node_t* FindMax(list_t* L, list_node_t* First, list_node_t* Last) {
    
    list_node_t* Rover = First;
    list_node_t* EST = First;
    
    do {
        
        Rover = Rover->next;
        if(L->comp_proc(Rover->data_ptr, EST->data_ptr) == -1) {
            EST = Rover;//Iteration -> always store the biggest Item
        }
        
    } while(Rover != Last);
    return EST;
    
    
}

list_node_t* FindMin(list_t* L, list_node_t* First, list_node_t* Last) {
    
    
    list_node_t* EST = First;
    list_node_t* Rover = First;
    do {
        Rover = Rover->next;
        if(L->comp_proc(Rover->data_ptr, EST->data_ptr) == 1) {
            EST = Rover;//Iteration -> always store the biggest Item
        }
        
    } while(Rover != Last);
    return EST;
    
}

/*****************Auxiliary Function for Recursive Selection Sort**************************/

/* Recursive Selection Sort Decending and Ascending Function, just to make it easy to read.
 * 
 * EST: The pointer to the node that contains LargEST or SmallEST priority. 
 *
 * FindMax, FindMin: return the list_node_t pointer, swap the EST data to the data that 
 * just after the sorted data.
 *
 */
 
void SelectionSortD(list_t** L, list_node_t* m, list_node_t* n)
{
    list_node_t* EST = NULL;
    
    if (m != n) {                              // the boundary never cross themselves
        EST = FindMax(*L, m, n);               // return the largest node containing priority
        swap(&(EST->data_ptr), &(m->data_ptr));
        
        SelectionSortD(&(*L), m->next, n);     // Selection Sort Descending
    }
}

void SelectionSortA(list_t** L, list_node_t* m, list_node_t* n)
{
    list_node_t* EST = NULL;

    if (m != n) {
        EST = FindMin(*L, m, n);                // The FindMin could not be data_ptr, because the direct operation of the data_ptr that   
        swap(&(EST->data_ptr), &(m->data_ptr)); // Do not belong to any list_node_t will lead every data_ptr to point to one place, causing
                                                // memery leak 
        SelectionSortA(&(*L), m->next, n);      // After swapping, shrink the upper bound
    } 
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Sorting the list via the recursive selection sort algorithm
 * 
 * L: pointer to list-of-interest.
 *
 * sort_order: 1 sort list in descending order 2 sort in ascending order 
 */

void list_recursive_selection_sort(list_t** L, int sort_order)
{
    
    /* TODO: see MP3 document for implementation notes */
    //assert(*L != NULL && sort_order != 0);
    list_node_t* First = (*L)->head;
    list_node_t* Last = (*L)->tail;
    
    
    if ((*L)->head == NULL || (*L)->tail == NULL || (*L)->head == (*L)->tail) {  
        return;
    } else {
        if (sort_order == 1) {//DESCENDING
        
            SelectionSortD(&(*L), First, Last);
            
        } else if (sort_order == 2) {//ASCENDING
        
            SelectionSortA(&(*L), First, Last);
            
        }
    }
}

/*+++++++++++++++Auxiliary Function For Iterative Selection Sort++++++++++++++++++++++++*/

/* Iterative Selection Sort: Sorting the list via the selection sort algorithm
 * 
 * Pre-Condition: the lower bound m must smaller than the upper bound, then Rover pointer
 * set to point to the first node of the list
 *
 * Post-Condition: The list is sorted based on the sort_order and list_t L is sorted with-
 * out return node.
 *
 * L: pointer to list-of-interest.  
 *
 * sort_order: 1 sort list in descending order 2 sort in ascending order
 *
 * m, n: the lower bound and the upper bound node
 *
 * swap function: void function that directly swap the data_ptr.  
 */


void IterativeSelectionSort(list_t** L, list_node_t* m, list_node_t* n, int sort_order) {
    
    list_node_t* Rover;
    list_node_t* EST;
    if (sort_order == 1) {
        while(m != n) {
            Rover = m; // start with the lower bound
            EST = m;   // start with the lower bound
            do{
                Rover = Rover->next;
                if ((*L)->comp_proc(Rover->data_ptr, EST->data_ptr) == -1) {
                    
                    EST = Rover;   // Just Like findMax
                } 
            } while (Rover != n);
        
            swap(&(EST->data_ptr), &(m->data_ptr));// swap
            m = m->next;
        }
    } else if (sort_order == 2) {
        while(m != n) {
            Rover = m;
            EST = m;
            do{
                Rover = Rover->next;
                if ((*L)->comp_proc(Rover->data_ptr, EST->data_ptr) == 1) {
                    
                    EST = Rover;   // Just Like findMin
                } 
            } while (Rover != n);
        
            swap(&(EST->data_ptr), &(m->data_ptr));// swap
            m = m->next;
        }
    }
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/* Selection Sort Helper Function: void function that directly swap the data_ptr.   
 *
 * L: pointer to list-of-interest.  
 *
 * sort_order: 1 sort list in descending order 2 sort in ascending order
 *
 * First, Last: the lower bound and the upper bound node  
 */
 
 
void list_selection_sort(list_t** L, int sort_order)
{
    /* TODO: see MP3 document for implementation notes */
    //assert(*L != NULL && sort_order != 0);
    list_node_t* First = (*L)->head;
    list_node_t* Last = (*L)->tail;

   
    if ((*L)->head == NULL || (*L)->tail == NULL || (*L)->head == (*L)->tail) {
        return;
    } else {
    
        IterativeSelectionSort(&(*L), First, Last, sort_order); 
          
    }
    
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/* Function to merge two linked lists and sort
 *
 * Pre-condition: the merge will be the last time that is split, so that the L is refer to the upper layer
 * The Left is the Head of the Left List and the Right is the head of Right List
 *
 * Post-condition: the merge List LR will combine the Left and Light List to one SORTED List 
 *
 * LR: the combination of the Left List and Right list. After every merge, destruct the Left and Right List
 * 
 * Left and Right list_t pointers: transmit the pointer from the recursing layer and from that layer, try to
 * merge and sort them by inserting into new list after comparing the first data of every list.
 *
 * rec_ptr: always points the returned data.
 */    
list_t* merge(list_t** Left, list_t** Right, int sort_order) 
{ 
    //assert(Left != NULL && Right != NULL);
    // Pick the smaller or larger value
    data_t* rec_ptr = NULL;

    list_t* LR = NULL;
    if (*Left != NULL && *Right != NULL) {// the pointer passed to the merge function must not be NULL

        LR  = list_construct((*Left)->comp_proc, (*Left)->data_clean);// recursively construct a pointer
        if (sort_order == 1) {// descending
            while((*Left)->head != NULL && (*Right)->head != NULL) {
                if (LR->comp_proc((*Left)->head->data_ptr, (*Right)->head->data_ptr) == -1) {//If Left first head is larger than the right head                
                    rec_ptr = list_remove(*Left, LISTPOS_HEAD);

                    list_insert(LR, rec_ptr, LISTPOS_TAIL);                                  // remove left first element and add tail
                    rec_ptr = NULL;
                } else {                                                                     //Else, remove the right first element!
                    rec_ptr = list_remove(*Right, LISTPOS_HEAD);                        

                    list_insert(LR, rec_ptr, LISTPOS_TAIL);                                  
                    rec_ptr = NULL;                             
                }
                
            }
            while ((*Left)->head != NULL) {
                rec_ptr = list_remove(*Left, LISTPOS_HEAD);// Notice that instead of using If and Else, use of the while will remove the remaining 
                                                           // items that are sorted but do not return from the recurse
                list_insert(LR, rec_ptr, LISTPOS_TAIL);
                rec_ptr = NULL;
            } while ((*Right)->head != NULL) {
                rec_ptr = list_remove(*Right, LISTPOS_HEAD);//same

                list_insert(LR, rec_ptr, LISTPOS_TAIL);
                rec_ptr = NULL;
            }
        } else if (sort_order == 2){// ascending
            while((*Left)->head != NULL && (*Right)->head != NULL) {
                if (LR->comp_proc((*Left)->head->data_ptr, (*Right)->head->data_ptr) == 1) {//If Left first head is smaller than the right head
                    rec_ptr = list_remove(*Left, LISTPOS_HEAD);                             //Then first remove the left, else remove the right 

                    list_insert(LR, rec_ptr, LISTPOS_TAIL);                                 
                    rec_ptr = NULL;
                } else {
                    rec_ptr = list_remove(*Right, LISTPOS_HEAD);

                    list_insert(LR, rec_ptr, LISTPOS_TAIL);
                    rec_ptr = NULL;
                }
                
            }
            while ((*Left)->head != NULL) {
                rec_ptr = list_remove(*Left, LISTPOS_HEAD);// Notice that instead of using If and Else, use of the while will remove the remaining 
                                                           // items that are sorted but do not return from the recurse
                list_insert(LR, rec_ptr, LISTPOS_TAIL);
                rec_ptr = NULL;
            } while ((*Right)->head != NULL) {             //same
                rec_ptr = list_remove(*Right, LISTPOS_HEAD);

                list_insert(LR, rec_ptr, LISTPOS_TAIL);
                rec_ptr = NULL;
            }
        }
        //assert((*Left)->head == NULL && (*Right)->head == NULL && LR != NULL);
        list_destruct(*Left);
        list_destruct(*Right);
                   
    }
    return LR;  
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/* Sorting the list via the merge sort algorithm
 * 
 * L: pointer to list-of-interest.  
 *
 * sort_order: 1 sort list in descending order 2 sort in ascending order 
 */

void list_merge_sort(list_t** L, int sort_order)      
{
    /* TODO: see MP3 document for implementation notes */
    //assert(*L != NULL && sort_order != 0); 
    
    if ((*L)->head == NULL || (*L)->head->next == NULL || (*L)->tail == NULL) // return to the main function and only the one item, so that terminate to the upper layer
        return; 
    list_t* Left = list_construct((*L)->comp_proc, (*L)->data_clean);
    list_t* Right = list_construct((*L)->comp_proc, (*L)->data_clean);
    int i;
    int mid = (*L)->current_list_size/2;
    int size = (*L)->current_list_size;
    data_t* rec_ptr = NULL;
    for (i = 0; i < mid; i ++) {
        rec_ptr = list_remove(*L, LISTPOS_HEAD);

        list_insert(Left, rec_ptr, LISTPOS_TAIL);
    }
    for (i = mid; i < size; i ++) {
        rec_ptr = list_remove(*L, LISTPOS_HEAD);

        list_insert(Right, rec_ptr, LISTPOS_TAIL);
    }// spilt complete!
    
    list_destruct(*L);// delete the previous list
    *L = NULL;
    list_merge_sort(&Left, sort_order);//recursive dividing from top
    list_merge_sort(&Right, sort_order);
    
    *L = merge(&Left, &Right, sort_order);// combine the two list recursively from bottom
         
} 

//++++++++++++++++++++++++++++++++++++++





/* ----- below are the functions  ----- */

/* Obtains a pointer to an element stored in the specified list, at the
 * specified list position
 * 
 * list_ptr: pointer to list-of-interest.  A pointer to an empty list is
 *           obtained from list_construct.
 *
 * pos_index: position of the element to be accessed.  Index starts at 0 at
 *            head of the list, and incremented by one until the tail is
 *            reached.  Can also specify LISTPOS_HEAD and LISTPOS_TAIL
 *
 * return value: pointer to element accessed within the specified list.  A
 * value NULL is returned if the pos_index does not correspond to an element in
 * the list.  For example, if the list is empty, NULL is returned.
 */
data_t * list_access(list_t *list_ptr, int pos_index)
{
    int count = 0;
    list_node_t *L = NULL;
 
    assert(list_ptr != NULL);

    /* debugging function to verify that the structure of the list is valid */
    list_debug_validate(list_ptr);

    /* TODO: handle four special cases.
     *   1.  The list is empty
     *   2.  Asking for the head 
     *   3.  Asking for the tail
     *   4.  Asking for invalid pos
     */

    L = list_ptr->head;

  
    
    if (L == NULL) { // 1. The list is empty
        return NULL;
    } else if (pos_index == LISTPOS_HEAD || pos_index == 0) { // 2. Asking for the head
        L = list_ptr->head;
    } else if (pos_index == LISTPOS_TAIL || pos_index == list_ptr->current_list_size - 1) { // 3. Asking for the tail
        L = list_ptr->tail;
    } else if (pos_index > list_ptr->current_list_size - 1 || (pos_index < 0 && pos_index 
    != LISTPOS_HEAD && pos_index != LISTPOS_TAIL)) { // 4. Asking for the invalid pos
        return NULL;
    }
    /* we now know pos_index is for an interal element */
    /* TODO: loop through the list until find correct position index */
      else { 
          while(count < pos_index) {
              L = L->next;
              count ++;
          }
          
    }
    assert(L != NULL);
    return L->data_ptr;

}

/* Allocates a new, empty list 
 *
 * By convention, the list is initially assumed to be sorted.  The field sorted
 * can only take values LIST_SORTED_ASCENDING LIST_SORTED_DESCENDING or 
 * LIST_UNSORTED
 *
 * Use list_free to remove and deallocate all elements on a list (retaining the
 * list itself).
 *
 * comp_proc = pointer to comparison function
 *
 * Use the standard function free() to deallocate a list which is no longer
 * useful (after freeing its elements).
 */
list_t * list_construct(int (*fcomp)(const data_t *, const data_t *),
        void (*dataclean)(data_t *))
{
    list_t *L;
    L = (list_t *) malloc(sizeof(list_t));
    L->head = NULL;
    L->tail = NULL;
    L->current_list_size = 0;
    L->list_sorted_state = LIST_SORTED_ASCENDING;
    L->comp_proc = fcomp;
    L->data_clean = dataclean;

    /* the last line of this function must call validate */
    list_debug_validate(L);
    return L;
}

/* Finds an element in a list and returns a pointer to it.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * elem_ptr: element against which other elements in the list are compared.
 *           Note: use the comp_proc function pointer found in the list_t 
 *           header block. 
 *
 * The function returns a pointer to the matching element with lowest index if
 * a match if found.  If a match is not found the return value is NULL.
 *
 * The function also returns the integer position of matching element with the
 *           lowest index.  If a matching element is not found, the position
 *           index that is returned should be -1. 
 *
 * pos_index: used as a return value for the position index of matching element
 *
 * Finder: list_node_t pointer that used to point to a matching record
 *
 * rec: used as a container to receive the return value of the comp function
 * if the element matches the things in the record, rec = 0, the elem found
 *
 */
data_t * list_elem_find(list_t *list_ptr, data_t *elem_ptr, int *pos_index)
{
    list_debug_validate(list_ptr);

    /*TODO: */
    /* fix the return value */
    int i = -1;
    list_node_t* Finder = NULL; 
    Finder = list_ptr->head;
    int rec = -2;//Intialize to a value that is not 1,0,-1
    if (list_ptr->head == NULL) {
        return NULL;
    }
    while (Finder != NULL) {
        rec = list_ptr->comp_proc(elem_ptr, Finder->data_ptr);// find the matching record                    
        if (rec == 0) {
            break;
        } 
        Finder = Finder->next;
        i ++;   
    }
    if (Finder != NULL) {// if find, the i can not be the last item
        *pos_index = i;
        return Finder->data_ptr;
                                                                                          
    } else {
        *pos_index = -1;
    }
    
    return NULL;

}

/* Deallocates the contents of the specified list, releasing associated memory
 * resources for other purposes.
 *
 * Free all elements in the list, and the header block.  Use the data_clean
 * function point to free the data_t items in the list.
 */
void list_destruct(list_t *list_ptr)
{
    /* the first line must validate the list */
    list_debug_validate(list_ptr);

    /*TODO*/
    // Your code starts here
    int i = 0;
    list_node_t* first = NULL;
    if (list_ptr->head != NULL) {
        first = list_ptr->head;      // first pointer to start at the beginning
        for (i = 0; i < list_ptr->current_list_size - 1; i ++) {
            list_ptr->data_clean (first->data_ptr); // clean the content first           
            first->data_ptr = NULL;    
            first = first->next;
            first->prev->next = NULL;
            list_ptr->head = first;
            free(first->prev);  
            first->prev = NULL;
        }
        list_ptr->data_clean (first->data_ptr);
        free(first);                                  
        first = NULL;
        
    }
    list_ptr->tail = NULL;
    list_ptr->head = NULL;
    free(list_ptr);                                 // free the list pointer
}

/* Inserts the specified data element into the specified list at the specified
 * position.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * elem_ptr: pointer to the element to be inserted into list.
 *
 * pos_index: numeric position index of the element to be inserted into the 
 *            list.  Index starts at 0 at head of the list, and incremented by 
 *            one until the tail is reached.  The index can also be equal
 *            to LISTPOS_HEAD or LISTPOS_TAIL (these are special negative 
 *            values use to provide a short cut for adding to the head
 *            or tail of the list).
 *
 * If pos_index is greater than the number of elements currently in the list, 
 * the element is simply appended to the end of the list (no additional elements
 * are inserted).
 *
 * Note that use of this function results in the list to be marked as unsorted,
 * even if the element has been inserted in the correct position.  That is, on
 * completion of this subroutine the list_ptr->list_sorted_state must be equal 
 * to LIST_UNSORTED.
 */
void list_insert(list_t *list_ptr, data_t *elem_ptr, int pos_index)
{
    assert(list_ptr != NULL);
    
    /*TODO*/
    // if the pos_index is larger than the current_list_size, then append it to the end
    
    list_node_t *Insert = NULL;
    list_node_t *Data = (list_node_t *) calloc (1, sizeof(list_node_t));// the data pointer used to store the data
    Data->data_ptr = elem_ptr; // take the data from elem-ptr
    elem_ptr = NULL;  // prevent the dangling and point to the same place
    if (list_ptr->current_list_size == 0) {
    
        list_ptr->head = Data;
        list_ptr->tail = Data;

    } else if (pos_index == LISTPOS_HEAD || pos_index == 0) {
    
        Data->prev = NULL;
        Data->next = list_ptr->head;// assign the Data next pointer to the list
        list_ptr->head->prev = Data;// assign the prev ptr of head to the Data Node 
        list_ptr->head = list_ptr->head->prev; // move the head to the left, the front
        
    } else if (pos_index == LISTPOS_TAIL || pos_index >= list_ptr->current_list_size - 1) {
    
        Data->next = NULL;
        Data->prev = list_ptr->tail;// assign the Data pointer to the list
        list_ptr->tail->next = Data; // assign the next of tail pointer to Data Node
        list_ptr->tail = list_ptr->tail->next;// move the tail pointer to the right, the back
         
    } else if (pos_index > 0){
    
        Insert = list_ptr->head;                                                              
        int i = 0;
                        
        while(i < pos_index) {
            Insert = Insert->next; // move the pointer along the list, to the position of insert
            i ++;
        }
        i = 0;
        Data->next = Insert; // insert to the previous of the Insert node points to
        Insert->prev->next = Data;
        Data->prev = Insert->prev;
        Insert->prev = Data;
        
            
    } else {
        return; //if the index is less than 1 and do not equal to the LISTPOS_HEAD or LISTPOS_TAIL
    }
    list_ptr->current_list_size ++;
    
    Insert = NULL;


    /*TODO*/
    /* the last two lines of this function must be the following */
    if (list_ptr->list_sorted_state != LIST_UNSORTED)
        list_ptr->list_sorted_state = LIST_UNSORTED;
    list_debug_validate(list_ptr);
}

/* Inserts the element into the specified sorted list at the proper position,
 * as defined by the comp_proc function pointer found in the header block.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * elem_ptr: pointer to the element to be inserted into list.
 *
 * If you use list_insert_sorted, the list preserves its sorted nature.
 *
 * If you use list_insert, the list will be considered to be unsorted, even
 * if the element has been inserted in the correct position.
 *
 * If the list is not sorted and you call list_insert_sorted, this subroutine
 * should generate a system error and the program should immediately stop.
 *
 * The comparison procedure must accept two arguments (A and B) which are both
 * pointers to elements of type data_t.  The comparison procedure returns an
 * integer code which indicates the precedence relationship between the two
 * elements.  The integer code takes on the following values:
 *    1: A should be closer to the list head than B
 *   -1: B should be closer to the list head than A
 *    0: A and B are equal in rank
 * This definition results in the list being in ascending order.  To insert
 * in decending order, change the sign of the value that is returned.
 *
 * Note: if the element to be inserted is equal in rank to an element already
 * in the list, the newly inserted element will be placed after all the
 * elements of equal rank that are already in the list.
 */
void list_insert_sorted(list_t *list_ptr, data_t *elem_ptr)
{
    assert(list_ptr != NULL);
    assert(list_ptr->list_sorted_state != LIST_UNSORTED);

    /* insert your code here */
    /*TODO*/

    int i = 0;
    list_node_t* data_carrier = (list_node_t *) calloc(1,sizeof(list_node_t)); // data_carrier carries that data
    list_node_t* finder_node = NULL; // assign the new value to the finder node
    data_carrier->data_ptr = elem_ptr; 
    data_carrier->next = NULL;
    data_carrier->prev = NULL;
    if (list_ptr->current_list_size == 0) {
    
        list_ptr->head = data_carrier;
        list_ptr->tail = data_carrier;
        

    } else if (list_ptr->current_list_size > 0) {
        if (list_ptr->list_sorted_state == LIST_SORTED_ASCENDING) {
        
            finder_node = list_ptr->head;

            for (i = 0; i < list_ptr->current_list_size; i ++) {
            // elem_ptr is closer to head than found finder_node, we put the node to the previous of the finder node
                if (list_ptr->comp_proc(elem_ptr, finder_node->data_ptr) == 1) {
                        // Insert the element behind the head of the list
                    if (finder_node->prev == NULL ) {
                        data_carrier->next = finder_node;
                        finder_node->prev = data_carrier;
                        data_carrier->prev = NULL;
                        list_ptr->head = list_ptr->head->prev;
                        break;
                    } else {
                        // Insert between the two nodes in the middle, still behind the finder node
                        data_carrier->next = finder_node;
                        finder_node->prev->next = data_carrier;
                        data_carrier->prev = finder_node->prev;
                        finder_node->prev = data_carrier;
                        break;
                    }
                } else { // if the compare function returns 0 or -1, keep moving forward
                    finder_node = finder_node->next;
                }
            }
            if (i == list_ptr->current_list_size) {//insert after the tail
                data_carrier->next = NULL;
                data_carrier->prev = list_ptr->tail;
                list_ptr->tail->next = data_carrier;
                list_ptr->tail = list_ptr->tail->next;
            }
            
        } else if (list_ptr->list_sorted_state == LIST_SORTED_DESCENDING) {
    
            finder_node = list_ptr->tail;
        
            for (i = 0; i < list_ptr->current_list_size; i ++) {
            // elem_ptr is NOT closer(smaller) to head than the found finder_node, then we put the element to the node after the finder node
                if (list_ptr->comp_proc(elem_ptr, finder_node->data_ptr) == 1) {        
                // Insert the element after the tail of the list 
                    if (finder_node->next == NULL) {
                        data_carrier->prev = finder_node;
                        finder_node->next = data_carrier;
                        data_carrier->next = NULL;
                        list_ptr->tail = list_ptr->tail->next;
                        break;
                    } else {
                    // Insert between the two nodes, still after the finder node
                        data_carrier->next = finder_node->next;
                        data_carrier->next->prev = data_carrier;
                        finder_node->next = data_carrier;
                        data_carrier->prev = finder_node;
                        break;
                    }
                } else { // if the compare function return s 0 or -1, keep moving backward
                    finder_node = finder_node->prev;
                }
            }
            if (i == list_ptr->current_list_size) {//insert on the tail
                data_carrier->prev = NULL;
                data_carrier->next = list_ptr->head;
                list_ptr->head->prev = data_carrier;
                list_ptr->head = list_ptr->head->prev;
            }
        }
    }
    list_ptr->current_list_size ++;

    /* the last line of this function must be the following */
    list_debug_validate(list_ptr);
}

/* Removes an element from the specified list, at the specified list position,
 * and returns a pointer to the element.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * pos_index: position of the element to be removed.  Index starts at 0 at
 *            head of the list, and incremented by one until the tail is
 *            reached.  Can also specify LISTPOS_HEAD and LISTPOS_TAIL
 *
 * Attempting to remove an element at a position index that is not contained in
 * the list will result in no element being removed, and a NULL pointer will be
 * returned.
 */
data_t * list_remove(list_t *list_ptr, int pos_index)
{
    list_debug_validate(list_ptr);


    int i;
    list_node_t* Finder;
    data_t* Data = NULL;

    Finder = list_ptr->head;
    if (Finder == NULL) return NULL;
    // 1.on the head
    // 2.on the middle
    // 3.on the tail
    
    if (pos_index == 0 || pos_index == LISTPOS_HEAD) { // 1. on the head
    
        Finder = list_ptr->head;
        Data = Finder->data_ptr;
        if (Finder->next != NULL) {
            Finder->next->prev = NULL;
            list_ptr->head = list_ptr->head->next;
            Finder->next = NULL;
        } else { // the head is only one element
            list_ptr->head = NULL;
            list_ptr->tail = NULL;
        }
        Finder->next = NULL;
        Finder->prev = NULL;
        Finder->data_ptr = NULL;
        if (Finder != NULL) {
            free(Finder);
        }
        list_ptr->current_list_size --;
        return Data;
    } else if (pos_index == list_ptr->current_list_size - 1 || pos_index == LISTPOS_TAIL ) {
    
        Finder = list_ptr->tail;                       // 2. on the tail
        Data = Finder->data_ptr;
        if (Finder->prev != NULL) {
            Finder->prev->next = NULL;
            list_ptr->tail = list_ptr->tail->prev;
            Finder->prev = NULL;
        } else { // the head is only one element
            list_ptr->head = NULL;
            list_ptr->tail = NULL;
        }
        Finder->next = NULL;
        Finder->prev = NULL;
        Finder->data_ptr = NULL;
        if (Finder != NULL) {
            free(Finder);
        }
        list_ptr->current_list_size --;
        return Data;
    } else if (pos_index > 0 && pos_index < list_ptr->current_list_size - 1) {
    
        Finder = list_ptr->head;                      // 3. on the middle
        for (i = 0; i < pos_index; i ++) {
            Finder = Finder->next;
        }
        Data = Finder->data_ptr;
        Finder->next->prev = Finder->prev;
        Finder->prev->next = Finder->next;
        
        Finder->next = NULL;
        Finder->prev = NULL;
        Finder->data_ptr = NULL;
        if (Finder != NULL) {
            free(Finder);
        }
        list_ptr->current_list_size --;
        return Data;
    }


    /* fix the return value */
    return NULL;
}

/* Reverse the order of the elements in the list.  Also change the 
 * list_sorted_state flag.  This function can only be called on a list
 * that is sorted.
 *
 * list_ptr: pointer to list-of-interest.  
 */
void list_reverse(list_t *list_ptr)
{
    assert(list_order(list_ptr) != 0);
    /*TODO*/
    
    // first find the head node or tail node, then reverse the each pointer.
    int rec = -2;// initialize to a value that is not 1,0,-1
    list_node_t *temp = NULL;
    list_node_t *temp_head = NULL;
    temp_head = list_ptr->head;
    while(list_ptr->head != NULL) {
        temp = list_ptr->head->prev;
        list_ptr->head->prev = list_ptr->head->next;
        list_ptr->head->next = temp;
        list_ptr->head = list_ptr->head->prev;
    }
    if (temp != NULL){
        list_ptr->head = temp->prev;
    }
    list_ptr->tail = temp_head;
    temp = NULL;
    temp_head = NULL;
    rec = list_order(list_ptr);
    if (rec == 1) { // Reverse the sorted state
        list_ptr->list_sorted_state = LIST_SORTED_DESCENDING;
    } else {
        list_ptr->list_sorted_state = LIST_SORTED_ASCENDING;
    }
    
    // after the list is reversed verify it is valid.
    list_debug_validate(list_ptr);
}

/* Obtains the length of the specified list, that is, the number of elements
 * that the list contains.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * Returns an integer equal to the number of elements stored in the list.  An
 * empty list has a size of zero.
 */
int list_size(list_t *list_ptr)
{
    assert(list_ptr != NULL);
    assert(list_ptr->current_list_size >= 0);
    return list_ptr->current_list_size;
}

/* Obtains the sort status and order of the specified list. 
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * Returns 
 *    1: the list is sorted in ascending order
 *   -1: descending order
 *    0: the list is not sorted but a queue
 */
int list_order(list_t *list_ptr)
{
    assert(list_ptr != NULL);
    if (list_ptr->list_sorted_state == LIST_SORTED_ASCENDING)
        return 1;
    else if (list_ptr->list_sorted_state == LIST_SORTED_DESCENDING)
        return -1;
    else if (list_ptr->list_sorted_state == LIST_UNSORTED)
        return 0;
    else 
        exit(5);  // this should not happen
}


/* This function verifies that the pointers for the two-way linked list are
 * valid, and that the list size matches the number of items in the list.
 *
 * If the linked list is sorted it also checks that the elements in the list
 * appear in the proper order.
 *
 * The function produces no output if the two-way linked list is correct.  It
 * causes the program to terminate and print a line beginning with "Assertion
 * failed:" if an error is detected.
 *
 * The checks are not exhaustive, so an error may still exist in the
 * list even if these checks pass.
 *
 * YOU MUST NOT CHANGE THIS FUNCTION.  WE USE IT DURING GRADING TO VERIFY THAT
 * YOUR LIST IS CONSISTENT.
 */
void list_debug_validate(list_t *L)
{
#ifdef VALIDATE
    list_node_t *N;
    int count = 0;
    assert(L != NULL);
    if (L->head == NULL)
        assert(L->tail == NULL && L->current_list_size == 0);
    if (L->tail == NULL)
        assert(L->head == NULL && L->current_list_size == 0);
    if (L->current_list_size == 0)
        assert(L->head == NULL && L->tail == NULL);
    assert(L->list_sorted_state == LIST_SORTED_ASCENDING 
            || L->list_sorted_state == LIST_SORTED_DESCENDING
            || L->list_sorted_state == LIST_UNSORTED);

    if (L->current_list_size == 1) {
        assert(L->head == L->tail && L->head != NULL);
        assert(L->head->next == NULL && L->head->prev == NULL);
        assert(L->head->data_ptr != NULL);
    }
    if (L->head == L->tail && L->head != NULL)
        assert(L->current_list_size == 1);
    if (L->current_list_size > 1) {
        assert(L->head != L->tail && L->head != NULL && L->tail != NULL);
        N = L->head;
        assert(N->prev == NULL);
        while (N != NULL) {
            assert(N->data_ptr != NULL);
            if (N->next != NULL)
                assert(N->next->prev == N);
            else
                assert(N == L->tail);
            count++;
            N = N->next;
        }
        assert(count == L->current_list_size);
    }
    if (L->list_sorted_state != LIST_UNSORTED && L->head != NULL) {
        N = L->head;
        int comp_val = -1 * list_order(L);
        while (N->next != NULL) {
            assert((L->comp_proc)(N->data_ptr, N->next->data_ptr) != comp_val);
            N = N->next;
        }
    }
#endif
}
/* commands for vim. ts: tabstop, sts: softtabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */

