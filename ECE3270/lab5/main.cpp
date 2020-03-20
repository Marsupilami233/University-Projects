#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "CL/opencl.h"
#include "AOCL_Utils.h"

using namespace aocl_utils;

// OpenCL runtime configuration
cl_platform_id platform = NULL;
unsigned num_devices = 0;
cl_device_id *device;
cl_context context = NULL;
cl_command_queue queue;
cl_program program = NULL;
cl_kernel addition_kernel;
cl_kernel multiplication_kernel;
cl_mem output_buf_ADD;
cl_mem output_buf_MUL;
cl_mem input_buf_M1;
cl_mem input_buf_M2;
cl_mem input_buf_V;
cl_mem input_buf_size;

// Problem data.
// problem size
//array
int *matrix1;
int *matrix2;
int *vector;
int *ADDOUT;
int *MULTOUT;
int L;



// Function prototypes
bool init_opencl();
void init_problem(char*);
void run();
void run_serial();
void cleanup();

char *input;

// Entry point.
int main(int argc, char **argv) {

    if (argc != 2) {

      printf("Usage: /.operation <array_size>\n");
      exit(0);
    }
  	// Initialize the problem data.
  	init_problem(argv[1]);

  	// Initialize OpenCL.
  	if(!init_opencl()) {
    		return -1;
  	}

  	// Run the kernel.
  	run();
    // Run the serial
    run_serial();

  	// Free the resources allocated
  	cleanup();

  	return 0;
}

/////// HELPER FUNCTIONS ///////

// Initializes the OpenCL objects.
bool init_opencl() {
  	cl_int status;

  	printf("Initializing OpenCL\n");

  	if(!setCwdToExeDir()) {
    		return false;
  	}

  	// Get the OpenCL platform.
  	platform = findPlatform("Altera");
  	if(platform == NULL) {
    		printf("ERROR: Unable to find Altera OpenCL platform.\n");
    		return false;
  	}

  	// Query the available OpenCL device.
  	device = getDevices(platform, CL_DEVICE_TYPE_ALL, &num_devices);
  	printf("Platform: %s\n", getPlatformName(platform).c_str());
  	printf("Using %d device(s)\n", num_devices);
  	printf("  %s\n", getDeviceName(*device).c_str());

  	// Create the context.
  	context = clCreateContext(NULL, num_devices, device, NULL, NULL, &status);
  	checkError(status, "Failed to create context");

  	// Create the program for all device. Use the first device as the
  	// representative device (assuming all device are of the same type).
  	std::string binary_file = getBoardBinaryFile("operation", *device);
  	printf("Using AOCX: %s\n", binary_file.c_str());
  	program = createProgramFromBinary(context, binary_file.c_str(), device, num_devices);

  	// Build the program that was just created.
  	status = clBuildProgram(program, 1, device, "", NULL, NULL);
  	checkError(status, "Failed to build program");

  	// Command queue.
  	queue = clCreateCommandQueue(context, *device, CL_QUEUE_PROFILING_ENABLE, &status);
  	checkError(status, "Failed to create command queue");

  	// Kernel 1.
  	const char *kernel_name = "addition_kernel";
  	addition_kernel = clCreateKernel(program, kernel_name, &status);
  	checkError(status, "Failed to create kernel");
  	// Kernel 2.
  	const char *kernel_name2 = "multiplication_kernel";
  	multiplication_kernel = clCreateKernel(program, kernel_name2, &status);
  	checkError(status, "Failed to create kernel");

  	return true;
}

