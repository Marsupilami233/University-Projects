/* lab4.c
 * Yulin Xiao         
 * Yulinx
 * ECE 2230 Spring 2019
 * MP3
 *
 * Purpose: The file contains a lot of informations about the test plan of MP4, to test the
 * whether the free list are consitent, and test special cases such as boundary conditions 
 * for memory block sizes. For example
 * Allocate blocks 1, 2, and 3
 * Print the free list
 * Free blocks 1 and 3
 * Print the free list and verify the hole between blocks 1 and 3
 * Extend the above with other patterns and sizes
 * Have one trial request a whole page (or a whole page minus space for a header)
 * Have one trial remove all memory from the free list and show the list is empty
 * 
 *
 * This file contains drivers to test the mem.c package for dynamic memory
 * allocation.
 * 
 * The following parameters can be set on the command line.  If not set,
 * default values are used.  
 *
 * If different options are implemented for the memory package, this provides a
 * simple mechanism to change the options.  
 *
 * -f best|first|worst  search policy to find memory block (first by default)
 * -c                   turn on coalescing (off by default)
 *
 * General options for all test drivers
 * -s 19283  random number generator seed
 *
 * The Unit test drivers.  Two example drivers are provided.  You MUST
 *           add one or more additional unit drivers for more detailed testing
 * 
 * -u 0      Tests one allocation for 16 bytes
 * -u 1      Tests 4 allocations including a new page
 *           The student must update this driver to match the details of
 *           his or her design.
 *
 * -u 2      Tests combinations of request the number of bytes that matches a whole page, 
 *           and a size that is one unit smaller than a page and request more bytes that 
 *           in one page to test if the allocation is exact(one PAGESIZE - 1), and 
 *           (one PAGESIZE - 2) so that the free list is left empty once. And then the 
 *           third allocation is one PAGESIZE and one PAGESIZE + 1 to test some bigger size of pages.
 * 
 * -u 3      Tests combinations of requests and frees such that the free list is empty
 *           Tests if the allocation is exact (one PAGESIZE - 1), and (two PAGESIZE - 1)  
 *           so that the free list is left empty twice. And then the third allocation is 
 *           some units of 2 * PAGESIZE + 224 - 2 and 31 to test some bigger size of pages. 
 *           After fourth allocation, the list must be empty with only a dummy block in it.
 * 
 * -u 4      Tests the difference between the best fit ,first fit and worst fit. The remainning 
 *           size will be best:8; first:9, head_first:10; and worst:10. And the difference
 *           between the start of the header will show that my Rover could move around the free list.
 *           This test makes six allocations from the free list with the goal
 *           of making the allocation to have the significant difference between three 
 *           SearchPolicy and two SearchLocation.                 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <ctype.h>

#include "mem.h"

/* prototypes for functions in this file only */
void getCommandLine(int, char**, int*, int*);

