__kernel void addition_kernel(__global int* restrict matrix1, __global int* restrict matrix2,__global int* restrict addition)

{
			int i = get_global_id(0);
			*(addition + i) = *(matrix1 + i) + *(matrix2 + i);
}

__kernel void multiplication_kernel(__global int* restrict multiplication, __global int* restrict addition_result, __global int* restrict vector, __global int* restrict array_length)
{
			int i = get_global_id(0);
			int j;
			for (j = 0; j < (*array_length); j ++)
				*(multiplication + i) += *(addition_result + i * (*array_length) + j) * (*(vector + j));
}
