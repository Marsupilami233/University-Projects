/* lab4.c template
 * Yulin Xiao
 * Yulinx
 * ECE 2220, Fall 2018
 * MP4
 *
 * Purpose: A simple file editor that can handle files with characters
 *          that are not printable.
 *
 *          The editor can find and replace any byte in the file.  In
 *          addition it can find any string.
 *
 * Assumptions:
 *     input file is read in as bytes and some operations are to take
 *
 * Command line argument
 *   name of file to read
 *
 * Bugs:
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXLINE 128

/* function prototypes */
int find_next_byte_match(int *, char byte[], char * mem_start, int mem_size);
int find_next_string(int *position, char str[], char * mem_start, int mem_size);
void replace_this_byte(int position, char *byte, char *mem_start, int mem_size);
void print_line(int position, char *mem_start, int mem_size);
void write_file(char * filename, char * mem_start, int file_size);
char *store_file(char * filename, int * file_size);
void print_menu(void);
int valid_filename(char *filename);


/*----------------------------------------------------------*/
/* Here is a sketch for a start to searching for a byte
 *
 * Search for a matching byte starting after position.  Wrap around
 *    to beginning of memory until found or return to starting position
 *
 * input:
 *     byte is a string with the hex characters for the byte to find
 *     mem_start is starting address of the data
 *     mem_size is the number of bytes in the memory block
 * input/output:
 *     position is the location of the cursor.  If the byte is found
 *     the position is updated to the memory offset for the match
 * return value:
 *     true if byte found
 */
int find_next_byte_match(int *position, char byte[], char * mem_start, int mem_size)
{
    // found = find_next_byte_match(&location, inputcs, file_in_memory, file_size);
    int found = 0;
    int i,asci = 0;//asci stores the ascii number of character
    int te_po;//temperate line position to print the cursor
    // store the charactor to br printed out
    if(byte[0]>=48&&byte[0]<=57) { //convert the 16 hexdecimal number from ascii to 16bit decimal number
        asci += ((byte[0] - 48)<<4);
    } else if(byte[0]>=65&&byte[0]<=70) {
        asci += ((byte[0] - 55)<<4);// seimcolom
    }
    if(byte[1]>=48&&byte[1]<=57) { //convert the 16 hexdecimal number from ascii to 16bit decimal number
        asci += (byte[1] - 48);
    } else if(byte[1]>=65&&byte[1]<=70) {
        asci += (byte[1] - 55);
    }
    for(i = *position; i < mem_size; i ++) {
        if(mem_start[i] == asci) {
            if(*position != 0 && i == *position) {
                continue;
            }else{
				found = 1;//set found 1
	            *position = i;//update the position to the address of the matching byte
				break;		
			}

            //showing the address in the memory bracket the value that position points to is constant

        }
    }
    te_po = (*position/16)*16;
    if(found == 1) {
        printf("\n            0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5\n");
        printf("[%6d]",te_po);
        printf(" [");
        for(i = 0; i < 16; i ++) {
            if(isprint(mem_start[te_po + i])) {
                printf("%3c",mem_start[te_po + i]);
            } else {
                printf("   ");//replace the character that can not been input
            }
        }
        printf("  ]");
        printf("\n");
        printf("            ");
        for(i = 0; i < 16; i ++) {
            printf("%02X ",(unsigned char)mem_start[te_po + i]);// preventing the over flow
        }
        printf("\n");
        printf("            ");
        for(i = 0; i < *position - te_po; i ++) {
            printf("   ");
        }
        printf("^");
        printf("\n\n");

    }





    /* if the byte is found then
     *    1.  update the position to the address of the matching byte
     *    2.  print the 16-byte word containing the byte
     *    3.  set found to 1
     * if not found, do not change location and do not print
     */

    return found;
}
/*----------------------------------------------------------*/
/* Here is a sketch for a start to searching for a string
 *
 * Search for a matching string starting after position.  Wrap around
 *    to beginning of memory until found or return to starting position
 *
 *    Note: the string does not match if it overlaps both the characters
 *          at the end and the beginning of memory.
 *
 *    If a match is found, print the first 16-byte word containing the
 *          start of the string and continue to print 16-byte words until
 *          all bytes in the matching string are displayed.
 * input:
 *     str is a string to find
 *     mem_start is starting address of the data
 *     mem_size is the number of bytes in the memory block
 * input/output:
 *     position is the location of the cursor.  If the string is found
 *     the position is updated to the memory offset for the first byte
 *     that matchs the string
 * return value:
 *     true if string found
 */