int main(int argc, char **argv)
{
    int unit_driver;  /* the unit test number to run */
    int seed;         /* random number seed */
    
    getCommandLine(argc, argv, &unit_driver, &seed);
    printf("Seed: %d\n", seed);
    srand48(seed);

    // The major choices: search policy and coalescing option 
    if (SearchPolicy == BEST_FIT) printf("Best-fit search policy");
    else if (SearchPolicy == FIRST_FIT) printf("First-fit search policy");
    else if (SearchPolicy == WORST_FIT) printf("Worst-fit search policy");
    else {
        fprintf(stderr, "Error with undefined search policy\n");
        exit(1);
    }
    if (SearchLoc == HEAD_FIRST) printf(" starting at head");
    else if (SearchLoc == ROVER_FIRST) printf( "starting at rover");
    else {
        fprintf(stderr, "Error with undefined search starting location\n");
        exit(1);

    }
    if (Coalescing == TRUE) printf(" using coalescing\n");
    else if (Coalescing == FALSE) printf(" without coalescing\n");
    else {
        fprintf(stderr, "Error specify coalescing policy\n");
        exit(1);
    }

    if (unit_driver == 0)
    {
        // unit driver 0: basic test with one allocation and free
        printf("\n----- Begin unit driver 0 -----\n");
        char *string;
        const char msg[] = "hello world 15c";//size is just a chunk_t size
        int len = strlen(msg);
        // add one for null character at end of string 
        string = (char *) Mem_alloc((len+1) * sizeof(char));
        strcpy(string, msg);
        printf("string length=%d\n:%s:\n", len, string);
        printf("\nFree list after first allocation\n");
        Mem_stats();
        Mem_print();
        Mem_free(string);
        printf("\nFree list after first free\n");
        printf("unit driver 0 has returned all memory to free list\n");
        Mem_stats();
        Mem_print();
        string = NULL;
        printf("\n----- End unit driver 0 -----\n");
    }
    else if (unit_driver == 1)
    {
        printf("\n----- Begin unit driver 1 -----\n");
        /* You MUST create at least three new unit drivers.

         Here is an example of a unit test driver.

         This test makes four allocations from the free list with the goal
         of making the third allocation the correct size so that the free
         list is left empty. The fourth allocation should allocate a new
         page To get this test to work with your code you need to 
         determine two values.
         */
        int unit_size = SIZEOF_CHUNK_T;
        int units_in_first_page = PAGESIZE/unit_size;
        assert(units_in_first_page * unit_size == PAGESIZE);
        printf("There are %d units per page, and the size of chunk_t is %d bytes\n", 
                units_in_first_page, unit_size); 

        int *p1, *p2, *p3, *p4;
        int num_bytes_1, num_bytes_2, num_bytes_3;
        int num_bytes_4;

        // allocate 1st pointer to 1/8 of a page
        num_bytes_1 = (units_in_first_page/8 - 1)*unit_size;
        p1 = (int *) Mem_alloc(num_bytes_1);
        printf("first: %d bytes (%d units) p=%p \n", 
                num_bytes_1, num_bytes_1/unit_size, p1);
        Mem_print();
         
        // allocate for 2nd pointer to 1/2 of a page
        num_bytes_2 = (units_in_first_page/2 - 1)*unit_size;
        p2 = (int *) Mem_alloc(num_bytes_2);
        printf("second: %d bytes (%d units) p=%p \n", 
                num_bytes_2, num_bytes_2/unit_size, p2);
        Mem_print();

        // allocate remaining memory in free list
        num_bytes_3 = units_in_first_page - num_bytes_1/unit_size 
            - num_bytes_2/unit_size - 3;
        num_bytes_3 *= unit_size;
        p3 = (int *) Mem_alloc(num_bytes_3);
        printf("third: %d bytes (%d units) p=%p \n", 
                num_bytes_3, num_bytes_3/unit_size, p3);
        Mem_print();
        printf("unit driver 1: above Mem_print shows empty free list\n");

        // allocate for 4th pointer to 1/4 a page when free list is empty
        num_bytes_4 = (units_in_first_page/4 - 1)*unit_size;
        p4 = (int *) Mem_alloc(num_bytes_4);
        printf("fourth: %d bytes (%d units) p=%p \n", 
                num_bytes_4, num_bytes_4/unit_size, p4);
        Mem_print();

        // next put the memory back into the free list:

        printf("first free of 1/8 a page p=%p \n", p1);
        Mem_free(p1);
        Mem_print();

        printf("second free of 3/8 a page p=%p \n", p3);
        Mem_free(p3);
        Mem_print();

        printf("third free of 1/2 a page p=%p \n", p2);
        Mem_free(p2);
        Mem_print();
        printf("fourth free of 1/4 a page p=%p\n", p4);
        Mem_free(p4);
        printf("unit driver 1 has returned all memory to free list\n");
        Mem_print();
        Mem_stats();
        printf("\n----- End unit test driver 1 -----\n");
    }


    // add your unit test drivers here to test for special cases such as
    //   -- request the number of bytes that matches a whole page, and a 
    //      size that is one unit smaller than a page
    //   -- request more bytes than in one page
    //   -- combinations of requests and frees such that the free list is empty
    //   -- demonstrate all patterns in coalescing
    //   -- show that rover spreads allocations in list and does not cluster
    //      fragments at head of the free list
    
    
    //Here are mine 3 test drivers, Notice all the units = 16 bytes and are EXACTLY as 
    //same as how many user wants, the size of chunks must be user wants + 1  
    
    else if (unit_driver == 2) {
printf("\n----- Begin unit driver 2 -----\n");
        /* I created three new unit drivers.

         Here is a unit test driver to test combinations of 
        -- request the number of bytes that matches a whole page, and a 
           size that is one unit smaller than a page
        -- request more bytes than in one page

         This test makes four allocations from the free list with the goal
         of making the allocation the whole page to test if the allocation is exact
         (one PAGESIZE - 1), and (one PAGESIZE - 2) so that the free list is left empty 
         once. And then the third allocation is one PAGESIZE and one PAGESIZE + 1
         to test some bigger size of pages.  
         */
        int unit_size = SIZEOF_CHUNK_T;
        int units_in_first_page = PAGESIZE/unit_size;
        assert(units_in_first_page * unit_size == PAGESIZE);
        printf("There are %d units per page, and the size of chunk_t is %d bytes\n", 
                units_in_first_page, unit_size); 

        int *p1, *p2, *p3, *p4;
        int num_bytes_1, num_bytes_2, num_bytes_3;
        int num_bytes_4;

        // allocate a 1st page - 1
        num_bytes_1 = (units_in_first_page - 1)*unit_size;
        p1 = (int *) Mem_alloc(num_bytes_1);
        printf("first: %d bytes (%d units) p=%p \n", 
                num_bytes_1, num_bytes_1/unit_size, p1);
        Mem_print();
         
        // allocate for 2nd pointer to a page - 2
        num_bytes_2 = (units_in_first_page - 2)*unit_size;
        p2 = (int *) Mem_alloc(num_bytes_2);
        printf("second: %d bytes (%d units) p=%p \n", 
                num_bytes_2, num_bytes_2/unit_size, p2);
        Mem_print();

        // allocate 3rd pointer to a page
        num_bytes_3 = (units_in_first_page)*unit_size;

        p3 = (int *) Mem_alloc(num_bytes_3);
        printf("third: %d bytes (%d units) p=%p \n", 
                num_bytes_3, num_bytes_3/unit_size, p3);
        Mem_print();
        printf("unit driver 1: above Mem_print shows empty free list\n");

        // allocate for 4th pointer to a page + 1
        num_bytes_4 = (units_in_first_page + 1)*unit_size;
        p4 = (int *) Mem_alloc(num_bytes_4);
        printf("fourth: %d bytes (%d units) p=%p \n", 
                num_bytes_4, num_bytes_4/unit_size, p4);
        Mem_print();

        // next put the memory back into the free list:

        printf("first free of one page - 1 p=%p \n", p1);
        Mem_free(p1);
        Mem_print();

        printf("second free of one page - 2 p=%p \n", p3);
        Mem_free(p2);
        Mem_print();

        printf("third free of one page p=%p \n", p2);
        Mem_free(p3);
        Mem_print();
        printf("fourth free of one page + 1 p=%p\n", p4);
        Mem_free(p4);
        printf("unit driver 2 has returned all memory to free list\n");
        Mem_print();
        Mem_stats();
        printf("\n----- End unit test driver 2 -----\n");        
    }
    
    else if (unit_driver == 3) {
        printf("\n----- Begin unit driver 3 -----\n");
        /* I created three new unit drivers.

         Here is a unit test driver to test combinations of requests and frees such that the free list is empty

         This test makes four allocations from the free list with the goal
         of making the allocation the whole page to test if the allocation is exact
         (one PAGESIZE - 1), and (two PAGESIZE - 1)so that the free list is left empty 
         twice. And then the third allocation is some units of 2 * PAGESIZE + 224 - 2 and 31
         to test some bigger size of pages.  
         */
        int unit_size = SIZEOF_CHUNK_T;
        int units_in_first_page = PAGESIZE/unit_size;
        assert(units_in_first_page * unit_size == PAGESIZE);
        printf("There are %d units per page, and the size of chunk_t is %d bytes\n", 
                units_in_first_page, unit_size); 

        int *p1, *p2, *p3, *p4;
        int num_bytes_1, num_bytes_2, num_bytes_3;
        int num_bytes_4;

        // allocate 1st pointer a page - 1
        num_bytes_1 = (units_in_first_page - 1)*unit_size;
        p1 = (int *) Mem_alloc(num_bytes_1);
        printf("first: %d bytes (%d units) p=%p \n", 
                num_bytes_1, num_bytes_1/unit_size, p1);
        Mem_print();
         
        // allocate for 2nd pointer to two pages - 1
        num_bytes_2 = (units_in_first_page * 2 - 1)*unit_size;
        p2 = (int *) Mem_alloc(num_bytes_2);
        printf("second: %d bytes (%d units) p=%p \n", 
                num_bytes_2, num_bytes_2/unit_size, p2);
        Mem_print();

        // allocate for 3rd pointer to two pages + 224 - 1
        num_bytes_3 = (units_in_first_page*2 + 224 - 1)*unit_size;

        p3 = (int *) Mem_alloc(num_bytes_3);
        printf("third: %d bytes (%d units) p=%p \n", 
                num_bytes_3, num_bytes_3/unit_size, p3);
        Mem_print();
        printf("unit driver 1: above Mem_print shows empty free list\n");

        // allocate for 4th pointer to 31 in
        num_bytes_4 = (31)*unit_size;
        p4 = (int *) Mem_alloc(num_bytes_4);
        printf("fourth: %d bytes (%d units) p=%p \n", 
                num_bytes_4, num_bytes_4/unit_size, p4);
        Mem_print();

        // next put the memory back into the free list:

        printf("first free of one page - 1 p=%p \n", p1);
        Mem_free(p1);
        Mem_print();

        printf("second free of two pages - 1 p=%p \n", p3);
        Mem_free(p2);
        Mem_print();

        printf("third free of one page + 31 - 1 p=%p \n", p2);
        Mem_free(p3);
        Mem_print();
        
        printf("fourth free of two pages + 224 - 2 p=%p\n", p4);
        Mem_free(p4);
        printf("unit driver 3 has returned all memory to free list\n");
        Mem_print();
        Mem_stats();
        printf("\n----- End unit test driver 3 -----\n");        
    }
    
    else if (unit_driver == 4) {
        printf("\n----- Begin unit driver 4 -----\n");
        /* I created three new unit drivers.

         Here is a unit test driver to test the difference between the best fit ,first fit 
         and worst fit. The remainning size will be best:8; first:9, and head_first:10;
         then worst:10. And the difference between the start of the header will show that 
         my Rover could move around the free list
         
         This test makes six allocations from the free list with the goal
         of making the allocation to have the significant difference between three 
         SearchPolicy. 
         
         first allocate: (PAGESIZE/SIZEOF_CHUNK_T + 60) - 1 unit size
         second allocate: (PAGESIZE/SIZEOF_CHUNK_T + 120) - 1 unit size
         third allocate: (PAGESIZE/SIZEOF_CHUNK_T + 180) - 1 unit size
         The memory chunk size after FIRST THREE allocation will be:
         
         
         **     **   **    *     *
 Dummy-> \\ 256 \\ ->\\ 60 \ 196 \ ->
 ^       **     **   **    *     *
 ^       
 ^       **     **   **     *     *
 ^       \\ 256 \\ ->\\ 120 \ 136 \ ->
 ^       **     **   **     *     *         
 ^       
 ^       **     **   **     *    *
 ^       \\ 256 \\ ->\\ 180 \ 76 \ ->>> 
 ^       **     **   **     *    *    |
 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<        
              
                      ^^^^^^^^^^^^                  
         consider the right chunks
                   
         Fourth allocate: (76 - 1) unit size
         Fifth allocate: (60 - 1) unit size
         Sixth allocate: (76 - 1) unit size
         
         The memory chunk size after LAST THREE allocation will be:
                  
         (1).best fit: 1* 196 136    (76 - 76 = nil)
                       2* 196 76     (136 -60 =  76)
                       3* 196        (76 - 76 = nil)Rover_first or head_first are the same
                       
         (2).first fit:1* 120 136 76 (196 - (76))
                       2* 60  136 76 (120 - (60))
                       3* 60  60  76 (136 - (76)) This is head_first. Rover_first will be:
                       
                       1* 196 136    (76 - 76 = nil) (Rover is on the last new node!) 
                       2* 136 136    (196 - 60)      (Rover is moved to next available!)
                       3* 60  136    (136 - 76)      (Rover stays 'cause the node is ready!)
                           
         (3).worst fit:1* 120 136 76 (196 - (76))   
                       2* 120 76  76 (136 - (60))
                       3* 44  76  76 (120 - (76))   Rover_first or head_first are the same
                                                       
         I wanna show that the first fit and the worst fit are very inefficient, they generate
         very similar consequences when in head first, and the best fit is the most efficient. 
         But the first_fit Policy will definitely different from where the Rover starts.
         */
        int unit_size = SIZEOF_CHUNK_T;
        int units_in_first_page = PAGESIZE/unit_size;
        assert(units_in_first_page * unit_size == PAGESIZE);
        printf("There are %d units per page, and the size of chunk_t is %d bytes\n", 
                units_in_first_page, unit_size); 

        int *p1, *p2, *p3, *p4, *p5, *p6;
        int num_bytes_1, num_bytes_2, num_bytes_3;
        int num_bytes_4, num_bytes_5, num_bytes_6;

        // allocate 1st pointer to a page + 60 - 1 units
        num_bytes_1 = (units_in_first_page + 60 - 1)*unit_size;
        p1 = (int *) Mem_alloc(num_bytes_1);
        printf("first: %d bytes (%d units) p=%p \n", 
                num_bytes_1, num_bytes_1/unit_size, p1);
        Mem_print();
         
        // allocate for 2nd pointer to a page + 120 - 1 units
        num_bytes_2 = (units_in_first_page + 120 - 1)*unit_size;
        p2 = (int *) Mem_alloc(num_bytes_2);
        printf("second: %d bytes (%d units) p=%p \n", 
                num_bytes_2, num_bytes_2/unit_size, p2);
        Mem_print();

        // allocate for 3rd pointer to a page + 180 - 1 units
        num_bytes_3 = (units_in_first_page + 180 - 1)*unit_size;

        p3 = (int *) Mem_alloc(num_bytes_3);
        printf("third: %d bytes (%d units) p=%p \n", 
                num_bytes_3, num_bytes_3/unit_size, p3);
        Mem_print();
        printf("unit driver 4: above are FIRST THREE allocation\n");
        // allocate 4th pointer to 76 - 1 units 
        num_bytes_4 = (76 - 1)*unit_size;
        p4 = (int *) Mem_alloc(num_bytes_4);
        printf("fourth: %d bytes (%d units) p=%p \n", 
                num_bytes_4, num_bytes_4/unit_size, p4);
        Mem_print();
         
        // allocate 5th pointer to 60 - 1 units
        num_bytes_5 = (60 - 1)*unit_size;
        p5 = (int *) Mem_alloc(num_bytes_5);
        printf("fifth: %d bytes (%d units) p=%p \n", 
                num_bytes_5, num_bytes_5/unit_size, p5);
        Mem_print();

        // allocate remaining memory in free list
        num_bytes_6 = (76 - 1)*unit_size;
        p6 = (int *) Mem_alloc(num_bytes_6);
        printf("sixth: %d bytes (%d units) p=%p \n", 
                num_bytes_6, num_bytes_6/unit_size, p6);
        Mem_print();
        printf("unit driver 4: above are LAST THREE allocation\n");

        // next put the memory back into the free list:

        printf("first free 256 + 60 - 1 p=%p \n", p1);
        Mem_free(p1);
        Mem_print();

        printf("second free 256 + 120 - 1 p=%p \n", p2);
        Mem_free(p2);
        Mem_print();

        printf("third free 256 + 180 - 1 p=%p \n", p3);
        Mem_free(p3);
        Mem_print();
        
        printf("fourth free 76 - 1 p=%p\n", p4);
        Mem_free(p4);
        Mem_print();
        
        printf("fifth free of 60 - 1 p=%p \n", p3);
        Mem_free(p5);
        Mem_print();
        
        printf("sixth free of 76 - 1 p=%p\n", p4);
        Mem_free(p6);
        
        printf("unit driver 4 has returned all memory to free list\n");
        
        Mem_print();
        Mem_stats();
        printf("\n----- End unit test driver 4 -----\n");        
    }       
    exit(0);
}

