/* MP7 
 * Name: Yulin Xiao
 * Username: Yulinx
 * lab7.c
 *
 * Thoughts: 0.process argument: to generate 4 child processes
 * to different 4 terminals using fork and arrays to store pid
 *           in IT services
 *           1. set initial random value #seeds of time srand(seed);
 *           2. set the alarm(1) for one second.
 *           3. set the initial value of the flag in Handlers
 *           4. if SNMPRang: print out the datas
 *           5. if RERang: Reconfigure
 *           6. if Alarm, do the thing to print the remainning time
 *
 *           in Main
 *           fork() the child process.
 *           wait for the 3 IT services exit value and print the result
 *         
 *           in Cyber Security Center
 *           input the command (sn,rn,kn)
 *           send kill() signal
 * Assumption of the MP7: try to write a game that could avert puesudo Cyber Attack
 *           
 * Bugs: when I first do the  command, it can normally print out things
 * But, when I sent the second command, it abnormally crashed
 */
#include <stdio.h>  // for I/O;
#include <unistd.h> // for kill(pid,n)
#include <stdlib.h> // for atoi()
#include <signal.h> // for signal(signum,handler);
#include <sys/types.h> // all the types.
#include <sys/wait.h> // for wait();
#include <time.h>  // for time();
#include <string.h> // for strcmp();
#define NUMTTYS 4
#define MAXLINE 100

int AlarmRang;
int RERang;
int SNMPRang;
int mark1;//mark for reconfigure
int mark2;//mark for SNMP
int count;
int calc_threat (int);
//declare the functions for signal hanlder and the value of threat.
void AlarmHandler(int);
void ReConfigHandler(int);
void SNMPHandler(int);
int calc_threat(int);

void AlarmHandler(int signum)
{
    AlarmRang = 1;
    count ++;
    alarm(1);

    // in the IT service or Cyber wil be better!
}
void ReConfigHandler(int signum)
{

    if (signum == SIGUSR1) {

        printf("I received the Reconfig Signal!threat -1\n");

        RERang = 1;// <------------------------------------------
    }
}
void SNMPHandler(int signum)
{
    if (signum == SIGUSR2) {

        printf("I received the SNMP package! print the status of the each of the terminal\n");
        SNMPRang = 1;// <-----------------------------------------
    }
}
void IT_Service(int Terminal)
{


    FILE *fpt;
    char strDev[100];
    // determine which ttys are open and available to this user
    sprintf(strDev, "/dev/pts/%d", Terminal);
    if ((fpt = fopen(strDev,"w")) != NULL) {
        //2.fopen ()tty for suboutput  <----------------------------------------------------------------
        // <==================

    } else {
        fprintf(fpt,"failed to open terminal %s\n", strDev);
        exit(1);
    }

    int threat = 1;   //1.initialize the threat to be level of one
    srand((unsigned)time(NULL)); //initialize the drand by srand(unsigned seed)
    signal(SIGALRM, AlarmHandler);//3.install signal handler
    signal(SIGUSR1, ReConfigHandler);
    signal(SIGUSR2, SNMPHandler);
    alarm(1);

//4.set alarm
    while(1) {
        pause();//6. pause wait for the signal input
        if (RERang) {
            RERang = 0;
            if (mark1 == 1) {
                fprintf(fpt,"Cannot reconfigure more than once. You are fired!\n");
                exit(3);
            } else {
                if (threat < 10) {
                    fprintf(fpt,"Threat level is not critical. You are fired!\n");
                    exit(3);
                } else {
                    fprintf(fpt,"Reconfiguring system to thwart attack – this may take a few seconds\n");      //opened terminal, ready to get suboutput
                    threat --;
                    mark1 = 1;
                }
            }
        } else if (SNMPRang) {//not nessassory to increase by one.
            SNMPRang = 0;
            alarm(1);
            mark2 = 1;
            if (count < 5) {

                fprintf(fpt,"Load too high. Threat is increased\n");
                threat ++;
                count = 0;

            } else {
                if (threat >= 10 && threat <= 15) {

                    fprintf(fpt,"Level of threat: Red\n");
                    count = 0;

                } else if (threat >=5 && threat < 10) {

                    fprintf(fpt,"Level of threat: Yellow\n");
                    count = 0;

                } else if ( threat < 5 ) {

                    fprintf(fpt,"Level of threat: Green\n");
                    count = 0;

                } else if (count < 5 && threat <= 15) {

                    fprintf(fpt,"Load too high. Threat is increased\n");
                    threat ++;
                    count = 0;

                }
            }
        }
        // before the executing, we should know that the SNMP signal have been sent, then start to count, so that the alarm should be set in the SNMP request, or set in the while loop, after the
        // SNMP request, using another counter :count ++
        else if (AlarmRang) {
            //alarm and the last SNMP request should be keep track of
            AlarmRang = 0;// set the flag to zero
            count ++;

            if (mark1 == 1) {

                threat --;

            } else {

                threat = calc_threat(threat);
            }
            if (count < 5) {
                fprintf(fpt, "Next Report available in %d seconds\n", count);

            }
            if (threat > 15) {
                fprintf(fpt, "Intruder! Data stolen ...\n");
                exit(4);
            } else if (threat < 10) {
                fprintf(fpt, "Attack averted. Mission Complete\n");
                exit(2);
            }



        }



    }

    fclose(fpt);

}

