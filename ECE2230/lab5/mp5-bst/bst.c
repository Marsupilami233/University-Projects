/* bst.c 
 * Yulin
 * Yulinx
 * ECE 2230 Spring 2019
 * MP 5
 *
 * Implementation of tree interface for basic binary tree
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>

#include "bst.h"

#define MYMAX(a, b) (a > b ? a : b)
// counters for statistics
int CompCalls = 0;
int NumRotations = 0;

/* definitions for use in bst.c only */
void ugly_print(bst_node_t *N, int level, int policy);
int bst_debug_validate_rec(bst_node_t *N, int min, int max, int *count);
int rec_height(bst_node_t *N);
int children(bst_node_t *N);
void pretty_print(bst_t *T);



/* ++++++++++++++++++++++++++++My function to create a new node+++++++++++++++++++++++ */
/*
 * This function takes key and data to malloc a new node
 */
bst_node_t* create_new_node(bst_key_t key, data_t* elem_ptr) 
{   // Initialize
    bst_node_t* new = (bst_node_t*) calloc(1, sizeof(bst_node_t));
    new->key = key;
    new->left = NULL;
    new->right = NULL;
    new->data_ptr = elem_ptr;
    new->height = 0;
    return new;
}
/* ++++++++++++++++++++++++++++My function to create a new node+++++++++++++++++++++++ */
/* --------------------------My recursive function for bst_access--------------------- */
/*
 * This function searches downward, reach a mathc node then return to main call function
 */   
bst_node_t* bst_recursive_search(bst_node_t* node, bst_key_t key) 
{
    CompCalls = 0;
    if (node == NULL) {// do not find, come to the empty node
        return NULL;
    }
    else {
        CompCalls++;// increment the CompCalls every time
        if (key < node->key) {
            return bst_recursive_search(node->left, key);
        }
        else if (key > node->key) {
            return bst_recursive_search(node->right, key);
        }        
        else {
            return node;
        }
    }
}
/* --------------------------My recursive function for bst_access--------------------- */ 
/* Finds the tree element with the matching key and returns the data that is
 * stored in this node in the tree.
 *
 * T - tree to search in key - key to find inside T
 *
 * RETURNS pointer to the data stored in the found node or NULL if no match is
 * found
 */
data_t bst_access(bst_t *T, bst_key_t key)
{
    /*TODO complete*/
    // start from the root
    bst_node_t* rec_ptr = bst_recursive_search(T->root, key);
    if (rec_ptr == NULL) return NULL; 
    return rec_ptr->data_ptr;
}

/* Creates the header block for the tree with the provided management policy,
 * and initializes it with default 'blank' data.
 *
 * tree_policy - tree management policy to use either AVL or BST.
 *
 * RETURNS pointer to the newly created tree
 */
bst_t *bst_construct(int tree_policy)
{
    /* TODO complete */
    // allocate a new header block
    bst_t *T = (bst_t*) calloc(1, sizeof(bst_t));
    T->policy = tree_policy;
    T->root = NULL;
    T->size = 0;// current size
    T->num_recent_key_comparisons = 0;
    // Do not count checks for the NULL pointers!
    T->num_recent_rotations = 0;
    return T;
}




/*
 * Delete the Nodes using postOrder and do not rebuild the node's sequence 
 * First recusively delete the node from its child, after free the data, free node 
 */
void delete_tree(bst_node_t* node) {
    // Base Case: the node has no children
    if (node == NULL) return;
    // delete the right substrees and the left subtrees
    delete_tree(node->left);
    delete_tree(node->right);
    free(node->data_ptr);
    free(node);
}
/*
 * Delete the Nodes using postOrder and do not rebuild the node's sequence 
 *
 */

/* Free all items stored in the tree including the memory block with the data
 * and the bst_node_t structure.  Also frees the header block.  
 *
 * T - tree to destroy
 */