/* read in command line arguments.  Note that Coalescing and SearchPolicy 
 * are stored in global variables for easy access by other 
 * functions.
 *
 * -u 0  is for the unit drivers, starting with driver 0
 */
void getCommandLine(int argc, char **argv, int *unit_driver, int *seed)
{
    /* The geopt function creates three global variables:
     *    optopt--if an unknown option character is found
     *    optind--index of next element in argv 
     *    optarg--argument for option that requires argument 
     *
     * The third argument to getopt() specifies the possible argument flags
     *   If an argument flag is followed by a colon, then an argument is 
     *   expected. E.g., "x:y" means -x must have an argument but not -y
     */
    int c;
    int index;

    while ((c = getopt(argc, argv, "s:f:u:h:c")) != -1)
        switch(c) {
            case 'u': *unit_driver = atoi(optarg);     break;
            case 's': *seed = atoi(optarg);            break;
            case 'c': Coalescing = TRUE;               break;
            case 'f':
                  if (strcmp(optarg, "best") == 0)
                      SearchPolicy = BEST_FIT;
                  else if (strcmp(optarg, "first") == 0)
                      SearchPolicy = FIRST_FIT;
                  else if (strcmp(optarg, "worst") == 0)
                      SearchPolicy = WORST_FIT;
                  else {
                      fprintf(stderr, "invalid search policy: %s\n", optarg);
                      exit(1);
                  }
                  break;
            case 'h':
                  if (strcmp(optarg, "rover") == 0)
                      SearchLoc = ROVER_FIRST;
                  else if (strcmp(optarg, "head") == 0)
                      SearchLoc = HEAD_FIRST;
                  else {
                      fprintf(stderr, "invalid search starting location: %s\n", optarg);
                      exit(1);
                  }
                  break;
            case '?':
                  if (isprint(optopt))
                      fprintf(stderr, "Unknown option %c.\n", optopt);
                  else
                      fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
                  /* fall through */
            default:
                  printf("Lab4 command line options\n");
                  printf("General options ---------\n");
                  printf("  -c        turn on coalescing (default off)\n");
                  printf("  -f best|first|worst\n");
                  printf("            search policy to find memory block (first by default)\n");
                  printf("  -h rover|head\n");
                  printf("            starting location for search\n");
                  printf("  -u 0      run unit test driver number 0\n");
                  exit(1);
        }
    for (index = optind; index < argc; index++)
        printf("Non-option argument %s\n", argv[index]);
}

/* vi:set ts=8 sts=4 sw=4 et: */
