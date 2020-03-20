#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void swap(int *a,int *b) {
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

void bubblesort(int *A, int sort_type) {
	int i,j;
	for (i = 0; i < 13;i ++) {
		for (j = i + 1; j < 13; j ++) {
			if (A[j] < A[i]) {
				swap(&A[i],&A[j]);
			}
		}
		
	}
}
int main(){
	int sort_type = 1;
	int A[] = {0,1,2,3,4,5,5,6,7,5,8,6,7};
	bubblesort(A, sort_type);
	int i = 0;
	for (i = 0; i < 13; i ++) {
		printf("%d ",A[i]);
	}
	printf("\n");
	
	return 0;
}