void bst_destruct(bst_t *T)
{
    /* TODO complete */
    if (T == NULL) return;
    delete_tree(T->root);
    free(T);
}

/* Insert data_t into the tree with the associated key. Insertion MUST
 * follow the tree's property (i.e., AVL or BST)
 *
 * T - tree to insert into
 * key - search key to determine if key is in the tree
 * elem_ptr - data to be stored at tree node indicated by key
 *
 * RETURNS 0 if key is found and element is replaced, and 1 if key is not found
 * and element is inserted
 */

int bst_insert(bst_t* T, bst_key_t key, data_t elem_ptr)
{
    CompCalls = 0;
    if (T->policy == BST) {//------ BST ---------
        assert(T->policy == BST);
        bst_node_t* Rover = T->root;
        bst_node_t* Rover_P = T->root;
        // 1.root == NULL, create new node
        // 2.Search for the node
        // 1) successful->replace
        // 2) unsuccessful->do not replace
        if (T->root == NULL) {
            T->root = create_new_node(key, elem_ptr);
            T->size ++;        
            return 1;
        } else {
            while (Rover != NULL) {
                CompCalls ++;
                if (key < Rover->key) {
                    Rover_P = Rover;
                    Rover = Rover->left;
                
                } else if (key > Rover->key) {
                    Rover_P = Rover;
                    Rover = Rover->right;
                
                } else {// the key is the exact match
                    break;
                }
            }
            if (Rover == NULL) {// unsuccessful search to create a new node!
                if (Rover_P->left == NULL && key < Rover_P->key) {// if the new node have to go left 
                    Rover_P->left = create_new_node(key, elem_ptr); 
                    T->size ++;
                } else {
                    Rover_P->right = create_new_node(key, elem_ptr);
                    T->size ++;
                } 
                T->num_recent_key_comparisons = CompCalls;            
                return 1;   
            } else {
                free(Rover->data_ptr);
                Rover->data_ptr = elem_ptr;
                T->num_recent_key_comparisons = CompCalls; 
                return 0;
            }
        
        }
    
    }//-------------------BST---------------
    else{
        assert(T->policy == AVL);
        int rec = bst_avl_insert(T, key, elem_ptr);
        return rec;
    }    
    // successful search to replace the data_ptr
    // unsuccessful search to create a new node!
#ifdef VALIDATE
        bst_debug_validate(T);
#endif
}
/*------------Update the height----------*/
int update_height(bst_node_t* node) {

    if (node == NULL)
	return 0;
    if (node->left == NULL && node->right == NULL) 
        return node->height = 0;
    int lh = update_height(node->left);
    int rh = update_height(node->right);
    return node->height = 1 + MYMAX(lh, rh); 

}
/*------------Update the height----------*/

/*-------check height lean-------*/
int check_lean(bst_node_t* node) {
    if (node == NULL) {
        return 0;
    } else {
        if (node->left != NULL && node->right != NULL) {
            if (node->left->height < node->right->height) {// if not empty, return node
                return (node->right->height - node->left->height);//like ?A > B :(A,B)
            } else {
                return (node->left->height - node->right->height);
            }            
        } else if (node->left == NULL && node->right != NULL){
            return (node->height - 0);
        } else if (node->right == NULL && node->left != NULL){
            return (node->height - 0);
        } else {// do not have left child or right child
            return 0;
        }
    }
}
/*------- check height lean-------*/

/*+++++++++++check height and return the node that violate the AVL property+++++++++++++*/

bst_node_t* check_height(bst_node_t* node) {
    int lean = 0;
    bst_node_t* L;
    bst_node_t* R;
    bst_node_t* Base;
    if (node == NULL) {
        return NULL;
    } else {
        lean = check_lean(node);
        L = check_height(node->left);
        R = check_height(node->right);
        if (L != NULL && R == NULL) {
            Base = L;
        } else if (L == NULL && R != NULL) {
            Base = R;
        } else {
            Base = NULL;// if L->left == NULL && R->right == NULL or all not NULL, means we could check the leanage  
        }
        if (lean == 2 && Base == NULL) {
            return node;
        }
    }
    return Base;
}
/*+++++++++++check height and return the node that violate the AVL property+++++++++++++*/