void Cyber_Ops_Center(int PID[4], int Terminal)
{
    char strDev[100];
    sprintf(strDev, "/dev/pts/%d", Terminal);
    FILE *fp;

    fp = fopen(strDev,"w");

    char cmd[2];

    // here is my code!
    //input from the keybroad in Cyber//I wanna to let it collect from the first to the last.
    fprintf(fp, "Please input the command:(/sn/rn/kn): \n");
    while(1) {

        scanf("%s", cmd);

        if (strcmp(cmd,"s1") == 0) {

            printf ("send SNMP to IT service number 1\n");

            kill(PID[0], SIGUSR2);

        } else if (strcmp(cmd,"s2") == 0) {

            printf ("send SNMP to IT service number 2\n");

            kill(PID[1], SIGUSR2);

        } else if (strcmp(cmd,"s3") == 0) {

            printf ("send SNMP to IT service number 3\n");

            kill(PID[2], SIGUSR2);

        } else if (strcmp(cmd,"r1") == 0) {

            printf ("send Reconfiguration to IT service number 1\n");

            kill(PID[0], SIGUSR1);

        } else if (strcmp(cmd,"r2") == 0) {

            printf ("send Reconfiguration to IT service number 2\n");

            kill(PID[1], SIGUSR1);

        } else if (strcmp(cmd,"r3") == 0) {

            printf ("send Reconfiguration to IT service number 3\n");

            kill(PID[2], SIGUSR1);

        } else if (strcmp(cmd,"k1") == 0) {

            printf ("Terminated IT service 1\n");

            kill(PID[0], SIGKILL);

        } else if (strcmp(cmd,"k2") == 0) {

            printf ("Terminated IT service 2\n");

            kill(PID[1], SIGKILL);

        } else if (strcmp(cmd,"k3") == 0) {

            printf ("Terminated IT service 3\n");

            kill(PID[2], SIGKILL);

        }


        fclose(fp);


    }
    exit(EXIT_SUCCESS);
}

int calc_threat(int threat)
{
    if (drand48() < 0.5) {
        threat ++;
    } else if (threat > 1 && drand48() < 0.6) {
        threat --;
    }
    return threat;
}

