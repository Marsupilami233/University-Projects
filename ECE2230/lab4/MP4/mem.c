/* mem.c          
 * Yulin Xiao          
 * Yulinx
 * ECE 2230 Spring 2019
 * MP4
 *
 * Propose: A template for MP4
 *
 * Assumptions: To allocate memory, frees and test them if the Rover is moving along the list. 
 *  
 *
 * Bugs: None that I know
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>

#include "mem.h"
/* Only in the mem.h and do many things in the mem.c, and divide them into a lot of auxilliary functions and free return the */
/* chunk_t definition (do not modify).
 * sizeof(chunk_t) must equal SIZEOF_CHUNK_T in mem.h.
 */
typedef struct chunk_tag {
    struct chunk_tag *next;  /* pointer to the next free block */
    long size;               /* size of memory block in units of
                                sizeof(chunk_t) */
} chunk_t;

typedef struct heap_stats { /* Caculatating continueously */
    int numItems;    /* number of chunks in the free list    */
    int min;         /* size of the smallest chunk, in bytes */
    int max;         /* size of the largest chunk, in bytes  */
    int average;     /* average size of the chunks, in bytes */
    int totalBytes;  /* total size of all chunks, in bytes   */

    /* the following two fields are updated in morecore() */
    int numSbrkCalls;  /* number of successful calls to sbrk()  */
    int numPages;      /* number of pages allocated with sbrk() */
} heap_stats_t;

/* global variables exported via mem.h */
int SearchPolicy = FIRST_FIT;
int SearchLoc = HEAD_FIRST;
int Coalescing = FALSE;

/* global variables restricted to mem.c only */
static chunk_t Dummy = {
    &Dummy,  /* next */
    0        /* size */
};
static chunk_t * Rover = &Dummy;
static heap_stats_t stats;  /* initialized by the O/S to all 0s */


/* prototypes for functions private to mem.c */
void mem_validate(void);
chunk_t *morecore(int);

/* function to request 1 or more pages from the operating system.
 *
 * new_bytes must be the number of bytes that are being requested from
 *           the OS with the sbrk command.  It must be an integer 
 *           multiple of the PAGESIZE
 *
 * returns a pointer to the new memory location.  If the request for
 * new memory fails this function simply returns NULL, and assumes some
 * calling function will handle the error condition.  Since the error
 * condition is catastrophic, nothing can be done but to terminate 
 * the program.
 */
chunk_t *morecore(int new_bytes) 
{
    char *cp;
    chunk_t *new_p;

    /* assert preconditions */
    assert(new_bytes % PAGESIZE == 0 && new_bytes > 0);
    assert(PAGESIZE % sizeof(chunk_t) == 0);

    /* allocate memory using sbrk() */
    cp = sbrk(new_bytes);
    if (cp == (char *) -1)  /* no space available */
        return NULL;
    new_p = (chunk_t *) cp;
    
    /* update statistics */
    stats.numSbrkCalls++;
    stats.numPages += new_bytes/PAGESIZE;

    return new_p;
}

/* TODO update this documentation according to the programming guide */
/* function to free the memory block and store it to where the Rover is and move the Rover 
 * onto the returned block.
 *
 * return_ptr is the pointer to correctly returned from user. It must be casted from the 
 *            void* to chunk_t* and move backward one to find a head.
 *
 * Rover_Prev is the memory block before the Rover.          
 *
 * Header     is the block head that found by (chunk_t*)return_ptr - 1      
 *  
 */
void Mem_free(void *return_ptr)
{
    /* TODO obviously the next line is WRONG!!!!  You must fix it. */
    // free(return_ptr);// Insert the pointer to the last nodes that allocated
    if (return_ptr != NULL) { 
        chunk_t* Rover_Prev = Rover;
        chunk_t* Header = (chunk_t*)return_ptr - 1;// Back for one node will help to find the Header block
        while (Rover_Prev->next != Rover) {
            Rover_Prev = Rover_Prev->next;
        }
        Header->next = Rover;
        Rover_Prev->next = Header;// the Previous is the two nodes from the Rover
        Rover = Header;// maintain the Rover and the Rover_Prev's places
    }
}
/*To do the allocate things: Partition, ExactFit, Allocate_Partition*/