/*-------Rotation-------*/ /*LL, LR, RL, RR*/
void rotation(bst_t* T, bst_node_t* A, bst_node_t* inserted) {
    if (A->left != NULL || A->right != NULL) {
        // check height
        // LL, LR, RL, RR
        // go through the inserted node to find its path!
        bst_node_t* PA = T->root;// A node's parent
        bst_node_t* B = NULL;
        bst_node_t* C = NULL;
        bst_node_t* BS = NULL;// B node's subtree
        bst_node_t* CS = NULL;// C node's subtree
        // find the A node's parent, or it will lost the whole subtree involved to rotate!
        while (PA->left != A && PA->right != A) {
            if (A->key < PA->key) {
                PA = PA->left;
            } else if (A->key > PA->key){
                PA = PA->right;
            } else {
                break;
            }
        }

        if (inserted->key < A->key) {
            B = A->left;//L
            if (inserted->key < B->key) {
                C = B->left;//LL
                // do one rotation (A-B-C) to the right (B-A-C) in level order
                BS = B->right;// find the B's right subtree
                B->right = A; // let B to be the new A subtree's root
                A->left = BS; // link the B's right subtree to A's left subtree, if NULL, set it to null btw
                
                if (PA == A) {
                    T->root = B;// if A is the root of the AVL tree, set B to the root
                } else if (PA->left == A){// if A is not root of the AVL tree, set B to A's parent
                    PA->left = B;
                } else {// if A is root of PA's right subtree
                    PA->right = B;
                }
                NumRotations++;
            } else {
                C = B->right;//LR
                // do two rotations (A-B-C) first to the left (A-C-B) then to the right (C-A-B) in level order
                CS = C->left;// find the C's left subtree
                C->left = B;// let C be the left subtree of B  
                B->right = CS;// link the C's left subtree to B's right subtree, if NULL, set it to null btw
                
                A->left = C; // Now, A is parent of B, C shall be new root of rotation
                NumRotations++;
                
                CS = C->right;// find the C's right subtree
                C->right = A; // let A to be the new C subtree's root  
                A->left = CS; // link the C's right subtree to A's left subtree, if NULL, set it to null btw
                
                if (PA == A) {
                    T->root = C;// if A is the root of the AVL tree, set B to the root
                } else if (PA->left == A){// if A is not root of the AVL tree, set B to A's parent
                    PA->left = C;
                } else {// if A is root of PA's right subtree
                    PA->right = C;
                }
                NumRotations++;
            }
        } else {
            B = A->right;//R
            if (inserted->key < B->key) {
                C = B->left;//RL
                // do two rotations (A-B-C) first to the right (A-C-B) then to the left (C-A-B) in level order
                CS = C->right;// find the C's right subtree
                C->right = B;// let C be the right subtree of B  
                B->left = CS;// link the C's right subtree to B's left subtree, if NULL, set it to null btw
                
                A->right = C; // Now, A is parent of B, C shall be new root of rotation
                NumRotations++;
                
                CS = C->left;// find the C's left subtree
                C->left = A; // let A to be the new C subtree's root  
                A->right = CS; // link the C's left subtree to A's right subtree, if NULL, set it to null btw
                
                if (PA == A) {
                    T->root = C;// if A is the root of the AVL tree, set B to the root
                } else if (PA->left == A){// if A is not root of the AVL tree, set B to A's parent
                    PA->left = C;
                } else {// if A is root of PA's right subtree
                    PA->right = C;
                }                
                NumRotations++;
            } else {
                C = B->right;//RR
                // do one rotation (A-B-C) to the left (B-A-C) in level order
                BS = B->left;// find the B's left subtree
                B->left = A; // let B to be the new A subtree's root
                A->right = BS; // link the B's left subtree to A's right subtree, if NULL, set it to null btw
                
                if (PA == A) {
                    T->root = B;// if A is the root of the AVL tree, set B to the root
                } else if (PA->left == A){// if A is not root of the AVL tree, set B to A's parent
                    PA->left = B;
                } else {// if A is root of PA's right subtree
                    PA->right = B;
                }
                NumRotations++;                
            }
        }
     
    } else {
        return;// error!
    }
}
/*-------Rotation-------*/ /*LL, LR, RL, RR*/