//Initialize data for the problem.
void init_problem(char* array) {
  L = atoi(array);
	matrix1 = (int *)calloc(L*L, sizeof(int));
	matrix2 = (int *)calloc(L*L, sizeof(int));
	vector = (int *)calloc(L, sizeof (int));
  ADDOUT = (int *)calloc(L*L, sizeof(int));
  MULTOUT= (int *)calloc(L, sizeof(int));
  int i,j,temp;
	FILE *fpt;
       if ((fpt = fopen("M1.bin", "rb")) == NULL)
       {
                printf("Cannot open the M1.bin to read!!!\n");
                exit(0);
       }
       fread(&temp, sizeof(int), 1, fpt);
       fread(matrix1, sizeof(int), L*L, fpt);
       fclose(fpt);

       if ((fpt = fopen("M2.bin", "rb")) == NULL)
       {
                printf("Cannot open the M2.bin to read!!!\n");
                exit(0);
       }
       fread(&temp, sizeof(int), 1, fpt);
       fread(matrix2, sizeof(int), L*L, fpt);
       fclose(fpt);

       if ((fpt = fopen("V.bin", "rb")) == NULL)
       {
               printf("Cannot open the V.bin to read!!!\n");
               exit(0);
       }
       fread(&temp, sizeof(int), 1, fpt);
       fread(vector, sizeof(int), L, fpt);
       fclose(fpt);

       //print out the result so I could see the difference
       /*Print the matrix1*/
       for (i = 0; i < L; i ++)
     	 {
     		       for (j = 0; j < L; j ++)
     		        {
     			          printf("%d ", *(matrix1 + (i * L) + j));
     		        }
     		       printf("\n");
     	 }
       /*Print the matrix2*/
       for (i = 0; i < L; i ++)
     	 {
     		       for (j = 0; j < L; j ++)
     		        {
     			          printf("%d ", *(matrix2 + (i * L) + j));
     		        }
     		       printf("\n");
     	 }
       /*Print the vector*/
       for (i = 0; i < L; i ++)
     	 {
     		       printf("%d ", *(vector + i));
     		       printf("\n");
     	 }
}

void run() {
	  int i,j, array_size = L;
  	cl_int status;
    FILE *fpt;
    //Input buffer for Matrix1.
  	input_buf_M1 = clCreateBuffer(context, CL_MEM_READ_ONLY,
  			sizeof(int) * (L * L), NULL, &status);
  	checkError(status, "Failed to create buffer for input M1");

  	//Input buffer for Matrix2.
  	input_buf_M2 = clCreateBuffer(context, CL_MEM_READ_ONLY,
  			sizeof(int) * (L * L), NULL, &status);
  	checkError(status, "Failed to create buffer for input M2");

  	//Input buffer for Size.
  	input_buf_size = clCreateBuffer(context, CL_MEM_READ_ONLY,
  			sizeof(int), NULL, &status);
  	checkError(status, "Failed to create buffer for input size");
    //Input buffer for Vector.
    input_buf_V = clCreateBuffer(context, CL_MEM_READ_ONLY,
        sizeof(int) * L, NULL, &status);

    // Output buffer.
    output_buf_ADD = clCreateBuffer(context, CL_MEM_READ_WRITE,
        		sizeof(int) * (L * L), NULL, &status);
    checkError(status, "Failed to create buffer for output ADD");

    // Output buffer.
    output_buf_MUL = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
        		sizeof(int) * L, NULL, &status);
    checkError(status, "Failed to create buffer for output MUL");



  	// Launch the problem for each device.
  	cl_event addition_kernel_event;
  	cl_event addition_finish_event;
	  cl_event addition_write_event;
    cl_event multiplication_kernel_event;
  	cl_event multiplication_finish_event;
	  cl_event multiplication_write_event;

    printf("My operation starts here: \n");
    printf("Addition:\n");

    const double start_time = getCurrentTimestamp();
	// Set kernel arguments.
  	unsigned argi = 0;
    status = clSetKernelArg(addition_kernel, argi++, sizeof(cl_mem), &input_buf_M1);
  	checkError(status, "Failed to set argument %d", argi - 1);
    status = clSetKernelArg(addition_kernel, argi++, sizeof(cl_mem), &input_buf_M2);
  	checkError(status, "Failed to set argument %d", argi - 1);
  	status = clSetKernelArg(addition_kernel, argi++, sizeof(cl_mem), &output_buf_ADD);
	  checkError(status, "Failed to set argument %d", argi - 1);

  	// Enqueue kernel.
	  // Use a global work size corresponding to the number of elements to add
  	// for this device.
	  //
  	// We don't specify a local work size and let the runtime choose
	  // (it'll choose to use one work-group with the same size as the global
 	  // work-size).
	  //
  	// Events are used to ensure that the kernel is not launched until
	  // the writes to the input buffers have completed.

	//Write this argument to a buffer.
	status = clEnqueueWriteBuffer(queue, input_buf_M1, CL_FALSE,
	0, L * L * sizeof(int), matrix1, 0, NULL, &addition_write_event);
	checkError(status, "Failed to write input buffer");

  status = clEnqueueWriteBuffer(queue, input_buf_M2, CL_FALSE,
  0, L * L * sizeof(int), matrix2, 0, NULL, &addition_write_event);
  checkError(status, "Failed to write input buffer");

	const size_t global_work_size = L * L;
	printf("Launching for device %d (%d elements)\n", 0, global_work_size);

	status = clEnqueueNDRangeKernel(queue, addition_kernel, 1, NULL,
	&global_work_size, NULL, 1, &addition_write_event, &addition_kernel_event);
	checkError(status, "Failed to launch kernel");

	clWaitForEvents(num_devices, &addition_kernel_event);

	// Read the result. This is the final operation.
	status = clEnqueueReadBuffer(queue, output_buf_ADD, CL_FALSE,
	0, L * L * sizeof(int), ADDOUT, 1, &addition_kernel_event, &addition_finish_event);


	// Wait for all devices to finish.
	clWaitForEvents(num_devices, &addition_finish_event);

	const double end_time = getCurrentTimestamp();


	// Wall-clock time taken.
 	printf("\nTime: %0.3f ms\n", (end_time - start_time) * 1e3);

	// Get kernel times using the OpenCL event profiling API.
  cl_ulong time_ns = getStartEndTime(addition_kernel_event);
	printf("Kernel time (device 0): %0.3f ms\n", double(time_ns) * 1e-6);

	// Release all events.
  clReleaseEvent(addition_write_event);
  clReleaseEvent(addition_kernel_event);
	clReleaseEvent(addition_finish_event);

  if ((fpt = fopen("ADDOUT.bin", "wb")) == NULL)
  {
      printf("Cannot open ADDOUT.bin for write binary!\n");
      exit(0);
  }
  array_size = L;
	fwrite(&array_size, 1, sizeof(int), fpt);//read in the binary values
	fwrite(ADDOUT, L*L, sizeof(int), fpt);// read in the binary values
	fclose(fpt);
  for (i = 0; i < L; i ++)
	{
		for (j = 0; j < L; j ++)
		{
			printf("%d ", *(ADDOUT + i*L + j) );
		}
		printf("\n");
	}

