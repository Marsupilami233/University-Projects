/* lab1.c
 * Yulin Xiao 
 * Yulinx         
 * ECE 2220, Fall 2018
 * MP1
 * 
 * Purpose:  To prompt user contact with other agents using encrypted message. 
 *
 * Assumptions: First, we use the while(1) to prompt user to add correct messagesize and keyvalue, if they did not enter properly, the program exits, since using do while() will be to hard     
 * for users to see what they have entered;
 * Second, enter the messages several times, cut them up using enter, receive them using function:fgets(),sscanf(),to flash up the stdin. Then judge them by minus KeyValue to see if it is
 a prime number. Count them up to see the numbers of "signals", and then loop for using again.
 * Third,if the unexpected input has been found, s=-1, it will show 'Corrupted Messages'. 's' acted as a signal.
 *
 * Bugs:  
 *
 * See the ECE programming guide for requirements
 *
 * Are you unhappy with the way this code is formatted?  You can easily
 * reformat (and automatically indent) your code using the astyle 
 * command.  If it is not installed use the Ubuntu Software Center to 
 * install astyle.  Then in a terminal on the command line do
 *     astyle --style=kr lab1.c
 *
 * See "man astyle" for different styles.  Replace "kr" with one of
 * ansi, java, gnu, linux to see different options.  Or, set up your
 * own style.
 *
 * To create a nicely formated PDF file for printing install the enscript 
 * command.  To create a PDF for "file.c" in landscape with 2 columns do:
 *     enscript file.c -G2rE -o - | ps2pdf - file.pdf
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAXLINE 100
#define MINNUM 10
#define MAXNUM 100
#define MINSIZE 6
#define MAXSIZE 12
#define MINKEY 1
#define MAXKEY 5

int main()

{
    char line[MAXLINE];
    int msgsize;
    int keyvalue;
    int signal=0;
    int temp;
    int i;
    int j;
    int msg=0;
    int id;
    int a[]={11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};
    int s=0;
    //first, using the fgets to receive msgsize, keysize, then use i as the loop times to receive the messages 
     fgets(line, MAXLINE, stdin);//user input msgsize
     sscanf(line, "%d", &msgsize);
     while(1){
         if(msgsize>=MINSIZE&&msgsize<MAXSIZE){
             printf("\nMsgsize is set to %d.\n", msgsize);
             break;
         }else{
             printf("\nMsgsize is out of range. Please try again.\n"); 
             exit(0);
             
         } 
     }
     fgets(line, MAXLINE, stdin);//user input keyvalue
     sscanf(line, "%d", &keyvalue);
     
     while(1){
    
         if(keyvalue>MINKEY&&keyvalue<MAXKEY){
             printf("\nKeyValue is set to %d.\n", keyvalue);
             break;
     	 }else{
	         printf("\nKeyValue is out of range. Please try again.\n");
			 exit(0);
    	 }
	 }
	 while(1){
	 

         for(i=0;i<msgsize;i++){
		     fgets(line, MAXLINE, stdin);
             sscanf(line, "%d", &msg);//each ENTER will read from the keyboard
             if(msg!=-2&&msg>MINNUM&&msg<MAXNUM){
             	 temp=msg-keyvalue;       //encryped message
                 for(j=0;a[j]<MAXNUM;j++){
                 	if(temp==a[j]){
                 		signal++;
                 		break;
                 	}
                 }
                
             }else if(msg==-2){
             	 printf("\n>> Agent Center is not safe now. Go find a safe place. Program exit.\n\n\n");
             	 exit(0);
             }else{
			    s=-1;
             }
         }msg=0;
         id=temp+keyvalue;
         if(s==-1){
         	printf("\n");
         	printf(">> Corrupted Message.\n");
         }else if(s==0){
		     switch(signal){
    	     case 1: printf("\n");printf(">> Msg:  I'm safe, all good.\n"); break;
    	     case 2: printf("\n");printf(">> Msg:  Mission Success. Agent ID: %d.\n", id); break;
             case 3: printf("\n");printf(">> Msg:  Don't contact me.\n"); break;
             case 4: printf("\n");printf(">> Msg:  Mission Failed. Agent ID: %d.\n", id); break;
             default: printf("\n");printf(">> Msg:  No Content.    %d\n",signal); break;
             } 
         }
         signal=0;
		 s=0;  
     }

     exit(0);

}
