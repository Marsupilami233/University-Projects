/* lab6.c template
 * Yulin Xiao
 * Yulinx
 * ECE 2220, Fall 2018
 * MP6
 *
 * Purpose: To simulate the port scan attack that happens during our daily lives,
 * Then trying to analysis the ports that are determined to be attacked.
 * Actually, this lab is designed to first generate ramdom "packages", that is the
 * struct Records, and find out the differences of stability between the bubblesort and qsort.
 *
 * Assumptions: I first use the command line arguments as the pipeline to my function of
 * four kinds of sort, then use the pipling to direct the output to the standard streams or
 * files to analyse the attacked ports.
 *
 * Command line arguments:         Usage: lab6 sort-field [sort-type]
 *                                              Sort-field
 *                                         1   seqnum
 *                                         2   threat score
 *                                         3   DNS name
 *                                         4   IP addresses
 *                                         5   Port Scan Analysis
 *                                              Sort-type
 *                                         0   do not sort just print data
 *                                         1   qsort (default)
 *                                         2   bubble-sort
 *
 * Known bugs: None
 *
 * Performance Evaluation: Insert your table of run times for sorting performance.
 *                         For each of the 4 field values, determine the largest
 *                         list size that can be sorted in less than one second
 *                         for that field. Repeat for each type of sort.
 *                         Approximate sizes are acceptable as long as you are
 *                         within 5% of the answer.
 *
 *    Sort-field          qsort(default)         bubblesort(my function)
 *
 *  1   seqnum               3100000                     13100
 *  2   threat score         3000000                     12800
 *  3   DNS name             2100000                     10300
 *  4   IP addresses         3000000                     12500
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#define DEST_POS 0
#define SRC_POS  1
#define NUMLTRS 8

// for printing IP addresses
#define OCT1(x) ((x & 0xFF000000) >> 24)
#define OCT2(x) ((x & 0xFF0000) >> 16)
#define OCT3(x) ((x & 0xFF00) >> 8)
#define OCT4(x)  (x & 0xFF)

struct Record {
    int seqnum;
    float threat;
    unsigned int addrs[2];
    unsigned short int ports[2];
    char dns_name[NUMLTRS];
};

/* This function verifies that the list is sorted.
 * It also checks that the seqnum's are all present
 *
 * The function produces no output if the list is correct.  It
 * causes the program to terminate and print a line beginning with
 * "Assertion failed:" if an error is detected.
 *
 * The checks are not exhaustive, so an error may still exist in the
 * list even if these checks pass.
 *
 * YOU MUST NOT CHANGE THIS FUNCTION.  WE USE IT DURING GRADING TO VERIFY THAT
 * YOUR LIST IS CONSISTENT.
 */
void validate_list(struct Record *ptr, int records,
                   int (*fcomp)(const void *, const void *))
{
    int i;
    int * seqnum_list = (int *)calloc(records, sizeof(int));
    if (seqnum_list == NULL) {
        fprintf(stderr, "lab6: Cannot malloc array of size %d\n", records);
        exit(1);
    }
    for(i = 0; i < records; i++) {
        if (i != records - 1)
            assert(fcomp(ptr+i, ptr+i+1) <= 0);
        assert(ptr[i].seqnum > 0 && ptr[i].seqnum <= records);
        seqnum_list[ptr[i].seqnum-1] = 1;
    }
    for(i = 0; i < records; i++)
        assert(seqnum_list[i] == 1);
    free(seqnum_list);
}

/* turn the binary data into formated ASCII text and send to sdtout
 *
 * This function is complete.
 *
 * YOU MUST NOT CHANGE THIS FUNCTION.  WE USE IT DURING GRADING TO VERIFY THAT
 * YOUR LIST IS CONSISTENT.
 */
void print_records(struct Record *S, int nrecs)
{
    int i;
    unsigned int a;
    for (i = 0; i < nrecs; i++) {
        printf("\nS[%d]->seqnum = %d threat = %g\n", i, S[i].seqnum, S[i].threat);
        a = S[i].addrs[DEST_POS];
        printf("       dest IP = %d.%d.%d.%d", OCT1(a),OCT2(a),OCT3(a),OCT4(a));
        a = S[i].addrs[SRC_POS];
        printf(" src IP  = %d.%d.%d.%d\n", OCT1(a),OCT2(a),OCT3(a),OCT4(a));
        printf("       ports: dest = %d src = %d\n", S[i].ports[DEST_POS], S[i].ports[SRC_POS]);
        printf("       dns_name = %s\n", S[i].dns_name);
    }
}