/* Insert data_t into the tree with the associated key. Insertion MUST
 * follow the tree's property AVL. This function should be called from
 * bst_insert for AVL tree's inserts.
 *
 * T - tree to insert into
 * key - search key to determine if key is in the tree
 * elem_ptr - data to be stored at tree node indicated by key
 *
 * RETURNS 0 if key is found and element is replaced, and 1 if key is not found
 * and element is inserted
 */
int bst_avl_insert(bst_t *T, bst_key_t key, data_t elem_ptr)
{
    //int replace = 0;
    
    /* TODO: insert into AVL tree. Rember to check balance and rebalance if
     * necessary */
    assert(T->policy == AVL);
    
/*-----------------------------Basic Insertion Operation----------------------*/    
    NumRotations = 0;
    CompCalls = 0;
    bst_node_t* Rover = T->root;
    bst_node_t* Rover_P = T->root;
    bst_node_t* new = NULL;
    bst_node_t* rec_ptr = NULL;
    // 1.root == NULL, create new node
    // 2.Search for the node
    // 1) successful->replace
    // 2) unsuccessful->insert
    if (T->root == NULL) {
        T->root = create_new_node(key, elem_ptr);
        T->size ++;
        T->root->height = 0;        
        return 1;
    } else {
        while (Rover != NULL) {
            CompCalls ++;
            if (key < Rover->key) {
                Rover_P = Rover;
                Rover = Rover->left;
                
            } else if (key > Rover->key) {
                Rover_P = Rover;
                Rover = Rover->right;
                
            } else {// the key is the exact match
                break;
            }
        }
        if (Rover == NULL) {// unsuccessful search to create a new node!
            if (Rover_P->left == NULL && key < Rover_P->key) {// if the new node have to go left
                new = create_new_node(key, elem_ptr); 
                Rover_P->left = new; 
                T->size ++;
            } else {
                new = create_new_node(key, elem_ptr);
                Rover_P->right = new;
                T->size ++;
            }
            //printf("<----------------------Before Rotate:-------------------->\n");
            
            update_height(T->root);//<----------------------------------------------------every time after insertion, change height
            rec_ptr = check_height(T->root); //<------------------------------------------AVL property, if NULL, do not change
            
            //ugly_print(T->root, 0, T->policy);
            //pretty_print(T);            
            if (rec_ptr != NULL) {
                rotation(T, rec_ptr, new);
                //printf("<+++++++++++++++++++++++After Rotate+++++++++++++++++++++++>\n");
                
                update_height(T->root);//<------------------------------------------------every time after rotation, change height
                //ugly_print(T->root, 0, T->policy);
                //pretty_print(T);
            }
            //printf("N: %d, C: %d, inserted: %d\n", NumRotations, CompCalls, new->key);
            T->num_recent_rotations = NumRotations;
            T->num_recent_key_comparisons = CompCalls;            
            return 1;   
        } else {
            free(Rover->data_ptr);
            Rover->data_ptr = elem_ptr;
            //printf("N: %d, C: %d, inserted: %d\n", NumRotations, CompCalls, new->key);
            T->num_recent_rotations = NumRotations;
            T->num_recent_key_comparisons = CompCalls;
            return 0;
        }
    }

/*-----------------------------Basic Insertion Operation------------------------- */        
    // successful search to replace the data_ptr
    // unsuccessful search to create a new node!
#ifdef VALIDATE
        bst_debug_validate(T);
#endif     
    
    //return replace;
}