int find_next_string(int *position, char str[], char * mem_start, int mem_size)
{
    int found = 0;
    int i,j,te_po;
    int count = 1;
    int t = 0;//record the extra lines of output
    for(i = *position; i < mem_size; i++) {
        if(mem_start[i] == str[0]) {// the start of the str = mem_start[i]
            t = i;
            for(j = 1; j < strlen(str); j ++) {
                if(mem_start[i+j] != str[j]) {
                    break;
                }
            }
            if(j == strlen(str)) {
                if(*position != 0 && i == *position) {//can not be repeated, but still not be the first
                    continue;
                } else {
                    break;
                }
            }

        }
    }
    if(j == strlen(str)) {// at the bottom of the str[]
        found = 1;
        *position = t;//update the position to the address of the matching byte
        te_po = (*position / 16) * 16;//showing the address in the memory bracket
    }
    t = ((*position % 16) + strlen(str))/16;
    if(found == 1) {//print the cursor first, then loop (*position % 16) + strlen(str)/16 = ?

        if(t == 0) {//one line output

            printf("\n            0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5\n");
            printf("[%6d]",te_po);
            printf(" [");
            for(i = 0; i < 16; i ++) {
                if(isprint(mem_start[te_po + i])) {
                    printf("%3c", mem_start[te_po + i]);
                } else {
                    printf("   ");
                }
            }
            printf("  ]");
            printf("\n");
            printf("            ");
            for(i = 0; i < 16; i ++) {
                printf("%02X ",(unsigned char)mem_start[te_po + i]);
            }
            printf("\n");
            printf("            ");
            for(i = 0; i < *position - te_po; i ++) {
                printf("   ");
            }
            printf("^");
            if(strlen(str) <= (16 - (*position - te_po))) { //when string length is shorter
                for(i = 0; i < strlen(str)-2; i ++) {
                    printf("  .");
                }
                printf("  |");
                printf("\n\n");
            }
        } else if(t >= 1) {//two or three lines
            printf("\n            0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5\n");
            printf("[%6d]",te_po);
            printf(" [");
            for(i = 0; i < 16; i ++) {
                if(isprint(mem_start[te_po + i])) {
                    printf("%3c", mem_start[te_po + i]);
                } else {
                    printf("   ");
                }
            }
            printf("  ]");
            printf("\n");
            printf("            ");
            for(i = 0; i < 16; i ++) {
                printf("%02X ",(unsigned char)mem_start[te_po + i]);
            }
            printf("\n");
            printf("            ");
            for(i = 0; i < *position - te_po; i ++) {
                printf("   ");
            }
            printf("^");
            for(i = 0; i < 16-(*position-te_po) - 1; i ++) {//-1 means not include ^
                printf("  .");
            }
            printf("\n\n");

            while(t > 1) {
                te_po += 16;
                printf("\n            0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5\n");
                printf("[%6d]",te_po);
                printf(" [");
                for(i = 0; i < 16; i ++) {
                    if(isprint(mem_start[te_po + i])) {
                        printf("%3c", mem_start[te_po + i]);
                    } else {
                        printf("   ");
                    }
                }
                printf("  ]");
                printf("\n");
                printf("            ");
                for(i = 0; i < 16; i ++) {
                    printf("%02X ",(unsigned char)mem_start[te_po + i]);
                }
                printf("\n");
                printf("            ");
                for(i = 0; i < 16; i++) {
                    printf(".  ");
                }
                t--;
                count ++;
            }

            // The last line!
			te_po += 16; 
            printf("\n            0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5\n");
            printf("[%6d]",te_po);
            printf(" [");
            for(i = 0; i < 16; i ++) {
                if(isprint(mem_start[te_po + i])) {
                    printf("%3c",mem_start[te_po + i]);
                } else {
                    printf("   ");
                }
            }
            printf("  ]");
            printf("\n");
            printf("            ");
            for(i = 0; i < 16; i ++) {
                printf("%02X ",(unsigned char)mem_start[te_po + i]);
            }
            printf("\n");
            printf("            ");
            for(i = 0; i < ((strlen(str) + *position) - te_po)-1; i++) {
                printf(".  ");
            }
            printf("|  ");
            printf("\n\n");
        }

        /* if the string is found then
         *    1.  update the location to the first character matching str
         *    2.  print the 16-byte word containing the string and repeat
         *        printing words until all characters in str are displayed
         *    3.  set found to 1
         * if not found, do not change location and do not print
         */

    }
    return found;
}