/* My bubble sort algorithm goes here
 *
 * The arguments must be exactly as defined here.
 *
 * Input:
 *    ptr:     points to the start of the array
 *    records: the number of elements in the array
 *    fcomp:   function pointer to comparison function
 *
 * Assumptions:
 *    the function sorts an array of type struct Record.
 *
 *    The comparison function must return an integer less than, equal to,
 *    or greater than zero if the first argument should appear in the list
 *    respectively before, no change, or after the second.
 */
void bubblesort(struct Record *ptr, int records,
                int (*fcomp)(const void *, const void *))
{
    struct Record tmp;
    int i = 0,j = 0;
    for(i = 0; i < records - 1; i ++) {
        for(j = 0; j < records - 1 - i; j ++) {
            int ret = fcomp(ptr+j, ptr+j+1);
            if(ret > 0) {
                tmp = ptr[j];
                ptr[j] = ptr[j + 1];
                ptr[j + 1] = tmp;
            }
        }
    }

}


/* An example of a function to compare records for use with qsort
 * or bubble sort.
 *
 * The function returns one of three values:
 *    -1 : record a should be before record b
 *     0 : the two records have the same ranking
 *     1 : record a should be after record b
 */
int SeqNumCompare(const void *a, const void *b)
{
    struct Record *sa, *sb;

    sa = (struct Record *) a;
    sb = (struct Record *) b;

    if (sa->seqnum < sb->seqnum) return -1;
    else if (sa->seqnum > sb->seqnum) return 1;
    else return 0;
}

int ThreatCompare(const void *a, const void *b)
{
    struct Record *sa, *sb;

    sa = (struct Record *) a;
    sb = (struct Record *) b;

    if (sa->threat < sb->threat) return -1;
    else if (sa->threat > sb->threat) return 1;
    else return 0;
}

int DNSNameCompare(const void *a, const void *b)
{
    struct Record *sa, *sb;
    sa = (struct Record *) a;
    sb = (struct Record *) b;
    for (int i = 0; sa->dns_name[i] != '\0' && sb->dns_name[i] != '\0'; i ++) {
        if(sa->dns_name[i] < sb->dns_name[i])
            return -1;
        else if(sa->dns_name[i] > sb->dns_name[i])
            return 1;
    }
    return 0;
}


int AddrCompare(const void *a, const void *b)
{
    struct Record *sa, *sb;

    sa = (struct Record *) a;
    sb = (struct Record *) b;

    if (sa->addrs[DEST_POS] < sb->addrs[DEST_POS]) return -1;
    else if (sa->addrs[DEST_POS] > sb->addrs[DEST_POS]) return 1;
    else if (sa->addrs[SRC_POS] < sb->addrs[SRC_POS]) return -1;
    else if (sa->addrs[SRC_POS] > sb->addrs[SRC_POS]) return 1;
    else return 0;

}