/* DO NOT NEED TO IMPLEMENT FOR REGULAR ASSIGNMENT. ONLY FOR EXTRA ASSIGNMENT.
 *
 * Removes the item in the tree with the matching key.
 * 
 * T - pointer to tree
 * key - search key for particular node in the tree 'T'
 *
 * RETURNS the pointer to the data memory block and free the bst_node_t memory
 * block.  If the key is not found in the tree, return NULL.  If the tree's 
 * policy is AVL, then ensure all nodes have the AVL property.
 *
 */

data_t bst_remove(bst_t *T, bst_key_t key)
{
    data_t dp = NULL;
    CompCalls = 0;
    NumRotations = 0;
    bst_node_t* Rover = T->root;
    bst_node_t* Rover_P = T->root;
    bst_node_t* Rover_L = T->root;
    bst_node_t* del = NULL;
    bst_node_t* est = NULL;
    bst_node_t* estp = NULL;
   
    /*node_search with a previous node*/
    while (Rover != NULL) {
        CompCalls ++;
        if (key < Rover->key) {
            Rover_P = Rover;
            Rover = Rover->left;
        } else if (key > Rover->key) {
            Rover_P = Rover;
            Rover = Rover->right;
        } else {
            break;
        }
    }
    
    if (Rover == NULL) return NULL;// not found, stop 
    
    
	    /*TODO: AVL remove */
    	    
	    
    
	    /*TODO: BST remove */
	dp = Rover->data_ptr;
	//Rover->data_ptr = NULL;
	del = Rover;// find the node to delete
	if (Rover == T->root) Rover_P = NULL;//<------------------------------------------Notice: if I do not do that, it will accidentally access the freeed memory!

	/*----------------Find a replaceable node---------------------------*/
	
	if (Rover->left != NULL && Rover->right != NULL) {
	    //first move the larger one!

	    Rover_L = Rover->left;// go to the left subtree
	    Rover = Rover->right;// go to right subtree
	    free(del);
	    del = NULL;
	    // iteratively find the smallest node in the right subtree
	    est = Rover;// start from root
	    estp = Rover;
	    while (est->left != NULL) {
	        estp = est;
	        est = est->left;
	    }
	    // put the est node to the place that the node is deleted
            
            if (Rover_P == NULL) {// the Rover_P is at root, the root is freed
                if (est == Rover) {// if the est is the only node in the subtree
                
                    T->root = est;// new root
                    
                    est->left = Rover_L;// replace the node and link the left subtree
                } else {
                    estp->left = NULL;// unlink
                    
                    T->root = est;
                    
                    est->left = Rover_L;//replace the node and link to the right subtree
                    if (est->right != NULL) estp->left = est->right;//link the right subtree of smallest node founded
                    est->right = Rover;// link the left subtree
                }                
            } else {
                if (est == Rover) {
                
                    if (Rover->key < Rover_P->key) {
                        Rover_P->left = Rover;
                    } else {
                        Rover_P->right = Rover;
                    }
                    
                    Rover->left = Rover_L;
                } else {
                    estp->left = NULL;// unlink
                    
                    if (est->key < Rover_P->key) {
                        Rover_P->left = est;
                    } else {
                        Rover_P->right = est;
                    }                    //replace the node
                    
                    est->left = Rover_L;//link the left subtree
                    if (est->right != NULL) estp->left = est->right;//link the right subtree of smallest node founded
                    est->right = Rover;// link the right subtree
                }
            }
            	    
        } else if (Rover->left == NULL && Rover->right != NULL) {// only right subtree
	    // direct link the right subtree

	    Rover = Rover->right;// go to right subtree
	    free(del);
	    del = NULL;
	    if (Rover_P == NULL) {
	        T->root = Rover;     
	    } else {
	        if (Rover->key < Rover_P->key) {
	            Rover_P->left = Rover;
	        } else {
	            Rover_P->right = Rover;
	        }
	    }
	} else if (Rover->left != NULL && Rover->right == NULL) {// only left subtree
            // direct link the left subtree

            Rover = Rover->left;// find the left subtree's root
            free(del);//delete
            del = NULL;
            if (Rover_P == NULL) {
                T->root = Rover;
            } else {
                if (Rover->key < Rover_P->key) {
                    Rover_P->left = Rover;
                } else {
                    Rover_P->right = Rover;
                }
            } 
	} else {// the node is deleted is a leaf
	    dp = Rover->data_ptr;//<------------------------------------------------------BP 1
	    //Rover->data_ptr = NULL;
	    free(Rover);
	    if (Rover_P == NULL) {
	        T->root = NULL;
	    } else {
	        if (Rover_P->left == Rover) {
	            Rover_P->left = NULL;    
	        } else if (Rover_P->right == Rover) {
	            Rover_P->right = NULL;
	        } else {
	            T->root = NULL;
	        }
	    }
	    Rover = NULL;
	   
	}    	
	T->size--;    
        T->num_recent_key_comparisons = CompCalls;     
    /*if (T->policy == AVL) {
        update_height(T->root);
            //printf("<----------------------Before Rotate:-------------------->\n");
            
            update_height(T->root);//<----------------------------------------------------every time after insertion, change height
            rec_ptr = check_height(T->root); //<------------------------------------------AVL property, if NULL, do not change
            
            //ugly_print(T->root, 0, T->policy);
            //pretty_print(T);            
            if (rec_ptr != NULL) {
                rotation(T, rec_ptr, new);
                //printf("<+++++++++++++++++++++++After Rotate+++++++++++++++++++++++>\n");
                
                update_height(T->root);//<------------------------------------------------every time after rotation, change height
                //ugly_print(T->root, 0, T->policy);
                //pretty_print(T);
            }
    }*/
    /*TODO: update tree stats*/
    T->num_recent_rotations = NumRotations;
#ifdef VALIDATE
        bst_debug_validate(T);
#endif
    return dp;
}