/*----------------------------------------------------------*/
/* Here is a sketch for a start to replace byte at current location
 *
 * input:
 *     position is the cursor location and the position for substitution
 *     byte is a string with the hex characters for the byte to replace
 *     mem_start is starting address of the data
 *     mem_size is the number of bytes in the memory block
 * input/output:
 */
void replace_this_byte(int position, char *byte, char *mem_start, int mem_size)
{
    /* after the replacement print the 16-byte word containing the byte */
    int i,asci = 0;//asci stores the ascii number of character
    int te_po;//temperate line position to print the cursor
    // store the charactor to br printed out
    if(byte[0]>=48&&byte[0]<=57) { //convert the 16 hexdecimal number from ascii to 16bit decimal number
        asci += ((byte[0] - 48)<<4);
    } else if(byte[0]>=65&&byte[0]<=70) {
        asci += ((byte[0] - 55)<<4);// seimcolom
    }
    if(byte[1]>=48&&byte[1]<=57) { //convert the 16 hexdecimal number from ascii to 16bit decimal number
        asci += (byte[1] - 48);

    } else if(byte[1]>=65&&byte[1]<=70) {
        asci += (byte[1] - 55);
    }
    mem_start[position] = (char) asci;
    te_po = (position/16)*16;
    printf("\n            0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5\n");
    printf("[%6d]",te_po);
    printf(" [");
    for(i = 0; i < 16; i ++) {
        if(isprint(mem_start[te_po + i])) {
            printf("%3c",mem_start[te_po + i]);
        } else {
            printf("   ");
        }
    }
    printf("  ]");
    printf("\n");
    printf("            ");
    for(i = 0; i < 16; i ++) {
        printf("%02X ",mem_start[te_po + i]);
    }
    printf("\n");
    printf("            ");
    for(i = 0; i < position - te_po; i ++) {
        printf("   ");
    }
    printf("^");
    printf("\n\n");
}
/*----------------------------------------------------------*/
/* Here is a sketch for a start to printing a 16-byte aligned
 * line of text.
 *
 * input:
 *     position is the cursor location
 *     mem_start is starting address of the data
 *     mem_size is the number of bytes in the memory block
 *
 * output:
 *     prints a 16-byte line of text that is aligned so that starting
 *     address is a multiple of 16
 */
void print_line(int position, char *mem_start, int mem_size)
{
    int te_po;
    te_po = (position/16)*16;
    int i;
    printf("\n            0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5\n");
    printf("[%6d]",te_po);
    printf(" [");
    for(i = 0; i < 16; i ++) {
        if(isprint(mem_start[te_po + i])) {
            printf("%3c",mem_start[te_po + i]);
        } else {
            printf("   ");
        }
    }
    printf("  ]");
    printf("\n");
    printf("            ");
    for(i = 0; i < 16; i ++) {
        printf("%02X ",mem_start[te_po + i]);
    }
    printf("\n");
    printf("            ");
    for(i = 0; i < position - te_po; i ++) {
        printf("   ");
    }
    printf("^");
    printf("\n\n");

}
/*----------------------------------------------------------*/
/* Write the contents in memory to a file
 *
 * Inputs:
 *     filename: name of output file.  No check is required to
 *               verify that the file does not already exist.  If the
 *               file exists, it will be overwritten
 *     mem_start: starting address of file in memory
 *     file_size: number of bytes to be written to the file
 *
 *  Output:
 *    creates a file in the current working directory.
 */