/* function to split the memory block and remove it from the free list and move the Rover 
 * onto the returned block orignal place to make it constant.
 *
 * Mptr       is the pointer to the largest or smallest or just available memory block 
 *            ready to do partition 
 *            
 * MptrP      is the prev pointer of Mptr
 *           
 * mov_units  is the unit Part pointer should move, = nunits = num_units 
 *
 * Part       is the pointer to do partition, and to find a place to set the header of a       
 *            memory block.
 *
 * rec_ptr    is the pointer to return to user to use the memory block(to fit())
 */
void* Partition(chunk_t* Mptr, chunk_t* MptrP, int mov_units) {

    Rover = Mptr;
    chunk_t* Rover_P = MptrP;
    chunk_t* Part = NULL;
    chunk_t* rec_ptr = NULL;                               
                              // <---------------------------1).Spilting into two blocks, set the new header to the Remain Block
    Part = Rover;             // <------------------------------Find the Part of Rover
    Part = Part + mov_units;  // <---------- -------------------Find the place to insert the blocks
    Part->next = Rover->next; // <------------------------------Set the next pointer to the same block that the Rover Points to            
    Part->size = Rover->size - mov_units;// <-------------------Set the size of the Remain Part
                              // <---------------------------2).Change the next pointer
    assert(Rover != &Dummy);
    Rover->next = NULL;       // <------------------------------Unlink the User's next block
    Rover->size = mov_units;  // <------------------------------Rover->size is the unit to give to the user
                              // <---------------------------3).Giving the rec_ptr the correct thing 
    rec_ptr = Rover + 1;      // <------------------------------Prev Node size
                 
    Rover_P->next = Part;     // <------------------------------Link the Prev User's block to the Remain Part
    Rover = Rover_P->next;    // <------------------------------Move the Rover to the next block, just after the Rover_P
     
    return rec_ptr;
                             
}
/* function to remove the memory block from the free list and move the Rover 
 * onto the returned block orignal place to make it constant.
 *
 * Mptr       is the pointer to the largest or smallest or just available memory block 
 *            ready to do partition 
 *            
 * MptrP      is the prev pointer of Mptr
 *           
 * Rover_P    is the prev pointer of Rover  
 *
 * rec_ptr    is the pointer to return to user to use the memory block(to fit())
 */
void* ExactFit(chunk_t* Mptr, chunk_t* MptrP) {

    Rover = Mptr;
    chunk_t* Rover_P = MptrP;
    chunk_t* rec_ptr = NULL;        
    Rover_P->next = Rover->next;// <--------------------------1).Link the Rover_prev to the next block
    assert(Rover != &Dummy);
    Rover->next = NULL;// <-----------------------------------2).Rover->next = NULL
    rec_ptr = Rover + 1;// <----------------------------------3).return
    Rover = Rover_P->next;// <-------------Rover and Rover_P is still consistent
    return rec_ptr;
    
}
/* function to call morecore for extra memory pages and split the returned memory block
 * and remove it from the free list and move the Rover onto the returned block orignal 
 * place to make it constant. (Some Paritions will be done by calling the Partition())
 *
 * new_bytes    is the bytes that needed to call morecore() 
 *            
 * Dummy_P      is the pointer points to the last memory block
 *           
 * alloc_units  is the units size that should allocate, = nunits = num_units = mov_units 
 *
 * Rover_P      is the pointer points to the Rover's prev block       
 *            
 * rec_ptr    is the pointer to return to user to use the memory block(to fit())
 */