/* RETURNS the number of keys in the tree */
int bst_size(bst_t *T)
{
    /*TODO */
    return T->size;    
}

/* RETURNS the number of key comparisons  */
int bst_key_comps(bst_t *T)
{
    return T->num_recent_key_comparisons; /*TODO */
}
/******* recursively use the internal path length ********/
int recursive_len(bst_node_t* node, int sum) {
    if (node == NULL)
        return 0;
    if (node->left == NULL && node->right == NULL) {
        return sum;
    }
    return (sum + recursive_len(node->left, sum + 1) + recursive_len(node->right,sum + 1));
    
}
/********recursively use the internal path length ********/
/* RETURNS the computed internal path length of the tree T */
int bst_int_path_len(bst_t *T)
{
    // internal path length 
    int len_total = recursive_len(T->root, 0);
    return len_total;
    /*TODO */
}

/* RETURNS the number of rotations from the last remove*/
int bst_rotations(bst_t *T)
{
    return T->num_recent_rotations; /*TODO */
}


/* prints the tree T */
void bst_debug_print_tree(bst_t *T)
{
    ugly_print(T->root, 0, T->policy);//XTRA
    printf("\n");
    if (T->size < 64)
	pretty_print(T);
}

/* basic print function for a binary tree */
void ugly_print(bst_node_t *N, int level, int policy)
{
    int i;
    if (N == NULL) return;
    ugly_print(N->right, level+1, policy);
    if (policy == AVL) {
	    for (i = 0; i<level; i++) printf("       ");
	        printf("%5d-%d\n", N->key, N->height);
    } else {
	    for (i = 0; i<level; i++) printf("     ");
	        printf("%5d\n", N->key);
    }
    ugly_print(N->left, level+1, policy);
}


