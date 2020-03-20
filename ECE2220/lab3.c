/* lab3.c
* Yulin Xiao
* Yulinx
* ECE 222, Fall 2018
* MP3
* Subject: ECE222-1,#3
*
* Purpose:  We will use the psude charactor operation to do kind of 
* encrype the character string including Addition, multipition, division, 
* and power.
*
* Assumptions:
*  #1   The user is prompted to enter a pseudo arithmetic command.  The
*       input must be verified to be grammatically correct.
*
*  #2:  The string and character type library cannot be used under
*       any circumstances.  You are encouraged to develop your own
*       functions to perform any similar operations that are needed.
*
*  #3   No changes to the code in main.  Your code must be placed in
*       functions.  Additional functions are encouraged.
*
* Bugs:
*
* It seems that when I do < testinput they will lose the last charactor
* of the right operand, but when I type it in, it won't happen .
*
*
* Notes:
*
* Shown in every row of programming, the program first reduce the ASCII code
* of each input charactor to find the pseudo charactor from a-z A-U, then I 
* use the submisson to sub the charactor to 0-46 the very tricky thing is 
* that the program has to use mod 46 to prevent the ASCII code to exceed the
* certain amount so that the other programmer will be easier to operate with
* these charactors.
*
* If your formatting is not consistent you must fix it.  You can easily
* reformat (and automatically indent) your code using the astyle
* command.  In a terminal on the command line do
*
*     astyle --style=kr lab3.c
*
* See "man astyle" for different styles.  Replace "kr" with one of
* ansi, java, gnu, linux, or google to see different options.  Or, set up
* your own style.
*/

// do not include any additional libraries
#include <stdio.h>

// do not change these constants
#define MAXLINE 80
#define MAXOPER 13

// named constants and strings
enum operations { NOOP, ADD, MUL, DIV, POW};
const char *operation_str[] = {"Invalid", "+", "*", "/", "^"};

// function prototypes
int process_input(const char *input, char *op_left, char *op_right);
void calc_output(const char *op_l, int op, const char *op_r, char *result);

// do not change any code in main.  We are grading based on the format
// of the output as given in the printfs in main.
int main()
{
    char input_line[MAXLINE];
    char left_operand[MAXOPER];
    char right_operand[MAXOPER];
    char answer[MAXOPER];
    int  operator;

    printf("\nMP3: Arithmetic on GF(47) with + * / ^ using letters\n");
    printf("Commands:\n\tabc+bbc\n\tturtle/frog\n\ttiger^one");
    printf("\tOperands are no more than 12 letters and no spaces\n");
    printf("\tCtrl-d to quit\n\n");
    printf("> ");

    // each call to fgets collects one line of input and stores in input_line
    // BEWARE: fgets includes the end-of-line character '\n' in input_line
    while (fgets(input_line, sizeof input_line, stdin) != NULL) {

        // clear for next round
        left_operand[0] = right_operand[0] = answer[0] = '\0';

        // check for valid grammar
        operator = process_input(input_line, left_operand, right_operand);

        if (operator == ADD || operator == MUL
                || operator == DIV || operator == POW) {

            // print parsed input
            printf("'%s'", left_operand);
            printf(" %s ", operation_str[operator]);
            printf("'%s' => ", right_operand);

            // perform pseudo arithmetic
            calc_output(left_operand, operator, right_operand, answer);

            // print result
            printf("'%s'\n\n", answer);
        } else {
            printf("# %s", input_line);
        }
        printf("> ");
    }
    printf("\nGoodbye\n");
    return 0;
}

/* Parse input of the form SOS where S is a string and O is a character.
 *
 * A string S must consist of up to 12 valid symbols a-z and A-U.
 * The operand O must be one character from: + * / ^
 * Any other characters found in the input, including spaces, are
 * grammatically incorrect and invalidate the input.
 *
 * There must be no spaces anywhere in the input, including between
 * either SO, OS, or leading or trailing spaces.
 *
 * Input: The input string is collected using fgets.  Recall the end-of-line
 *        character is included in the input string and marks the end of
 *        the input.  This string must not be changed.
 *
 * Output: There are three outputs from this function.
 *
 *   The return value is one of NOOP, ADD, MUL, DIV, POW which are
 *      named constants.  If the input is invalid for any reason
 *      then the output must be NOOP.  Otherwise the return value
 *      corresponds to operand O.
 *
 *   If the input is grammatically correct, then two strings are also
 *   returned, one for each of the left and right operands.  If the input
 *   in invalid the two output strings are undefined.
 */