void write_file(char * filename, char * mem_start, int file_size)
{
    FILE *fileout;

    if ((fileout = fopen(filename, "w")) == NULL) {
        printf("Cannot write to file \"%s\"\n", filename);
        exit (1);
    }
    fileout = fopen(filename, "w");
    fprintf(fileout, "%s\n", mem_start);
    /* like reading in a file but use fprintf instead of fscanf */

    fclose(fileout);
}
/*----------------------------------------------------------*/
/* The function to open the file, find the size of the file,
 * malloc memory to hold the contents of the file.
 *
 * There are two return values
 *   1.  the return value is a pointer to the starting
 *       memory location of the data
 *   2.  the size of the memory block in bytes is also
 *       returned in file_size
 */
char *store_file(char * filename, int * file_size)
{
    FILE *fileinput;
    char    ch;
    int     count;
    char    *ptrFileInMemory;
    if ((fileinput = fopen(filename, "r")) == NULL) {
        printf("Cannot Read from File \"%s\"\n", filename);
        exit (1);
    }
    count = 0;
    while (fscanf(fileinput,"%c",&ch) == 1) {
        count++;
    }
    fclose(fileinput);

    ptrFileInMemory = (char *) malloc (count * sizeof(char));

    if (ptrFileInMemory == NULL) {
        printf("Error: Cannot Allocate Space for File\n");
        exit (1);
    }

    // read in the file and save in memory starting at ptrFileInMemory
    count = 0;
    fileinput=fopen(filename,"r");
    while (fscanf(fileinput,"%c",&ch) == 1) {
        // same as *(ptrFileInMemory + count) = ch;
        ptrFileInMemory[count] = ch;
        // to print address, same as &(ptrFileInMemory[count])
        count++;
    }
    fclose(fileinput);
    *file_size = count;
    return ptrFileInMemory;
}

/*----------------------------------------------------------*/
/*
 * Functions from here to end of this file should NOT be changed
 */
/* print the menu of choices to the user
 *
 */
void print_menu(void)
{
    printf("Simple editor commands\n\n");
    printf("f FF  : Search for next matching byte (in hex) after current location\n");
    printf("/Blue Ridge : Search for next matching string after current location\n");
    printf("r FF  : Replace byte (in hex) at current location\n");
    printf("G num : Goto byte at position 'num' from start of memory\n");
    printf("l num : Move 'num' bytes from current location (can be negative)\n");
    printf("j     : Move forward 16 bytes from current location\n");
    printf("k     : Move backward 16 bytes from current location\n");
    printf("w filename   : Write memory to filename\n");
    printf("q     : Quit\n");
    printf("?     : Print this menu\n");
}
/* Simple test to restrict filename.  This is overly restrictive but
 * avoids difficult strings.
 *
 * Input: a string
 * Output: True if only alpha-numeric, _, -, . characters.  Else false
 *
 * This function is complete: do not change
 */
int valid_filename(char *filename)
{
    char c;
    /* guess max filename length is 128 */
    if (strlen(filename) == 0 || strlen(filename) >= MAXLINE - 1)
        return 0;
    /* make sure first character is alpha numeric */
    if (!isalnum(*filename++)) return 0;
    while((c = *filename++)) {
        if (isalnum(c)) continue;
        if (c == '_' || c == '-' || c == '.') continue;
        return 0;
    }
    return 1;
}