void* Alloc_Partition(int alloc_units) {
    
    int new_bytes = 0;
    new_bytes = alloc_units * SIZEOF_CHUNK_T;
    chunk_t* Dummy_P = &Dummy;// place to find the most right block
    chunk_t* new_a = NULL;
    chunk_t* Rover_P = Rover;
    chunk_t* rec_ptr = NULL;
    /*Set the Dummy size!*/
    while(Dummy_P->next != &Dummy) {
        Dummy_P = Dummy_P->next;
    }
         
    if (new_bytes % PAGESIZE != 0) {
        new_a = morecore((new_bytes / PAGESIZE + 1) * PAGESIZE); // if the nbytes is not mutiple of PAGESIZE, so that allocate for more than one page
        new_a->size = ((new_bytes / PAGESIZE + 1) * PAGESIZE) / SIZEOF_CHUNK_T;   
    } else { 
        new_a = morecore(new_bytes);// exact bytes of the memory
        new_a->size = new_bytes / SIZEOF_CHUNK_T;
    }
    /*Put the allocated memory into the last block*/
    Rover = new_a;
    Dummy_P->next = new_a;// every time that allocate new block put it on the last item
    new_a->next = &Dummy; 
    Dummy_P = new_a;// Dummy_P will be the new last block
    /*Catch up with the Rover*/
    while(Rover_P->next != Rover) {
        Rover_P = Rover_P->next;
    }
 
    assert(Rover == Dummy_P && Dummy_P->next == &Dummy);
    if (alloc_units < new_a->size) {// allocate units are not equal to the newly allocated size
        rec_ptr = Partition(Rover, Rover_P, alloc_units);
    } else {// allocate units are the the newliy allocated size, can not larger than the new_a->size
        
        rec_ptr = Rover + 1;// return the user
        Rover_P->next = Rover->next;// the allocated block is link to the Rover's next block
        assert(Rover != &Dummy);
        Rover->next = NULL;// set it to NULL
        Rover = Rover_P->next;// Rover move to the next of the allocated block
    }
    
    return rec_ptr;                              
}

/*To do the allocate things: Partition, ExactFit, Allocate_Partition*/
/* TODO update this documentation */
/*---------------------------------Best, First, Worst----------------------------*/
/* function to find the smallest memory block from the free list and move the Rover 
 * onto the returned block orignal place to make it constant.
 *
 * num_units  is the number of units the sizeof block should allocate, = nunits 
 *
 * Mptr       is the pointer to the largest or smallest or just available memory block 
 *            ready to do partition 
 *            
 * MptrP      is the prev pointer of Mptr
 *           
 * Record_R   is the pointer to record Rover's place before calling the function  
 *
 * return_ptr is the pointer to return to user to use the memory block (to Mem_alloc())
 *
 * Min        is an integer to record the difference between the block size and the
 *            sizeof (user needed + 1)
 */
void *BestFit(int num_units) {
    assert(SearchPolicy == BEST_FIT && num_units >= 2);
    chunk_t* Record_R = Rover;// The Pointer to Record the position before the Rover moved
    chunk_t* M_ptr = NULL;
    chunk_t* M_ptrP = NULL;
    chunk_t* return_ptr = NULL;// return to Mem_Alloc
    int Min = -1;
    do {
        if (Rover->size - num_units >= 0) {// The blocks are available
            if (Rover == Record_R || Min == -1) {// Rover is at the first memory block or it is the first memory block
                Min = Rover->size - num_units;// Record
                M_ptr = Rover;// Record Location
            } else {// the Second available block is found and not Rover is not at its orignial place, compare 
                if (Rover->size - num_units < Min) {// find smaller part
                    Min = Rover->size - num_units;// Record
                    M_ptr = Rover;// Record Location
                } 
            }   // the Second available blocks are not the best, original, and smaller do nothing 
        }
        Rover = Rover->next; 
        
    } while (Rover != Record_R);// circle around and without compare the last part
    // there are several situations: 
    // 1.Min = -1, nothing avaible found
    // 2. Min = 0, exactfit, found it at M_ptr
    // 3. Min > 0, bestfit? not really:1) the only one block meets the requirment
    //                                 2) the first one block meets the requirment
    //                                 3) the last one or the one in the middle, how to find it?
    // 4. While loop did not enter!: only dummy goto 1.                       
    // 5. Min < 0, bug! 
                        
    if (M_ptr != NULL) {// found!
        M_ptrP = M_ptr;
        while (M_ptrP->next != M_ptr) {
            M_ptrP = M_ptrP->next;// find the block before the founded smallest
        }
    }
    assert(Min >= -1);
    if (Min > 0) {
        return_ptr = Partition(M_ptr, M_ptrP, num_units);
    } else if (Min == 0) {
        return_ptr = ExactFit(M_ptr, M_ptrP);
    } else if (Min == -1){
        return_ptr = Alloc_Partition(num_units);
    }

    return return_ptr;
}
/* function to find the first memory block from the free list and move the Rover 
 * onto the returned block orignal place to make it constant.
 *
 * num_units  is the number of units the sizeof block should allocate, = nunits
 *
 * Mptr       is the pointer to the largest or smallest or just available memory block 
 *            ready to do partition 
 *            
 * Rover_P      is the prev pointer of Rover
 *           
 * Record_R   is the pointer to record Rover's place before calling the function  
 *
 * return_ptr is the pointer to return to user to use the memory block (to Mem_alloc())
 *
 * Min        is an integer to record the difference between the block size and the
 *            sizeof (user needed + 1)
 */
