/* lab8.c 
 * Yulin Xiao  << replace with your name!
 * Yulinx 
 * ECE 2220, Fall 2018
 * MP8
 *
 * Purpose: A chat program.  For this assignment you add code to
 *          display the text using a split screen
 *
 * Assumptions: The port number MYPORT is assumed to be free.  If you
 *              get a failed to bind message try changing the port number
 *              to a larger value (but just increase by +1 until you find
 *              an open port)
 *
 *              To connect to a remote chat program, the port number use by
 *              both programs must be the same.
 *
 *             When debugging may leave terminal in bad state. To reset do:
 *                 stty sane
 *
 * Command line arguments: The hostname to chat with.
 *   
 * Known bugs: Need to implement curses code.  What about really long lines? 
 * I don't know why the program can only reads one line.
 *
 *
 * You can chat with yourself by using
 *    ./chat localhost
 *
 * to chat between two different machines run chat on each
 * machine with the other machine name as the argument.  However, most
 * packets seem to be blocked by either firewalls or NAT.  Running both
 * programs on different apolloXX.ces.clemson.edu works.
 *
 * This program is based on code posted at Beej's Guide to Network
 * Programming found at http://www.beej.us/guide/bgnet/ 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/wait.h>
#include <curses.h>

#define MYPORT "4950"  // the port users will be connecting to

#define MAXBUFLEN 100

const char *welcome = "Welcome to ECE 2220 chat";
const char *remote_name = "apolloXX";

// Collect input from keyboard and send packet to remote host
//
// input: sockfd  -- a socket has already been set up to the remote host
//
// output: 
//    print input from keyboard to bottom half of screen
//
//    return a flag to indicate the program should quit
//         1 -- user typed "quit" so tell main to end
//         0 -- default: do not quit
//
int chat_talker(int sockfd)
{
    char buf[MAXBUFLEN];
    int numbytes;
    int quit_flag = 0;   // change to 1 when user types quit
    char buf_count[MAXBUFLEN];
    int i;
    char *screen_divider, *addr;
    int t = 0,s,k;
    int count = 0;
    int ch;

//    fgets(buf, sizeof(buf), stdin);
//    buf[strlen(buf)-1] = '\0';  // chomp end of line

    if (strcmp(buf, "quit") == 0) {
        quit_flag = 1;
    }

    if ((numbytes = send(sockfd, buf, strlen(buf), 0)) == -1) {
        perror("chat: send");
        exit(1);
    }
//    printf("\ntalker: sent %d bytes \n", numbytes);
//    printf("next message: ");
//<+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++






    screen_divider = (char *) malloc((COLS+1) * sizeof(char));

    memset(screen_divider, ' ', COLS);  // fill with spaces
    if (COLS < 5) {
        fprintf(stderr, "Screen is too small %d\n", COLS);
    } else if (COLS < strlen(welcome) + strlen(remote_name) + 1) {
        sprintf(screen_divider, "chat");
    } else {
        memcpy(screen_divider, welcome, strlen(welcome));  // finally add 
        addr = screen_divider + COLS - strlen(remote_name);// the remote
        memcpy(addr, remote_name, strlen(remote_name));    // name
    }
    screen_divider[COLS] = '\0';          // last spot with null

    clear();           // Clear the Screen
    move(LINES/2, 0);
    standout();        // makes next line bold
    addstr(screen_divider);
    standend();        // turns off bold
    move(LINES-1, 0);  // Move Cursor to Lower Left

    count = 0;

//<-----------------------------------------------------
        // self-validate part
        // read keyboard input until end of line
        i = 0;
        while((ch = getch()) != '\n') { 
            buf[i++] = ch;         // but what about buffer overflow!
            if (i == MAXBUFLEN - 1) {
                break;
            }
        }
        buf[i] = '\0';
        
        
        move(LINES - 2, 0);
        sprintf(buf_count, "The message sent is: %d bytes", i);// output how many bypes are sent to the server
        addstr(buf_count);


        char **str;
        str = (char **) calloc (LINES/2, sizeof(char *));
        for (t = 0; t < LINES/2; t ++) {

	    str[t] = (char *) calloc (MAXBUFLEN, sizeof(char));
        
	}        
	
        if (count >= LINES/2 - 1) {
            //clear();    // Clear the Screen 
            count = 0;
	    move(LINES/2, 0);   // perform it again!
            standout();        // makes next line bold
            addstr(screen_divider);
            standend();        // turns off bold
            move(LINES-1, 0);  // Move Cursor to Lower Left

        } else {

	    count++;
	    for (t = 0; t < count; t ++) { // t has to be zero
		for (s = 0; s < strlen(buf); s ++) {  // s has to be zero
		    str[t][s] = buf[s]; // store the buf string teperarily into a dynamic allocated double 
                }
            }

		for (k = 0 ; k < count; k ++) {
		    move(LINES - 3 - k, 0);
		    addstr(str[count -1 - k]);
		}

	    
	}
        move(LINES-1, 0);  // Move Cursor to Lower Left
        clrtoeol();        // Clear current line to end of line of the input from keyboard to prevent the interaction
//        refresh();         // Update Display
    
    
    free(screen_divider);
    for (t = 0; t < LINES/2; t ++) {
	free(str[t]);
    }
    free(str);

//<++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    return quit_flag;
}

// Receive packets from a remote host.  
//
// input: sockfd  -- a socket has already been set up to the remote host
//
// output: should print received data to top half of the screen
//
void chat_listener(int sockfd)
{
    char buf[MAXBUFLEN];
    int numbytes;
//    int t,s,k;
    int count = 0;
    if ((numbytes = recv(sockfd, buf, MAXBUFLEN-1 , 0)) == -1) {
        perror("chat: recv");
        exit(1);
    }
//        char **str;
//        str = (char **) calloc (LINES/2, sizeof(char *));
//        for (t = 0; t < LINES/2; t ++) {

//	    str[t] = (char *) calloc (MAXBUFLEN, sizeof(char));
        
//	} 

//    printf("\n\tlistener: packet is %d bytes long\n", numbytes);
    buf[numbytes] = '\0';
//    printf("\tlistener: packet contains \"%s\"\n\n", buf);

    if (count >= LINES/2 - 1) {
        //clear();    // Clear the Screen
        count = 0;

    } else {
	move(LINES/2 - 1 - count, 0);// Move Cursor to Location beyond the input line and the input bytes
        addstr(buf); // Display the Text input
	count++;
	    
    }
    move(LINES - 1, 0);
    refresh();         // Update Display
    

    
}

/* main sets up the structures with IP address information and calls
 * socket, bind, and connect to set up the details of the UDP socket.
 *
 * Use the select() function to wait for input from either the keyboard
 * or from the remote host
 *
 * Changes to main should be minimal.  Just initialize curses
 * and clean up at end.  All other work is in the two
 *    chat_talker
 *    chat_listener
 * functions that collect input that needs to be displayed
 */