/* Basic validation function for tree T */
void bst_debug_validate(bst_t *T)
{
    int size = 0;
    assert(bst_debug_validate_rec(T->root, INT_MIN, INT_MAX, &size) == TRUE); //<----------------------BP2
    assert(size == T->size);
    if (T->policy == AVL)
	    rec_height(T->root);
}


/* A tree validation function
 */
int bst_debug_validate_rec(bst_node_t *N, int min, int max, int *count)
{
    if (N == NULL)
        return TRUE;
    if (N->key <= min || N->key >= max)
        return FALSE;
    assert(N->data_ptr != NULL);
    *count += 1;
    return bst_debug_validate_rec(N->left, min, N->key, count) &&
        bst_debug_validate_rec(N->right, N->key, max, count);
}

/* Verifies AVL tree properties */

int rec_height(bst_node_t *N)
{
    if (N == NULL)
	    return 0;
    int lh = rec_height(N->left);
    int rh = rec_height(N->right);
    int lean = lh - rh;
    assert(lean == 0 || lean == 1 || lean == -1);
    return 1 + MYMAX(lh, rh); 

}





/* Recursive function to count children */
int children(bst_node_t *N)
{
    if (N == NULL) return 0;
    return 1 + children(N->left) + children(N->right);
}



/* Prints the tree to the terminal in ASCII art*/
void pretty_print(bst_t *T)
{
    typedef struct queue_tag {
	    bst_node_t *N;
	    int level;
	    int list_sum;
    } queue_t;

    queue_t Q[T->size];
    int q_head = 0;
    int q_tail = 0;
    int i, j;
    int current_level = 0;
    int col_cnt = 0;
    bst_node_t *N;

    Q[q_tail].N = T->root;
    Q[q_tail].level = 0;
    Q[q_tail].list_sum = 0;
    q_tail++;
    for (i = 0; i < T->size; i++)
    {
	assert(q_head < T->size);
	N = Q[q_head].N;
	if (Q[q_head].level > current_level) {
	    printf("\n");
	    current_level++;
	    col_cnt = 0;
	}
	int left_ch = children(N->left);
	int my_pos = 1 + left_ch + Q[q_head].list_sum;
	int left_child_pos = my_pos;
	if (N->left != NULL)
	    left_child_pos = 1 + Q[q_head].list_sum + children(N->left->left);
	int right_child_pos = my_pos;
	if (N->right != NULL)
	    right_child_pos = my_pos + 1 + children(N->right->left);
	for (j = col_cnt + 1; j <= right_child_pos; j++)
	{
	    if (j == my_pos)
		if (left_child_pos < my_pos)
		    if (N->key < 10)
			printf("--%d", N->key);
		    else if (N->key < 100)
			printf("-%d", N->key);
		    else
			printf("%d", N->key);
		else
		    printf("%3d", N->key);
	    else if (j == left_child_pos)
		//printf("  |");
		printf("  /");
	    else if (j > left_child_pos && j < my_pos)
		printf("---");
	    else if (j > my_pos && j < right_child_pos)
		printf("---");
	    else if (j == right_child_pos)
		//printf("--|");
		printf("-\\ ");
	    else
		printf("   ");
	}
	col_cnt = right_child_pos;

	if (N->left != NULL) {
	    Q[q_tail].N = N->left;
	    Q[q_tail].level = Q[q_head].level + 1;
	    Q[q_tail].list_sum = Q[q_head].list_sum;
	    q_tail++;
	}
	if (N->right != NULL) {
	    Q[q_tail].N = N->right;
	    Q[q_tail].level = Q[q_head].level + 1;
	    Q[q_tail].list_sum = Q[q_head].list_sum + left_ch + 1;
	    q_tail++;
	}
	q_head++;
    }
    printf("\n");
}

/* vi:set ts=8 sts=4 sw=4 et: */