void *FirstFit(int num_units) {
    assert(SearchPolicy == FIRST_FIT && num_units >= 2);
    chunk_t* Record_R = Rover;// The Pointer to Record the position of Rover
    chunk_t* Rover_P = NULL;// The Pointer to Record the position before Rover
    chunk_t* return_ptr = NULL;// return to Mem_Alloc
    do {
        if (Rover->size - num_units >= 0) {// Found!
            Rover_P = Rover;
            while (Rover_P->next != Rover) {// if the first block is the availble block
                Rover_P = Rover_P->next;
            }
            break;
        }
        // not found
        Rover_P = Rover;// follow the Rover
        Rover = Rover->next;
    } while (Rover != Record_R);
    // There are several conditions: 
    // only Dummy: Rover->size - num_units < 0
    // only one in the list: found: Rover == Record, Rover_P == &Dummy
    // not found: Rover_P must be the prev of the Rover, Rover is static   
    if (Rover->size - num_units > 0) {
        return_ptr = Partition(Rover, Rover_P, num_units);
    } else if (Rover->size - num_units == 0) {
        return_ptr = ExactFit(Rover, Rover_P);
    } else {
        return_ptr = Alloc_Partition(num_units);
    }
    return return_ptr;
}
/* function to find the largest memory block from the free list and move the Rover 
 * onto the returned block orignal place to make it constant.
 *
 * num_units  is the number of units the sizeof block should allocate, = nunits
 *
 * Mptr       is the pointer to the largest or smallest or just available memory block 
 *            ready to do partition 
 *            
 * MptrP      is the prev pointer of Mptr
 *           
 * Record_R   is the pointer to record Rover's place before calling the function  
 *
 * return_ptr is the pointer to return to user to use the memory block (to Mem_alloc())
 *
 * Max        is an integer to record the difference between the block size and the
 *            sizeof (user needed + 1)
 */
