/* lab2.c
 * Yulin Xiao
 * Yulinx
 * ECE 222, Fall 2018
 * MP2
 *
 * Purpose:  To use the binary operation encode the input charactor to see if there is an error occurred . Then we also use the binary operations
 * to correct the errors from the input, and convert the correct input.
 *
 * Assumptions:
 *  #1:  We provide a user to enter commands:
 *            enc CUt
 *            dec 0E8A549C
 *            quit
 *       Encoding takes three printable ASCII letters
 *       Decoding takes up to eight HEX digits. If exactly eight digits are
 *            entered, the first digit must be 0 or 1.
 *            Leading zeros can be dropped.
 *
 *  #2:  The string and character type libraries cannot be used except as
 *       already provided.  These libraries are for checking inputs in main
 *       and in printing after decoding is complete.  They cannot be used
 *       for anyother purpose.
 *
 *  #3:  No arrays can be used (excpet as already provided for collecting
 *       keyboard input).  You must use bitwise operators for all encoding
 *       and decoding.  If you want to use an array as a lookup table you
 *       must first propose your design and get it approved.  Designs that
 *       use tables to avoid bitwise operators will not be approved.  There
 *       are many good and simple designs that do not require tables.
 *
 *  #4   No changes to the code in main.  Your code must be placed in
 *       functions.  Additional functions are encouraged.
 *
 * Bugs:
 *
 * See the ECE 223 programming guide
 *
 * If your formatting is not consistent you must fix it.  You can easily
 * reformat (and automatically indent) your code using the astyle
 * command.  If it is not installed use the Ubuntu Software Center to
 * install astyle.  Then in a terminal on the command line do
 *
 *     astyle --style=kr lab1.c
 *
 * See "man astyle" for different styles.  Replace "kr" with one of
 * ansi, java, gnu, linux, or google to see different options.  Or, set up
 * your own style.
 *
 * To create a nicely formated PDF file for printing install the enscript
 * command.  To create a PDF for "file.c" in landscape with 2 columns do:
 *     enscript file.c -G2rE -o - | ps2pdf - file.pdf
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXLINE 100

// function prototypes
void encode(unsigned char first_letter, unsigned char second_letter,
            unsigned char third_letter);
void decode(unsigned int codeword);

int main()
{
    char line[MAXLINE];
    char command[MAXLINE];
    char inputcs[MAXLINE];
    int  items;
    int i, invalid;
    unsigned int codeword;

    printf("\nMP2: encoding and decoding (29, 24) Hamming code.\n");
    printf("Commands:\n\tenc 3-letters\n\tdec 8-hex-digits\n\tquit\n");

    // each call to fgets, collects one line of input and stores in line
    while (fgets(line, MAXLINE, stdin) != NULL) {
        items = sscanf(line, "%s%s", command, inputcs);
        if (items == 1 && strcmp(command, "quit") == 0) {
            break;
        } else if (items == 2 && strcmp(command, "enc") == 0) {
            // encoding
            if (strlen(inputcs) != 3 || !isprint(inputcs[0]) ||
                    !isprint(inputcs[1]) || !isprint(inputcs[2])) {
                printf("Invalid input to encoder: %s\n", inputcs);
            } else {
                encode(inputcs[0], inputcs[1], inputcs[2]);
            }
        } else if (items == 2 && strcmp(command, "dec") == 0) {
            // decoding: convert hex digits to integer
            items = sscanf(inputcs, "%x", &codeword);
            if (items != 1 || strlen(inputcs) > 8) {
                printf("Invalid input to decoder: %s\n", inputcs);
            } else {
                // verify all digits are hex characters because
                // scanf does not reject invalid letters
                for (i=0, invalid=0; i < strlen(inputcs) && !invalid; i++) {
                    if (!isxdigit(inputcs[i]))
                        invalid = 1;
                }
                // if 8 digits, leading digit must be 1 or 0
                if (invalid) {
                    printf("Invalid decoder digits: %s\n", inputcs);
                } else if (strlen(inputcs) == 8 && inputcs[0] != '1'
                           && inputcs[0] != '0') {
                    printf("Invalid decoder leading digit: %s\n", inputcs);
                } else {
                    decode(codeword);
                }
            }
        } else {
            printf("# :%s", line);
        }
    }
    printf("Goodbye\n");
    return 0;
}

/* encode: calculates parity bits and prints codeword
 *
 * input: three ASCII characters
 * assumptions: input is valid
 *
 * Example: if input letters are is 'C', 'U', and 't'
 * the final print must be:
 *  ---01110 10001010 01010100 10011100
 *  Codeword: 0x0E8A549C
 */