int process_input(const char *input, char *op_left, char *op_right)//input_Line, Left_operand, Right_operand
{
    // replace the following with your code
    int i, j;// the loop integer that used for input strings.
    short int c=0;
    for(i = 0,j = 0; input[i] != '\n'; i ++) { // get rid of the last line feed condition 1: not the end
        if(input[i] != 43 && input[i] != 42 && input[i] != 47 && input[i] != 94 && c==0) { //  condition 2: not the operator and emerge only once!
            if(i<=12) { // condition 3: not exceed 12 left operand
                op_left[i] = input[i];// first store:X operator X > 12 , then forgot that whether it is a-z A-U
                if( !( (input[i] >= 65 && input[i] <= 85) || (input[i] >= 97 && input[i] <= 122) ) ) {
                    return 0;// condition 4: be in range
                }
            } else {
                return 0;// condition 3: exceed the 12 and condition 2: not the operator
            }
        } else if(input[i] == 43 && c == 0) { // condition 2: the operator --> '+' and condition 1: not the line feed did not repeat operator
            c=1;
            op_left[i]='\0';// not useful if you don't use them and return 0
        } else if(input[i] == 42 && c == 0) { // condition 2: the operator --> '*' and condition 1: not the line feed did not repeat operator
            c=2;
            op_left[i]='\0';
        } else if(input[i] == 47 && c == 0) { // condition 2: the operator --> '/' and condition 1: not the line feed did not repeat operator
            c=3;
            op_left[i]='\0';
        } else if(input[i] == 94 && c == 0) { // condition 2: the operator --> '*' and condition 1: not the line feed did not repeat operator
            c=4;
            op_left[i]='\0';
            // the function will end if it reaches the return value
        } else { // codition 2: repeat or operator return already
            if(j<=12) { // condition 3: not exceed 12 left operand
                op_right[j] = input[i];// first store:X operator X > 12 , then forgot that whether it is a-z A-U
                j ++;
                if( !( (input[i] >= 65 && input[i] <= 85) || (input[i] >= 97 && input[i] <= 122) ) ) {
                    return 0;// condition 4: be in range
                }
            }
        }

    }
    op_right[j]='\0';
    return c;
}

/* Pseudo mathematical opertions on the two operands work as follows.
 *
 * Each character is converted to an integer in the range 1...46, where a is 0,
 * b is 1, c is 2, ..., z is 25.  The operation is then performed using
 * math on a finite field with no carries.
 *
 * If the two input strings are not the same length, then each output character
 * beyond the length of the shorter string should be a copy of the character
 * from f(!((op_left[i]>=65&&op_left[i]<=85)||(op_right[i]>the longer string but with the opposite case.
 *
 * Input: The two operand strings and the operator are assumed to be valid (and
 *        are verified as valid in the parse_input function).
 *
 * Output: The final string generated by the above rules is stored in the
 *         output string named result.  The input strings must not be
 *         changed.
 */
void calc_output(const char *l_op, int op, const char *r_op, char *result)
{
    int q=1; // store huge numbers
    int k,s;// counting the loop, the loop variable
    int r,l;// right and left character's number
    int re;// result number
    for(k = 0; l_op [k] != '\0' && r_op [k] != '\0'; k ++) {
        if(l_op [k] >= 65 && l_op [k] <= 85 ) {
            l = l_op [k] - 39; // for A-U
        } else {
            l = l_op [k] - 97;// for a-z
        }
        if(r_op [k] >= 65 && r_op [k] <= 85 ) {
            r = r_op [k] - 39;
        } else {
            r = r_op [k] - 97;
        }
        if(op == 1) {
            re = (r + l) % 47;// adding
            if(re <= 25) {
                result [k] = re + 97;
            } else {
                result [k] = re + 39;
            }
        } else if(op == 2) {
            re = (r * l) % 47;//multiplying
            if(re <= 25) {
                result [k] = re + 97;
            } else {
                result [k] = re + 39;
            }
        } else if(op == 3) {
            if (r == 0) {
                result [k] = 97;//set output to zero
            } else {
                for(s = 0; (( l + 47 * s ) % r) != 0; s ++) {
                    ;//do nothing but looping to find a number that dividable 
                }
                re = (l + 47*s) / r;
                s = 0;
                if(re <= 25) {
                    result [k] = re + 97;
                } else {
                    result [k] = re + 39;
                }
            }
        } else if(op == 4) {
            if(r == 0) {
                result [k] = 98; // set output to one
            } else {
                for(s = 0; s < r; s ++) {
                    q = (q * l) % 47;//powering the left charactor
                }
                re = q;
                q = 1;
                if(re <= 25) {
                    result [k] = re + 97;
                } else {
                    result [k] = re + 39;
                }

            }
        }
    }
    s = k;//temperarily store the counter, ready to cope with input
    if(l_op[k]!='\0'&&r_op[k]=='\0') {
        for(; l_op[k]!='\0'&&r_op[k]=='\0'; k++) { //right operand > left operand
            if(l_op[k]>=97) {
                result [k] = l_op[k] - 32;
            } else {
                result [k] = l_op[k] + 32;
            }
        }
        result [k] = '\0';
    }  
    else if(l_op[s]=='\0'&&r_op[s]!='\0') {
        for(; l_op[s]=='\0'&&r_op[s]!='\0'; s++) { //left operand < right operand
            if(r_op[s]>=97) {
                result [s] = r_op[s] - 32;
            } else {
                result [s] = r_op[s] + 32;
            }

        }
        result [s] = '\0';
    }
    k = 0; //initialize the counter
    s = 0;

}