void *WorstFit(int num_units) {
    assert(SearchPolicy == WORST_FIT && num_units >= 2);
    chunk_t* Record_R = Rover;// The Pointer to Record the position before the Rover moved
    chunk_t* M_ptr = NULL;
    chunk_t* M_ptrP = NULL;
    chunk_t* return_ptr = NULL;// return to Mem_Alloc
    int Max = -1;
    do {
        if (Rover->size - num_units >= 0) {// The blocks are available
            if (Rover == Record_R || Max == -1) {// Rover is at the first memory block or it is the first memory block
                Max = Rover->size - num_units;// Record
                M_ptr = Rover;// Record Location
            } else {// the Second available block is found and not Rover is not at its orignial place, compare 
                if (Rover->size - num_units > Max) {// find bigger part
                    Max = Rover->size - num_units;// Record
                    M_ptr = Rover;// Record Location
                } 
            }   // the Second available blocks are not the best, original, and larger do nothing 
        }
        Rover = Rover->next; 
        
    } while (Rover != Record_R);// circle around and without compare the last part
    // there are several situations: 
    // 1.Max = -1, nothing avaible found
    // 2. Max = 0, exactfit, found it at M_ptr
    // 3. Max > 0, bestfit? not really:1) the only one block meets the requirment
    //                                 2) the first one block meets the requirment
    //                                 3) the last one or the one in the middle, how to find it?-->M-ptr
    // 4. While loop did not enter!: only dummy goto 1.                       
    // 5. Max < -1, bug!                     
    if (M_ptr != NULL) {// found!
        M_ptrP = M_ptr;
        while (M_ptrP->next != M_ptr) {
            M_ptrP = M_ptrP->next;// find the block before the founded smallest
        }
    }
    assert(Max >= -1);
    if (Max > 0) {
        return_ptr = Partition(M_ptr, M_ptrP, num_units);
    } else if (Max == 0) {
        return_ptr = ExactFit(M_ptr, M_ptrP);
    } else if (Max == -1){
        return_ptr = Alloc_Partition(num_units);
    }

    return return_ptr;
}
/*---------------------------------Best, First, Worst----------------------------*/
/*Using the auxiliary function to do the Comparation and Division*/ 
/* sub-main function to alloc the memory blocks using three SearchPolicy and two SearchLoc 
 * by call three functions and initialize the Rover to Dummy block 
 *
 * nbytes     is the number of bytes the user wants, should be converted to nunits
 *
 * nunits     is the unit size of each memory block 
 *
 * rec_ptr is the pointer to return to user to use the memory block (to main() in lab4.c)
 *
 */
void *Mem_alloc(int nbytes) {
    /* assert preconditions */
    /*  1. get the appropriate big data chunk from core
        2. link it to the free list
        3. get the user's usable memory chunk and return
        4. do the options of Rover Search option and Head Search option
        5. doing the first-fit, best-fit, and the worst-fit

     */
    assert(nbytes > 0);

    /* TODO obviously the next line is WRONG!!!!  You must fix it. *///<-------------fixed
    chunk_t* rec_ptr = NULL;// the Pointer to receive the returned pointer
    int nunits = 0;
    nbytes = nbytes + SIZEOF_CHUNK_T;// must allocate from the beginning, can not change the nunits first!
//+++++++++++++++++++++++++++++++++++++++++calculate nunits++++++++++++++++++++++++++++++++++++++++++++++++++    
    if (nbytes % SIZEOF_CHUNK_T != 0) {
        nunits = nbytes / SIZEOF_CHUNK_T + 1; // PAGESIZE is the size that morecore can allocate for, if the nbytes is not mutiple of PAGESIZE, so that allocate for mor than one page       
    } else { 
        nunits = nbytes / SIZEOF_CHUNK_T;     // exact bytes of the memory
    }    
//++++++++++++++++++++++++++++++++++++++++calculate nunits+++++++++++++++++++++++++++++++++++++++++++++++

//----------------Reset the Rover Position before doing the Search!----------------------------------------------------
    if (SearchLoc == ROVER_FIRST) {// if the Rover is the last block of the free part and the option is the Rover, not the head
            
            //Don't move
        
    } else if (SearchLoc == HEAD_FIRST) {
    
        while (Rover != &Dummy) {
            Rover = Rover->next;
        }
    
    }
    
//----------------Reset the Rover Position before doing the Search!------------------------------------------------------ 

//+++++++++++++++The Fit type: FIRST_FIT, BEST_FIT, WORST_FIT+++++++++++++++++++++++++++++++
    if (SearchPolicy == BEST_FIT) {
        rec_ptr = BestFit(nunits);
    } else if (SearchPolicy == FIRST_FIT) {
        rec_ptr = FirstFit(nunits);
    } else if (SearchPolicy == WORST_FIT) {
        rec_ptr = WorstFit(nunits);
    }
//+++++++++++++++The Fit type: FIRST_FIT, BEST_FIT, WORST_FIT+++++++++++++++++++++++++++++++       
        
    return rec_ptr;
}

/* prints stats about the current free list
 *
 * -- number of items in the linked list including dummy item
 * -- min, max, and average size of each item (in bytes)
 * -- total memory in list (in bytes)
 * -- number of calls to sbrk and number of pages requested
 *
 * A message is printed if all the memory is in the free list
 */