printf("multiplication:\n");


const double start_time1 = getCurrentTimestamp();

unsigned argj = 0;
status = clSetKernelArg(multiplication_kernel, argj++, sizeof(cl_mem), &output_buf_MUL);
checkError(status, "Failed to set argument %d", argj - 1);
status = clSetKernelArg(multiplication_kernel, argj++, sizeof(cl_mem), &output_buf_ADD);
checkError(status, "Failed to set argument %d", argj - 1);
status = clSetKernelArg(multiplication_kernel, argj++, sizeof(cl_mem), &input_buf_V);
checkError(status, "Failed to set argument %d", argj - 1);
status = clSetKernelArg(multiplication_kernel, argj++, sizeof(cl_mem), &input_buf_size);
checkError(status, "Failed to set argument %d", argj - 1);



//Write this argument to a buffer.
status = clEnqueueWriteBuffer(queue, output_buf_ADD, CL_FALSE,
0, L * L * sizeof(int), ADDOUT, 0, NULL, &multiplication_write_event);
checkError(status, "Failed to write input-output buffer");

status = clEnqueueWriteBuffer(queue, input_buf_V, CL_FALSE,
0, L * sizeof(int), vector, 0, NULL, &multiplication_write_event);
checkError(status, "Failed to write input buffer");

status = clEnqueueWriteBuffer(queue, input_buf_size, CL_FALSE,
0, sizeof(int), &array_size, 0, NULL, &multiplication_write_event);
checkError(status, "Failed to write input buffer");

const size_t global_work_size2 = L;
printf("Launching for device %d (%d elements)\n", 0, global_work_size2);

status = clEnqueueNDRangeKernel(queue, multiplication_kernel, 1, NULL,
&global_work_size2, NULL, 1, &multiplication_write_event, &multiplication_kernel_event);
checkError(status, "Failed to launch kernel");

clWaitForEvents(num_devices, &multiplication_kernel_event);

// Read the result. This is the final operation.
status = clEnqueueReadBuffer(queue, output_buf_MUL, CL_FALSE,
0, L * sizeof(int), MULTOUT, 1, &multiplication_kernel_event, &multiplication_finish_event);


// Wait for all devices to finish.
clWaitForEvents(num_devices, &multiplication_finish_event);

const double end_time1 = getCurrentTimestamp();


// Wall-clock time taken.
printf("\nTime: %0.3f ms\n", (end_time1 - start_time1) * 1e3);