void encode(unsigned char first_letter, unsigned char second_letter,
            unsigned char third_letter)
{
    // you must construct the codeword
    unsigned int codeword = 0;
    int i = 0;  // dummy variable, I changed it to my initialize use
    int a, b, c ; // to store in the ASCII number of the charactor (first_number,second_number,third_number)
    int j, k, t, temp, s;// j is the another looping variable for constructions of binary number using binary operation:shift right
    int P1, P2, P4, P8, P16;// those variable is parity bits
    printf("%9s%9c%9c%9c\n", "Encoding:", third_letter, second_letter, first_letter);
    printf(" 0x    00%9x%9x%9x\n", third_letter, second_letter, first_letter);


    a = first_letter;
    b = second_letter;
    c = third_letter;


    // print out each binary number at a time.
    j = sizeof(unsigned int)*8/4;// char has 4 bits, in whole numbers we have 32 bits for unsigned int,j = 8
    printf(" -------- ");
    for(k = 0; k < j; k ++) {
        printf("%d",c >> (j-k-1)&0x1);
    }
    printf(" ");
    for(k = 0; k < j; k ++) {
        printf("%d",b >> (j-k-1)&0x1);
    }
    printf(" ");
    for(k = 0; k < j; k ++) {
        printf("%d",a >> (j-k-1)&0x1);
    }
    printf("\n");
    k = 0;                       // the 24 bits of Information word has generated, the recycled variable should be initialized

    // insert the hamming code
    // a,b,c could be used repeatly so that, we could take down the lost bits
    // calculate the parity bits for P16!

    temp = (c << 16)+(b << 8) + a; // temp = information code in integer number, 32 bits

    t = temp & 0xaaad5b; // using 'and' operation to yank out the bits to add up with P1
    for(j = 24; j > 0; j --) {
        k = k + (t >> (j-1)&0x1);
    }
    if((k+i) % 2) {
        i = 1;
    } else {
        i = 0;
    }
    k = 0;
    printf("P1 : %d\n", i);
    P1 = i;
    i = 0;
    t = temp & 0x33366d;// using 'and' operation to yank out the bits to add up with P2
    for(j=24; j>0; j--) {
        k = k + (t >> (j-1)&0x1);// add up every bits, to see if the sum is odd or even
    }
    if((k+i) % 2) {
        i = 1;
    } else {
        i = 0;
    }
    k = 0;
    printf("P2 : %d\n", i);
    P2 = i;
    i = 0;
    t = temp & 0xc3c78e;// using 'and' operation to yank out the bits to add up with P4
    for(j=24; j>0; j--) {
        k = k + (t>>(j-1)&0x1);// add up every bits, to see if the sum is odd or even
    }
    if((k+i) % 2) {
        i = 1;
    } else {
        i = 0;
    }
    k = 0;
    printf("P4 : %d\n", i);
    P4 = i;
    i = 0;
    t = temp & 0xfc07f0;// using 'and' operation to yank out the bits to add up with P8
    for(j=24; j>0; j--) {
        k = k + (t >> (j-1)&0x1);// add up every bits, to see if the sum is odd or even
    }
    if((k+i) % 2) {
        i = 1;
    } else {
        i = 0;
    }
    k = 0;
    printf("P8 : %d\n", i);
    P8 = i;
    i = 0;
    t = temp & 0xfff800;// using 'and' operation to yank out the bits to add up with P16
    for(j=24; j>0; j--) {
        k = k + (t >> (j-1)&0x1);// add up every bits, to see if the sum is odd or even
    }
    if((k+i) % 2) {
        i = 1;
    } else {
        i = 0;
    }
    k = 0;
    printf("P16: %d\n", i);
    P16 = i;
    i = 0;// initialization
    t = 0;// initialization
    // printf the codeword in binary bits
    printf(" ---");
    for(k = 24; k > 0; k --) {
        printf("%d",s=temp>>(k-1)&0x1);// print every bits in information word, then use switch
        // to add up parity bits and spaces. k is used to define
        // which bits they are counting now
        t ++; // store the 0 or 1 every bits of the 24bits
        s <<= (29-t);
        codeword += s;
        s = 0;
        switch(k) {
        case 20:
            printf(" ");
            break;
        case 12:
            printf(" %d", s = P16);
            t ++;
            s <<= (29-t);
            //s=P16;
            codeword += s;
            s = 0;
            break;
        case 5:
            printf(" %d", s = P8);
            t ++;
            s <<= (29-t);
            //s=P8;
            codeword += s;
            s = 0;
            break;
        case 2:
            printf("%d", s = P4);
            t ++;
            s <<= (29-t);
            //s=P4;
            codeword +=s;
            s = 0;
            break;
        case 1:
            printf("%d", s = P2);
            t ++;
            s <<= (29-t);
            //s=P2;
            codeword += s;
            s = 0;
            printf("%d", s = P1);
            t ++;
            s <<= (29-t);
            //s=P1;
            codeword += s;
            s = 0;
            break;
        default:
            ;
            break;
        }
    }






    // print the information word in binary form with spaces

    // print the parity bits, one bit per line.  Do not change
    // the format, but you must change the dummy variable i
    // to match your design



    // print the codeword bits in binary form with spaces





    // print the codeword in hex format89
    printf("\n");
    printf(" Codeword: 0x%.8X\n", codeword);
    printf("\n");
}