void Mem_stats(void)
{
    /* TODO calculate the latest stats and put them in the stats struct */
    chunk_t* Record = &Dummy;
    int unit_min = 0;
    int unit_max = 0;
    int unit_sum = 0;
    unit_min = Dummy.size;
    unit_max = Dummy.size;
    stats.numItems = 0;
    do {
        if (Record->size < unit_min) {
            unit_min = Record->size;
        }
        if (Record->size > unit_max) {
            unit_max = Record->size;
        }
        unit_sum += Record->size;
        stats.numItems ++;
        Record = Record->next;
    } while(Record != &Dummy);
    stats.min = unit_min * SIZEOF_CHUNK_T;
    stats.max = unit_max * SIZEOF_CHUNK_T;
    stats.totalBytes = unit_sum * SIZEOF_CHUNK_T;
    stats.average = (unit_sum / stats.numItems) * SIZEOF_CHUNK_T;    
    /* ======= DO NOT MODIFY FROM HERE TO END OF Mem_stats() ======= */
    printf("\n\t\tMP4 Heap Memory Statistics\n"
            "----------------------------------------------------\n\n"
            "\tNumber of blocks in free list: %d\n"
            "\tMin: %d\n"
            "\tMax: %d\n"
            "\tAverage: %d\n"
            "\tTotal bytes in free list: %d\n"
            "\tNumber of sbrk calls: %d\n"
            "\tNumber of requested pages: %d\n"
            "\tHeap status:  %s\n",
            stats.numItems, stats.min, stats.max, stats.average, stats.totalBytes,
            stats.numSbrkCalls, stats.numPages,
            stats.totalBytes == stats.numPages*PAGESIZE ? \
            "all memory is in the heap -- no leaks are possible\n"\
            : "heap is in-use -- leaks are possible\n");
}

/* print table of memory in free list 
 *
 * The print should include the dummy item in the list 
 */
void Mem_print(void)
{
    assert(Rover != NULL && Rover->next != NULL);
    chunk_t *p = Rover;
    chunk_t *start = p; /* save my starting block */
    do {
        printf("p=%p, size=%ld, end=%p, next=%p %s\n", 
                p, p->size, p + p->size, p->next, p->size!=0?"":"<-- dummy");
        p = p->next;
    } while (p != start); /* only 1 time through the list */
    mem_validate();
}

/* This is an experimental function to attempt to validate the free
 * list when coalescing is used.  It is not clear that these tests
 * will be appropriate for all designs.  If your design utilizes a different
 * approach, that is fine.  You do not need to use this function and you
 * are not required to write your own validate function.
 */
void mem_validate(void)
{
    assert(Rover != NULL && Rover->next != NULL);
    assert(Rover->size >= 0);
    int wrapped = FALSE;
    int found_dummy = FALSE;
    int found_rover = FALSE;
    chunk_t *p, *largest, *smallest;

    /* for validate begin at Dummy */
    p = &Dummy;
    do {
        if (p->size == 0) {
            assert(found_dummy == FALSE);
            found_dummy = TRUE;
        } else {
            assert(p->size > 0);
        }
        if (p == Rover) {
            assert(found_rover == FALSE);
            found_rover = TRUE;
        }
        p = p->next;
    } while (p != &Dummy);
    assert(found_dummy == TRUE);
    assert(found_rover == TRUE);

    if (Coalescing) {
        do {
            if (p >= p->next) {
                /* this is not good unless at the one wrap */
                if (wrapped == TRUE) {
                    printf("validate: List is out of order, already found wrap\n");
                    printf("first largest %p, smallest %p\n", largest, smallest);
                    printf("second largest %p, smallest %p\n", p, p->next);
                    assert(0);   /* stop and use gdb */
                } else {
                    wrapped = TRUE;
                    largest = p;
                    smallest = p->next;
                }
            } else {
                assert(p + p->size < p->next);
            }
            p = p->next;
        } while (p != &Dummy);
        assert(wrapped == TRUE);
    }
}

/* vi:set ts=8 sts=4 sw=4 et: */