// Added to sort the dest_ip, src_ip and destination port
int AddrPortCompare(const void *a, const void *b)
{
    struct Record *sa, *sb;
    sa = (struct Record *) a;
    sb = (struct Record *) b;
    if (sa->addrs[DEST_POS] < sb->addrs[DEST_POS]) return -1;
    else if (sa->addrs[DEST_POS] > sb->addrs[DEST_POS]) return 1;
    else if (sa->addrs[SRC_POS] < sb->addrs[SRC_POS]) return -1;
    else if (sa->addrs[SRC_POS] > sb->addrs[SRC_POS]) return 1;
    else if (sa->ports[DEST_POS] < sb->ports[DEST_POS]) return -1;
    else if (sa->ports[DEST_POS] > sb->ports[DEST_POS]) return 1;
    else return 0;
}
/*----------------------------------------------------------*/
int main(int argc, char *argv[])
{
    int sort_field = 0;
    int sort_type  = 1;
    clock_t start, end;

    if (argc != 2 && argc != 3) {
        fprintf(stderr, "Usage: lab6 sort-field [sort-type]\n");
        fprintf(stderr, "Sort-field\n");
        fprintf(stderr, "  1   seqnum\n");
        fprintf(stderr, "  2   threat score\n");
        fprintf(stderr, "  3   DNS name\n");
        fprintf(stderr, "  4   IP addresses\n");
        fprintf(stderr, "  5   Port Scan Analysis\n");
        fprintf(stderr, "Sort-type\n");
        fprintf(stderr, "  0   do not sort just print data\n");
        fprintf(stderr, "  1   qsort (default)\n");
        fprintf(stderr, "  2   bubble-sort\n");

        exit(1);
    }
    sort_field = atoi(argv[1]);
    if (sort_field < 1 || sort_field > 5) {
        fprintf(stderr, "lab6: Invalid sort field %d\n", sort_field);
        exit(1);
    }
    if (sort_field == 5 && argc != 2) {
        fprintf(stderr, "lab6: Invalid port scan attack analysis %d\n", sort_field);
        exit(1);
    }
    if (argc == 3) {
        sort_type = atoi(argv[2]);
        if (sort_type < 0 || sort_type > 2) {
            fprintf(stderr, "lab6: Invalid sort type %d\n", sort_type);
            exit(1);
        }
    }

    int num_records = -1;
    int num_items = -1;
    struct Record *rec_ptr;

    num_items = fread(&num_records, sizeof(num_records), 1, stdin);
    if (num_items != 1) {
        fprintf(stderr, "lab6: Error fread failed to read number records %d\n", num_items);
        exit(1);
    }
    if (num_records < 2) {
        fprintf(stderr, "lab6: Error invalid number records %d\n", num_records);
        exit(1);
    }
    printf("lab6: num records %d\n", num_records);

    /* Notice this is the same approach we used in MP5 except here
     * we can read all the the records with one call to fread.
     */
    rec_ptr = (struct Record *) malloc(num_records * sizeof(struct Record));
    num_items = fread(rec_ptr, sizeof(struct Record), num_records, stdin);
    if (num_items != num_records) {
        fprintf(stderr, "lab6: failed to read all records %d\n", num_items);
        exit(1);
    }

    // The code in main from the beginning until here is complete
    // Your changes start below

    if (sort_type == 1) {  // qsort
        // this is the sort of the SeqNum
        // this is the correct way to time qsort
        if (sort_field == 1) {
            start = clock();
            qsort(rec_ptr, num_records, sizeof(struct Record), SeqNumCompare);
            end = clock();

            // you must print using exactly this format
            fprintf(stderr, "lab6: qsort time=%g for %d records\n",
                    1000*((double)(end-start))/CLOCKS_PER_SEC, num_records);

            // after sorting you must validate that the list is sorted
            validate_list(rec_ptr, num_records, SeqNumCompare);
        }
        // this is the sort of the threat score
        // this is the correct way to time qsort
        if (sort_field == 2) {
            start = clock();
            qsort(rec_ptr, num_records, sizeof(struct Record), ThreatCompare);
            end = clock();

            // you must print using exactly this format
            fprintf(stderr, "lab6: qsort time=%g for %d records\n",
                    1000*((double)(end-start))/CLOCKS_PER_SEC, num_records);

            // after sorting you must validate that the list is sorted
            validate_list(rec_ptr, num_records, ThreatCompare);
        }
        if (sort_field == 3) {
            start = clock();
            qsort(rec_ptr, num_records, sizeof(struct Record), DNSNameCompare);
            end = clock();

            // you must print using exactly this format
            fprintf(stderr, "lab6: qsort time=%g for %d records\n",
                    1000*((double)(end-start))/CLOCKS_PER_SEC, num_records);

            // after sorting you must validate that the list is sorted
            validate_list(rec_ptr, num_records, DNSNameCompare);

        }

        if (sort_field == 4) {
            start = clock();
            qsort(rec_ptr, num_records, sizeof(struct Record), AddrCompare);
            end = clock();

            // you must print using exactly this format
            fprintf(stderr, "lab6: qsort time=%g for %d records\n",
                    1000*((double)(end-start))/CLOCKS_PER_SEC, num_records);

            // after sorting you must validate that the list is sorted
            validate_list(rec_ptr, num_records, AddrCompare);

        }

    }
    if (sort_type == 2) {  // bubble sort
        if (sort_field == 1) {
            start = clock();
            bubblesort(rec_ptr, num_records, SeqNumCompare);
            end = clock();

            // you must print using exactly this format
            fprintf(stderr, "lab6: bubble time=%g for %d records\n",
                    1000*((double)(end-start))/CLOCKS_PER_SEC, num_records);

            // after sorting you must validate that the list is sorted
            validate_list(rec_ptr, num_records, SeqNumCompare);
        }
        // this is the sort of the threat score
        // this is the correct way to time qsort
        if (sort_field == 2) {
            start = clock();
            bubblesort(rec_ptr, num_records, ThreatCompare);
            end = clock();

            // you must print using exactly this format
            fprintf(stderr, "lab6: bubblesort time=%g for %d records\n",
                    1000*((double)(end-start))/CLOCKS_PER_SEC, num_records);

            // after sorting you must validate that the list is sorted
            validate_list(rec_ptr, num_records, ThreatCompare);
        }
        if (sort_field == 3) {
            start = clock();
            bubblesort(rec_ptr, num_records, DNSNameCompare);
            end = clock();

            // you must print using exactly this format
            fprintf(stderr, "lab6: bubblesort time=%g for %d records\n",
                    1000*((double)(end-start))/CLOCKS_PER_SEC, num_records);

            // after sorting you must validate that the list is sorted
            validate_list(rec_ptr, num_records, DNSNameCompare);

        }
        if (sort_field == 4) {
            start = clock();
            bubblesort(rec_ptr, num_records, AddrCompare);
            end = clock();

            // you must print using exactly this format
            fprintf(stderr, "lab6: bubblesort time=%g for %d records\n",
                    1000*((double)(end-start))/CLOCKS_PER_SEC, num_records);

            // after sorting you must validate that the list is sorted
            validate_list(rec_ptr, num_records, AddrCompare);

        }

    }


    // Somewhere you have to handle the port scan analysis.  You must either
    // use the first print to claim there are no attacks.  Or, you must
    // print one output for each attack discovered and a final count of the number
    // of attacks that are found.

    if (sort_field == 5) {
        //fprintf(stderr,"lab6: no attacks detected for %d records\n", num_records);
        //OR
        unsigned int dest_ip = 0, src_ip = 0;  // source and destination IP address for attack
        int start_port = 0;                  // lowest port num in an attack
        int port_count = 0;                  // size of attack (num consequitive ports scanned)
        int  attack_num = 0, num_attacks = 0;  // count the number of attacks discovered
        //first, we should sort the dest_ip and src_ip and ports.
        // i is the loop variable
        // count is the counter that confirm the consecutive 16 ports
        qsort(rec_ptr, num_records, sizeof(struct Record), AddrPortCompare);
        validate_list(rec_ptr, num_records, AddrPortCompare);

        print_records(rec_ptr, num_records);
        int i;
        int count;
        for (i = 0; i < num_records; i ++) {
            if (rec_ptr[i].addrs[DEST_POS] != rec_ptr[i + 1].addrs[DEST_POS] ||
                    rec_ptr[i].addrs[SRC_POS] != rec_ptr[i + 1].addrs[SRC_POS] ||
                    rec_ptr[i].ports[DEST_POS] + 1 != rec_ptr[i + 1].ports[DEST_POS]) {
                if (count >= 16) {
                    dest_ip = rec_ptr[i].addrs[DEST_POS];
                    src_ip = rec_ptr[i].addrs[SRC_POS];
                    start_port = rec_ptr[i - count].ports[DEST_POS];
                    port_count = count;
                    attack_num ++;
                    fprintf(stderr,"attack %d at %d.%d.%d.%d from %d.%d.%d.%d starting at %d for %d ports\n",
                            attack_num, OCT1(dest_ip),OCT2(dest_ip),OCT3(dest_ip),OCT4(dest_ip),
                            OCT1(src_ip), OCT2(src_ip), OCT3(src_ip), OCT4(src_ip),
                            start_port, port_count);
                    // after printing details of each attack, print how many attacks are found
                    count = 0;

                } else {
                    count = 0;
                }
            } else {
                count ++;
            }

        }

        num_attacks = attack_num;
        if (num_attacks == 0) {
            fprintf(stderr,"lab6: no attacks detected for %d records\n", num_records);
        }
        fprintf(stderr,"lab6: discovered %d attacks\n", num_attacks);
        count = 0;
        attack_num = 0;
        num_attacks = 0;
    }

    // printing the records is required.  If you don't want to save
    // the output, on the shell redirect stdout to /dev/null
    print_records(rec_ptr, num_records);

    free(rec_ptr);

    return EXIT_SUCCESS;
}