int main(int argc, char* argv[])
{
    FILE *fpt[NUMTTYS];// first file pointer
    int ttyindex; // /dev/pts/terminal# 4 6 12 20
    char strDev[100]; // store /dev/pts/
    int Terminal[NUMTTYS]; //store  terminals# into int array
    int term_cnt = 0; //counter for terminal
    int IT;
    if (argc != NUMTTYS+1) {
        printf("Usage: ./lab7 1 2 3 4, where the numbers are the terminal pts values\n");
        exit(1);
    }
    for (term_cnt = 0; term_cnt < NUMTTYS; term_cnt++) { // find the combination of the /dev/pts/#
        ttyindex = -1;
        ttyindex = atoi(argv[term_cnt+1]);
        if (ttyindex < 1) {
            printf("invalid terminal number %s\n", argv[term_cnt+1]);
            exit(1);
        }

        sprintf(strDev, "/dev/pts/%d", ttyindex);

        if ((fpt[term_cnt] = fopen(strDev,"w")) != NULL) {
            Terminal[term_cnt] = ttyindex;

        } else {// behind those lines are useless
            printf("failed to open terminal %s\n", strDev);
            exit(1);
        }
    }
    printf("This program can write to %d terminals\n", term_cnt);   //delete me for test
    for (ttyindex=0; ttyindex < term_cnt; ttyindex++) {
        printf("Terminal[%d] = %d\n", ttyindex, Terminal[ttyindex]);
        fprintf(fpt[ttyindex], "Hello terminal %d, your are %d in my list\n",//replace me
                Terminal[ttyindex], ttyindex);
    }

    // here is my code!
    // collect the data of terminal numbers from argc, atoiargv
    int PID[NUMTTYS];
    int status;
    int exit_status;
    int wait_ret;
    PID[0] = fork(); // terminal 1
    if (PID[0] != 0) {
        PID[1] = fork(); // terminal 2  // always is in the parent process
        if (PID[1] != 0) {
            PID[2] = fork(); // terminal 3  // always is in the parent process
            if (PID[2] != 0) {
                PID[3] = fork(); // terminal 4 // always is in the parent process
                if (PID[3] != 0) {
                    // in parent process
                    //while(1) { // loop                  //  <-------------------------------------------------                                            why loop?
                    wait_ret = wait(&status);    // first letter of the pdf using wait() collect the exit data

                    exit_status = WEXITSTATUS(status);
                    if (exit_status == 4) {
                        if(wait_ret == PID[0]) {
                            IT = 1;
                        } else if(wait_ret == PID[1]) {
                            IT = 2;
                        } else if(wait_ret == PID[2]) {
                            IT = 3;
                        }
                        printf("IT service %d compromised, we are going out of business!\n", IT);//threat > 15
                    }

                    wait_ret = wait(&status);    // first letter of the pdf using wait() collect the exit data

                    exit_status = WEXITSTATUS(status);
                    if (exit_status == 3) {
                        if(wait_ret == PID[0]) {
                            IT = 1;
                        } else if(wait_ret == PID[1]) {
                            IT = 2;
                        } else if(wait_ret == PID[2]) {
                            IT = 3;
                        }
                        printf("Call HR, we need a new cybersecurity expert for service %d\n", IT);//fired
                    }

                    wait_ret = wait(&status);    // first letter of the pdf using wait() collect the exit data

                    exit_status = WEXITSTATUS(status);
                    if (exit_status == 2) {
                        if(wait_ret == PID[0]) {
                            IT = 1;
                        } else if(wait_ret == PID[1]) {
                            IT = 2;
                        } else if(wait_ret == PID[2]) {
                            IT = 3;
                        }

                        printf(" Job well done IT specialist %d . Prepare for new attacks! \n",IT);//kn
                    }

                    //}                            //Parent Process : whole PID[4] = {PID1, PID2, PID3, PID4(Cyber)}
                } else Cyber_Ops_Center(PID,Terminal[0]);//Terminal 0 PID[4] PID = {PID1, PID2, PID3, 0}
            } else IT_Service(Terminal[3]);//Terminal 3 s3 r3 k3  PID = {PID1, PID2, 0}
        } else IT_Service(Terminal[2]);//Terminal 2 s2 r2 k2  PID = { PID1,0 }
    } else IT_Service(Terminal[1]);// Terminal 1 s1 r1 k1 PID = {0}

    return 0;

}
