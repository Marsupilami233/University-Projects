#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/*This file automatically generates the 4x4 matrics and then do the addtion
 *Note that every add should do the same thing
 */
int main(int argc, char* argv[])
{
	if (argc != 2) {
		printf("Usage: ./create <array_size>\n");
		exit(0);
	}
	int L = atoi(argv[1]);
	int i,j;
	int *Matrix1, *Matrix2, *Vector;
	Matrix1 = (int *) calloc(L * L, sizeof(int));
	Matrix2 = (int *) calloc(L * L, sizeof(int));
	Vector = (int *) calloc(L, sizeof(int));
	FILE *fpt;
	srand((unsigned)time(NULL));//initialize rand()
	/*srand is used in the top initialize the rand() with different value*/
	/*--------------------------Generate M1-----------------------*/
	for (i = 0; i < L; i ++)
	{
		for (j = 0; j < L; j ++)
		{
			*(Matrix1 + (i * L) + j) = rand()%10;
		}
	}
	/*printf("The matrix M1 that generated is: \n");
	for (i = 0; i < L; i ++)
	{
		for (j = 0; j < L; j ++)
		{
			printf("%d ", Matrix[i][j]);
		}
		printf("\n");
	}*/
	printf("Size of %dX%d Matrix1 generated!\n", L, L);
	if ((fpt = fopen("M1.bin","wb")) == NULL)
	{
		printf("Cannot open the M1.bin for write!\n");
		exit(0);
	}
	int array_size = L;
	fwrite(&array_size, 1, sizeof(int), fpt);// read in the binary values
	fwrite(Matrix1, L*L, sizeof(int), fpt);// read in the binary values
	fclose(fpt);
	free(Matrix1);
	/*---------------------------Generate M1-----------------------*/
	/*---------------------------Generate M2-----------------------*/
	for (i = 0; i < L; i ++)
	{
		for (j = 0; j < L; j ++)
		{
			*(Matrix2 + (i * L) + j) = rand()%10;
		}
	}
	/*printf("The matrix M2 that generated is: \n");
	for (i = 0; i < L; i ++)
	{
		for (j = 0; j < L; j ++)
		{
			printf("%d ", Matrix[i][j]);
		}
		printf("\n");
	}*/
	printf("Size of %dX%d Matrix2 generated!\n", L, L);
	if ((fpt = fopen("M2.bin","wb")) == NULL)
	{
		printf("Cannot open the M2.bin for write!\n");
		exit(0);
	}
	array_size = L;
	fwrite(&array_size, 1, sizeof(int), fpt);//read in the binary values
	fwrite(Matrix2, L*L, sizeof(int), fpt);// read in the binary values
	fclose(fpt);
	free(Matrix2);	
	/*---------------------------Generate M2-----------------------*/
	/*---------------------------Generate Vector-----------------------*/
	for (i = 0; i < L; i ++)
	{
		*(Vector + i)  = rand()%10;
	}
	/*printf("The vector V that generated is: \n");
	for (i = 0; i < L; i ++)
	{
		printf("%d ", Vector[i]);
	}
	printf("\n");*/
	printf("Size of %d Vector generated!\n", L);
	if ((fpt = fopen("V.bin","wb")) == NULL)
	{
		printf("Cannot open the V.bin for write!\n");
		exit(0);
	}
	fwrite(&array_size, 1, sizeof(int), fpt);// read in the binary values
	fwrite(Vector, L, sizeof(int), fpt);// read in the binary values
	fclose(fpt);
	free(Vector);	
	/*---------------------------Generate Vector-----------------------*/
	return 0;
}