int main(int argc, char *argv[])
{
    int sockfd;
    struct addrinfo hints, *addr_result, *p;
    int rv;

    if (argc != 2) {
        fprintf(stderr, "usage: chat hostname\n");
        exit(1);
    }

    // set up details for address information data structure
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    // first: open socket and bind to port on my host so
    // we can receive packets on this port

    // This fills in structure with IP details for the local host
    if ((rv = getaddrinfo(NULL, MYPORT, &hints, &addr_result)) != 0) {
        fprintf(stderr, "getaddrinfo my host: %s\n", gai_strerror(rv));
        return 1;
    }
    // loop through all the results and bind to the first we can
    for(p = addr_result; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                        p->ai_protocol)) == -1) {
            perror("chat: socket");
            continue;
        }
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("chat: bind");
            continue;
        }
        break;
    }
    if (p == NULL) {
        fprintf(stderr, "chat: failed to bind to socket\n");
        return 2;
    }
    freeaddrinfo(addr_result);

    // second: repeat with remote host IP details so we can
    // connect this socket to the remote host only.  As an extension
    // you could use a different port number for the remote host.
    if ((rv = getaddrinfo(argv[1], MYPORT, &hints, &addr_result)) != 0) {
        fprintf(stderr, "getaddrinfo remote host: %s\n", gai_strerror(rv));
        return 1;
    }
    // loop through all the results and connect to the first we can
    for(p = addr_result; p != NULL; p = p->ai_next) {
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("chat: connect");
            continue;
        }
        break;
    }
    if (p == NULL) {
        fprintf(stderr, "chat: failed to connect socket\n");
        return 2;
    }
    freeaddrinfo(addr_result);

    // End of code to set up UDP socket

    fprintf(stderr, "chat: waiting to chat with %s\n", argv[1]);

    // set up master set of file descriptors to use with select
    fd_set master;
    fd_set read_fds;

    FD_ZERO(&master);
    FD_ZERO(&read_fds);
    FD_SET(0, &master);   // standard input is zero
    FD_SET(sockfd, &master);

    int quit_flag = 0;    // changed by chat_talker when user types quit
    initscr();  // Initialize Curses System.  Must be called first to set 
    // global variables such as COLS and LINES
    while(quit_flag == 0) {
        // specify who to monitor on this trial.  We want both
        read_fds = master;
        // block and wait for input from stdin or socket
        if (select(sockfd+1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("chat: select");
            exit(4);
        }

        // which one is set?  Rare, but both could be set
        if (FD_ISSET(0, &read_fds)) {
            quit_flag = chat_talker(sockfd);      // keyboard input
        } 
        if (FD_ISSET(sockfd, &read_fds)) {
            chat_listener(sockfd);                // remote host sent data
        } 
    }
    endwin();    // Quit ncurses system
    close(sockfd);

    return 0;
}