/* decode: checks parity bits and prints information characters
 *
 * input: A 29-bit codeword
 * assumptions: the codeword has either no or only one error.
 *
 *              the information characters may not be printable
 *
 * FYI: when a codeword has more than one error the decoding algorithm
 * may generate incorrect information bits.  In a practical system
 * inforamtion is grouped into multiple codewords and a final CRC
 * code verifies if all codewords are correct.  We will not
 * implement all of the details of the system in this project.
 *
 * Example: if the codeword is 0x0E8A549C
 * the final print must be:
 *  No error
 *  -------- 01110100 01010101 01000011
 *  Information Word: 0x745543 (CUt)
 *
 * Example with one error in codeword bit 21: 0x0E9A549C
 * Notice the 8 in the previous example has been changed to a 9
 * the final print must be:
 *  Corrected bit: 21
 *  -------- 01110100 01010101 01000011
 *  Information Word: 0x745543 (CUt)
 */
void decode(unsigned int codeword)
{

    unsigned int info_word = 0;
    unsigned char first_letter = 0x42;
    unsigned char second_letter = 0x61;
    unsigned char third_letter = 0x64;
    int bit_error_location = 21;

    int i = 0;// dummy variable
    int j, k = 0,t = 0,temp,s = 0;// j is for loop pointer, k is for calculating the '0's and '1's, to see the correct parity number really is.
    int P1, P2, P4, P8, P16; // the parity bits
    printf("Decoding: 0x%.8X\n", codeword);

    temp = codeword;
    // print out the Error bits and information word, and determine the 3 ASCII characters in binary operation


    t = temp & 0x15555555; // caculate P1, with bit No.29, 27, 25, 23, 21, 19, 17, 15, 13, 11, 9, 7, 5, 3
    for(j = 29; j > 0; j --) {
        k = k + (t >> (j-1)&0x1);
    }
    if(k % 2) {
        i = 1;
    } else {
        i = 0;
    }
    k = 0;
    printf("E1 : %d\n", i);
    P1 = i;
    i = 0;
    t = temp & 0x06666666;// caculate P2, with bit No.27, 26, 23, 22, 19, 18, 15, 14, 11, 10, 7, 6, 3
    for(j = 29; j > 0; j --) {
        k = k + (t >> (j-1)&0x1);
    }
    if(k % 2) {
        i = 1;
    } else {
        i = 0;
    }
    k = 0;
    printf("E2 : %d\n", i);
    P2 = i;
    i = 0;
    t = temp & 0x18787878;// caculate P4, with bit No.29, 28, 23, 22, 21, 20, 15, 14, 13, 12, 7, 6, 5
    for(j = 29; j > 0; j --) {
        k = k + (t >> (j-1)&0x1);
    }
    if(k % 2) {
        i = 1;
    } else {
        i = 0;
    }
    k = 0;
    printf("E4 : %d\n", i);
    P4 = i;
    i = 0;
    t = temp & 0x1f807f80;// caculate P8, with bit No.29, 28, 27, 26, 25, 24, 15, 14, 13, 12, 11, 10, 9
    for(j = 29; j > 0; j --) {
        k = k + (t >> (j-1)&0x1);
    }
    if(k % 2) {
        i = 1;
    } else {
        i = 0;
    }
    k = 0;
    printf("E8 : %d\n", i);
    P8 = i;
    i = 0;
    t = temp & 0x1fff8000; // caculate P16, with bit No.29, 28, 27, 26, 25, 24, 23, 22, 21, 19, 18, 17
    for(j = 29; j > 0; j --) {
        k = k + (t >> (j-1)&0x1);
    }
    if(k % 2) {
        i = 1;
    } else {
        i = 0;
    }
    k = 0;
    printf("E16: %d\n", i);
    P16 = i;
    i = 0;
    bit_error_location = (P16 << 4) + (P8 << 3) + (P4 << 2) + (P2 << 1) + P1;
    // Add up the parity bits to figure out which is the bit of the error, if there is some.
    // and determine the three characters
    // here is the template format for the prints.

    if (bit_error_location == 0)
        printf(" No error\n");
    else if (bit_error_location > 0 && bit_error_location <= 29) {
        printf(" Corrected bit: %d\n", bit_error_location);
    } else
        printf(" Decoding failure: %d\n", bit_error_location);
    // correction
    if(bit_error_location > 0 && bit_error_location <= 29) {
        if(codeword>>(bit_error_location-1)&0x1) {
            codeword-=1<<(bit_error_location-1);
        } else {
            codeword+=1<<(bit_error_location-1);
        }
    }
    temp=codeword;// the temp stores the codeword for caculating the information word.
    // print the info_word in binary format.
    printf(" -------- ");
    for(k = 29; k > 0; k --) {
        printf("%d",s = temp >> (k-1)&0x1);
        t++;
        // store the 0 or 1 every bits of the 24bits
        s <<= (24-t);
        info_word += s;
        s = 0;
        // As the same as what I do in encode function, I change the for loop for 29 bits,
        // when encountered with parity bits, I skip it and add spaces if needed.
        switch(k) {
        case 22:
        case 13:
            printf(" ");
            break;
        case 17:
        case 9:
        case 5:
            k --;
            break;
        case 3:
            k -= 3;
            break;
        default:
            ;
            break;
        }
    }

    printf("\n");
    // print the information word in hex:
    printf(" Information Word: 0x%.6X", info_word);

    // convert the info_word into three characters for printing
    // only print information word as letters if 7-bit printable ASCII
    // otherwise print a space for non-printable information bits
    third_letter = info_word >> 16;
    second_letter = (info_word >> 8) - (third_letter << 8);
    first_letter = info_word - (second_letter << 8) - (third_letter << 16);

    if ((first_letter & 0x80) == 0 && isprint(first_letter))
        printf(" (%c", first_letter);
    else
        printf(" ( ");
    if ((second_letter & 0x80) == 0 && isprint(second_letter))
        printf("%c", second_letter);
    else
        printf(" ");
    if ((third_letter & 0x80) == 0 && isprint(third_letter))
        printf("%c)\n", third_letter);
    else
        printf(" )\n");
    printf("\n");
}