/*----------------------------------------------------------*/
int main(int argc, char *argv[])
{
    char *filename;        /* the input file name */
    char *file_in_memory;  /* starting address of memory block to store file */
    int file_size;
    size_t fn_len;         /* length of the input file name */
    int found = 0;         /* if search was successful */
    int location = 0;      /* current location in memory [0, file_size) */
    int items;
    char line[MAXLINE];
    char command[MAXLINE];
    char inputcs[MAXLINE];
    char junk[MAXLINE];

    if (argc != 2) {
        printf("Usage: lab4 filename\n");
        exit(1);
    }

    /* prepare filename */
    fn_len = strlen(argv[1]);
    /* remember the null */
    filename = (char *) malloc((fn_len + 1) * sizeof(char));
    strcpy(filename, argv[1]);

    /* open file and store in memory starting at pointer */
    file_in_memory = store_file(filename, &file_size);

    print_menu();

    printf("> ");
    while (fgets(line, MAXLINE, stdin) != NULL) {
        printf("\n");
        items = sscanf(line, "%s%s%s", command, inputcs, junk);
        if (items == 2 && strcmp(command, "f") == 0) {
            if (strlen(inputcs) != 2 || !isxdigit(inputcs[0]) ||
                    !isxdigit(inputcs[1])) {
                printf("Invalid byte: %s\n", inputcs);
            } else {
                found = find_next_byte_match(&location, inputcs, file_in_memory, file_size);
                if (!found) {
                    printf("Did not find byte: %s\n", inputcs);
                }
            }
        } else if (strncmp(line, "/", 1) == 0) {
            strcpy(inputcs, line + 1);
            if (inputcs[strlen(inputcs)-1] == '\n')
                inputcs[strlen(inputcs)-1] = '\0';
            found = find_next_string(&location, inputcs, file_in_memory, file_size);
            if (!found) {
                printf("String not found: '%s'\n", inputcs);
            }
        } else if (items == 2 && strcmp(command, "r") == 0) {
            if (strlen(inputcs) != 2 || !isxdigit(inputcs[0]) ||
                    !isxdigit(inputcs[1])) {
                printf("Invalid replace: %s\n", inputcs);
            } else {
                replace_this_byte(location, inputcs, file_in_memory, file_size);
            }
        } else if (items == 2 && strcmp(command, "G") == 0) {
            int new_location = -1;
            new_location = atoi(inputcs);
            if (new_location < 0 || new_location >= file_size) {
                printf("Invalid goto: %s\n", inputcs);
            } else {
                location = new_location;
                print_line(location, file_in_memory, file_size);
            }
        } else if ((items == 1 || items == 2) && strcmp(command, "l") == 0) {
            int new_location, offset = file_size;
            if (items == 2)
                offset = atoi(inputcs);
            else
                offset = 1;
            new_location = location + offset;
            if (new_location < 0 || new_location >= file_size) {
                printf("Invalid lateral move: %s\n", inputcs);
            } else {
                location = new_location;
                print_line(location, file_in_memory, file_size);
            }
        } else if (items == 2 && strcmp(command, "w") == 0) {
            if ( !valid_filename(inputcs)) {
                printf("Invalid write: %s\n", inputcs);
            } else {
                write_file(inputcs, file_in_memory, file_size);
            }
        } else if (items == 1 && strcmp(command, "j") == 0) {
            if (location + 16 >= file_size) {
                printf("Invalid move down: %d\n", location);
            } else {
                location += 16;
                print_line(location, file_in_memory, file_size);
            }
        } else if (items == 1 && strcmp(command, "k") == 0) {
            if (location - 16 < 0) {
                printf("Invalid move up: %d\n", location);
            } else {
                location -= 16;
                print_line(location, file_in_memory, file_size);
            }
        } else if (items == 1 && strcmp(command, "q") == 0) {
            break;
        } else if (items == 1 && strcmp(command, "?") == 0) {
            print_menu();
        } else {
            printf("# :%s", line);
        }
        printf("> ");
    }

    /* for every malloc there must be a free */
    free(file_in_memory);
    free(filename);

    printf("Goodbye\n");
    return EXIT_SUCCESS;
}
