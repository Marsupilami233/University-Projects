#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

#include "bst.h"


int main() {
	bst_t* T = NULL;
	T = bst_construct(BST);
	int* data_ptr = NULL;
	T->root = bst_insert(T, 1, data_ptr);
	bst_insert(T, 2, data_ptr);
	bst_insert(T, 3, data_ptr);
	bst_insert(T, 4, data_ptr);
	bst_insert(T, 5, data_ptr);
	bst_debug_print_tree(T);
	bst_destruct(T);
	return 0;
}
