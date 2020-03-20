/*
 * UserName: Yulinx Slzhu(partner)
 * Name: Yulin Xiao, Shu Zhu(partner)
 * 
 * Project 2
 * 
 * Assumptions: to use the mutex lock and condition variables to do the proceed 
 * threads with different priorities and first come and first serve. So basicly 
 * the threads will have to arrive based on how they come and how they proceed
 * 
 * Bugs: None that I know.
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <assert.h>

#include "plock.h"

/*------------------------EnQueue------------------------------*/
/*
 * This helper function is used to do the EnQueue whenever 
 * a thread is entered into the critical section
 * rover : the pointer to iteratively move through all of the
 * nodes.
 * rover->p : the pointer to point to the rover's previous node
 * new_node : the newly created node for CV's waiting list 
 * The node is stored in the priority queue with the highest 
 * priority is on the front. And when the equal priority thread enters
 * the node should be placed behind it. 
 *
 */
void EnQueue(plock_t* lock, node_t* new_node)
{
	
	node_t* rover = lock->head;
	node_t* roverp = lock->head;
	if (lock->head == NULL) {
		lock->head = new_node;
		new_node->next = NULL;
	} else {
		if(new_node->priority > lock->head->priority) {
			new_node->next = lock->head;
			lock->head = new_node;
		} else {
			while (rover->next != NULL) {
				if (new_node->priority > rover->priority) {
					break;
				}
				roverp = rover;
				rover = rover->next;
			}
			if (rover->next == NULL) {// safe exit
				if (rover == lock->head) {// only one node and priority is equal or larger than
					rover->next = new_node;
					new_node->next = NULL;
				} else if (new_node->priority > rover->priority) {// coincidence
					roverp->next = new_node;
					new_node->next = rover;
				} else {// all large or equal to
					rover->next = new_node;
					new_node->next = NULL;
				}
			} else {// break
				roverp->next = new_node;
				new_node->next = rover;
			} 
		}
	}
} 
/*------------------------EnQueue------------------------------*/

/*------------------------DeQueue------------------------------*/
/*
 * This helper function is used to do the DeQueue, and the operations 
 * are identical to a normal priority queue. 
 */
node_t* DeQueue(plock_t* lock)
{
	node_t* Deq_node = NULL;
	
	if (lock->head == NULL) {
		return Deq_node;
	} else {
		Deq_node = lock->head;// remove from the head
		lock->head = lock->head->next;
		Deq_node->next = NULL;
	}	
	return Deq_node;
}
/*------------------------DeQueue------------------------------*/

/*
 * This function calls malloc() to allocate space for an instance of the
 * plock_t data type, initializes the plock value to FREE, initializes
 * the mutex mlock using the appropriate pthread library call,
 * appropriately initializes the waiting list head pointer, and returns
 * a pointer to the instance.
 */

plock_t* plock_create() 
{

	plock_t* lock = (plock_t*) malloc(sizeof(plock_t));
	lock->value = FREE;
	pthread_mutex_init(&(lock->mlock), NULL);
	lock->head = NULL;
	return lock;
}


/*
 * This function first garbage collects any nodes remaining on the waiting
 * list by destroying the condition variables using the appropriate pthread
 * library call and freeing the nodes. It then destroys the mutex mlock
 * using the appropriate pthread library call and finally frees the plock
 * data structure itself.
 */

/*
 * May need to do Queue Destroy once we know that something is left in the queue
 * After the queue is destroyed, we will have to do the waiting list
 * Before we destroy the waiting list, we have to destroy the CV
 * So: 1.Queue(waiting list) 2.CV 3. waiting list 4.mutex mlock 5. plock ADT  
 */

void plock_destroy(plock_t* lock) 
{
    node_t* DeQ_node = lock->head;
		while(DeQ_node != NULL) {
			DeQ_node = DeQueue(lock);
			pthread_cond_destroy(&(DeQ_node->waitCV));
			free(DeQ_node);
		}
    pthread_mutex_destroy(&(lock->mlock));//4. mutex
		free(lock);//5. plock ADT
}
/*--------++++++++Create a new node for packaging the thread++++++++--------*/
node_t* create_node(int priority) 
{
	node_t* node = (node_t *) malloc(sizeof(node_t));
	node->priority = priority;
	pthread_cond_init(&(node->waitCV), NULL);
	node->next = NULL;
	return node;
}
/*--------++++++++Create a new node for packaging the thread++++++++--------*/

/*
 * This function checks the state variables of the plock data structure to
 * determine if the calling thread can proceed or instead must be added to
 * a waiting list. If the thread must be added, a node instance is created
 * and the condition variable within the node must be initialized using the
 * appropriate pthread library call. (The creation and addition could be
 * structured as a private helper function if you wish. Helper functions
 * should be contained in the plock.c source file.) The thread can then
 * wait on the condition variable in the node.
 *
 */
void plock_enter(plock_t* lock, int priority)
{
	// Put them into queue, then determine what thread should be run
	// run or put into waiting list
	
	// when the queue is empty, direct access will cause segfault
	pthread_mutex_lock(&(lock->mlock));
	
	node_t* new_node = create_node(priority);	
	
	EnQueue(lock, new_node);
	
	while (lock->value != FREE || priority < lock->head->priority) {
	// if the condition is not correct, we wait 
		pthread_cond_wait(&(new_node->waitCV), &(lock->mlock));
	}			// judge if and which the thread is eligible to run
	lock->value = BUSY;		
	//delete the node and destroy the node and proceed is in the exit
	node_t* DeQ_node = DeQueue(lock);
	free(DeQ_node);
	pthread_mutex_unlock(&(lock->mlock));	

}
/*
 * This function checks the state variables of the plock data structure to
 * determine what update action to take. For example, it may need to signal
 * a waiting thread using the appropriate pthread library call.
 */
 void plock_exit(plock_t* lock)
{
			 
	pthread_mutex_lock(&(lock->mlock));	
	    		
	lock->value = FREE;  
  
  	if (lock->head != NULL)	pthread_cond_signal(&(lock->head->waitCV));
	
	pthread_mutex_unlock(&(lock->mlock));
	
}