// Get kernel times using the OpenCL event profiling API.
  cl_ulong time_ns1 = getStartEndTime(multiplication_kernel_event);
printf("Kernel time (device 0): %0.3f ms\n", double(time_ns1) * 1e-6);

// Release all events.
clReleaseEvent(multiplication_write_event);
clReleaseEvent(multiplication_kernel_event);
clReleaseEvent(multiplication_finish_event);

if ((fpt = fopen("MULTOUT.bin", "wb")) == NULL)
{
    printf("Cannot open MULTOUT.bin for write binary!\n");
    exit(0);
}
array_size = L;
fwrite(&array_size, 1, sizeof(int), fpt);//read in the binary values
fwrite(MULTOUT, L, sizeof(int), fpt);// read in the binary values
fclose(fpt);
for (i = 0; i < L; i ++)
{

    printf("%d ", *(MULTOUT + i) );
    printf("\n");
}

}
// the serial part of my c code
void run_serial()
{
  FILE *fpt;
  int *addition;
  int i,j;
  int array_size;
	addition = (int *)calloc (L*L, sizeof(int));
	/*------------------TIMING-------------------*/
  const double start_time = getCurrentTimestamp();
	/*------------------TIMING-------------------*/

	/*--------------This Part Needs to be Timed---------------*/

	for (i = 0; i < L; i ++)
	{
		for (j = 0; j < L; j ++)
		{
			*(addition + (i * L) + j) = *(matrix1 + (i * L) + j) + *(matrix2 + (i * L) + j);
		}
	}

	/*--------------This Part Needs to be Timed---------------*/

	/*------------------TIMING-------------------*/
	const double end_time = getCurrentTimestamp();
	/*------------------TIMING-------------------*/
  printf("\nTime for addition serial: %0.3f ms\n", (end_time - start_time) * 1e3);

	if ((fpt = fopen("ADD_ser.bin","wb")) == NULL)
	{
		printf("Cannot open the ADD_ser.bin for write!\n");
		exit(0);
	}
	array_size = L;
	fwrite(&array_size, 1, sizeof(int), fpt);//read in the binary values
	fwrite(addition, L*L, sizeof(int), fpt);// read in the binary values
	fclose(fpt);
	for (i = 0; i < L; i ++)
	{
		for (j = 0; j < L; j ++)
		{
			printf("%d ", *(addition + (i * L) + j));
		}
		printf("\n");
	}
	/*Do some multiplication*/
	int *multiplication;
	multiplication = (int *)calloc(L, sizeof(int));
	/*------------------TIMING-------------------*/
  const double start_time1 = getCurrentTimestamp();
	/*------------------TIMING-------------------*/
	/*--------------This Part Needs to be Timed---------------*/

	for (i = 0; i < L; i ++)
	{
		for (j = 0; j < L; j ++)
		{
			*(multiplication + i) += *(addition + (i * L) + j) * (*(vector + j));
		}
	}
	/*--------------This Part Needs to be Timed---------------*/

	/*------------------TIMING-------------------*/
	const double end_time1 = getCurrentTimestamp();
	/*------------------TIMING-------------------*/
  printf("\nTime for multiplication serial: %0.3f ms\n", (end_time1 - start_time1) * 1e3);

	if ((fpt = fopen("MULT_ser.bin","wb")) == NULL)
	{
		printf("Cannot open the MULT_ser.bin for write!\n");
		exit(0);
	}
	array_size = L;
	fwrite(&array_size, 1, sizeof(int), fpt);//read in the binary values
	fwrite(multiplication, L, sizeof(int), fpt); //read in the binary values
	fclose(fpt);
	for (i = 0; i < L; i ++)
	{
		printf("%d ", *(multiplication + i));
		printf("\n");
	}
  free(addition);
  free(multiplication);
}

// Free the resources allocated during initialization
void cleanup() {

  	if(addition_kernel) {
    		clReleaseKernel(addition_kernel);
  	}

    if(multiplication_kernel) {
    		clReleaseKernel(multiplication_kernel);
  	}

  	if(queue) {
      		clReleaseCommandQueue(queue);
  	}

  	if(output_buf_ADD) {
      		clReleaseMemObject(output_buf_ADD);
  	}

    if(output_buf_MUL) {
      		clReleaseMemObject(output_buf_MUL);
  	}

  	if(program) {
    		clReleaseProgram(program);
  	}
  	if(context) {
    		clReleaseContext(context);
  	}
}
