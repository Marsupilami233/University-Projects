/* Lab5.c
 * Yulin Xiao
 * Yulinx
 * ECE 2220, Fall 2018
 *
 * Purpose: A program to read in a bmp file and check for
 *          valid structures then uses the four instructions:
 *          ./lab5.c {trunc,center,mag,scale} input.bmp output.bmp
 *          using the command line arguements and then trying to
 *          modify the files pointers and structure pointer
 *
 * Assumptions:
 *       The input.txt must be 24-bit color, without compression, and without
 *       a color map. The users are promped to input the istructions only in
 *       command line argument, the output file is generated in the same path
 *       of the input file.
 *
 *
 * Command line argument
 *   name of bit mapped image file (bmp file) to read
 *
 * Bugs:None
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The header is 14 bytes, however on most systems
 * the sizeof(struct Header) is 16 due to alignment
 * Thus trying to read the header with one fread may fail.
 * read each Header info one by one.
 */
struct Header {
    unsigned short int Type;                 /* Magic identifier            */
    unsigned int Size;                       /* File size in bytes          */
    unsigned short int Reserved1, Reserved2;
    unsigned int Offset;                     /* Offset to image data, bytes */
};

struct InfoHeader {
    unsigned int Size;               /* header size in bytes      */
    int Width, Height;               /* Width and height of image */
    unsigned short int Planes;       /* Number of colour planes   */
    unsigned short int Bits;         /* Bits per pixel            */
    unsigned int Compression;        /* Compression type          */
    unsigned int ImageSize;          /* Image size in bytes       */
    int xResolution,yResolution;     /* Pixels per meter          */
    unsigned int Colors;             /* Number of colors         */
    unsigned int ImportantColors;    /* Important colors         */
};

const char Matrix[3][3] = {
    {  0, -1,  0 },
    { -1,  4, -1 },
    {  0, -1,  0 }
};

#define LINE 256

struct Pixel {
    unsigned char Red, Green, Blue;
};


/*----------------------------------------------------------*/

int main(int argc, char *argv[])
{
    char filein[LINE];              /*Read in the filein header file*/
    FILE *fpin;                     /*input file*/
    FILE *fpout;                    /*output file*/
    struct InfoHeader infoheader;
    struct Header header;
    int expected_bytes;
    int error_code = 0;             /*As read in the example*/
    int row, column;
    int pixel_cols, pixel_rows, pixel_count;
    int items_found;                /*Same in the example*/
    struct Pixel one_pixel;
    int result = 0;

    if (argc != 4) {
        printf(">> Usage: ./lab5.c {trunc,center,mag,scale} input.bmp output.bmp\n");
        exit(1);
    }
    strcpy(filein, argv[2]);

    if ((fpin = fopen(filein, "rb")) == NULL) {
        printf("Cannot Open File. %s\n", filein);
        exit (1);
    }

    /* Read header */
    fread(&header.Type, sizeof(short int), 1, fpin);
    fread(&header.Size, sizeof(int), 1, fpin);
    fread(&header.Reserved1, sizeof(short int), 1, fpin);
    fread(&header.Reserved2, sizeof(short int), 1, fpin);
    fread(&header.Offset, sizeof(int), 1, fpin);

    printf("header.Type = %x\n", header.Type);
    printf("header.Size = %d\n", header.Size);
    printf("header.Offset = %d\n", header.Offset);

    if (header.Type != 0x4D42) {
        printf("This does not appear to be a bmp file: %s\n", filein);
        exit(1);
    }
    fread(&infoheader.Size, sizeof(int), 1, fpin);
    fread(&infoheader.Width, sizeof(int), 1, fpin);
    fread(&infoheader.Height, sizeof(int), 1, fpin);
    fread(&infoheader.Planes, sizeof(short int), 1, fpin);
    fread(&infoheader.Bits, sizeof(short int), 1, fpin);
    fread(&infoheader.Compression, sizeof(int), 1, fpin);
    fread(&infoheader.ImageSize, sizeof(int), 1, fpin);
    fread(&infoheader.xResolution, sizeof(int), 1, fpin);
    fread(&infoheader.yResolution, sizeof(int), 1, fpin);
    fread(&infoheader.Colors, sizeof(int), 1, fpin);
    fread(&infoheader.ImportantColors, sizeof(int), 1, fpin);
    /*Useful infoheader informing about the input.bmp*/
    printf("infoheader.Size = %d\n", infoheader.Size);
    printf("infoheader.Width = %d\n", infoheader.Width);
    printf("infoheader.Height = %d\n", infoheader.Height);
    printf("infoheader.Planes = %d\n", infoheader.Planes);
    printf("infoheader.Bits = %d\n", infoheader.Bits);
    printf("infoheader.Compression = %d\n", infoheader.Compression);
    printf("infoheader.ImageSize = %d\n", infoheader.ImageSize);
    printf("infoheader.xResolution = %d\n", infoheader.xResolution);
    printf("infoheader.yResolution = %d\n", infoheader.yResolution);
    printf("infoheader.Colors = %d\n", infoheader.Colors);
    printf("infoheader.ImportantColors = %d\n", infoheader.ImportantColors);


    if (header.Offset != 54) {
        printf("problem with offset.  Cannot handle color table\n");
        error_code +=1;
    }
    if (infoheader.Size != 40) {
        printf("Size is not 40, perhaps a bmp format not handled\n");
        error_code +=2;
    }
    if (infoheader.Planes != 1 || infoheader.Compression != 0) {
        printf("Planes or Compression format not handled\n");
        error_code +=4;
    }
    if (infoheader.Bits != 24) {
        printf("Only 24 bit color handled\n");
        error_code +=8;
    }
    expected_bytes = (infoheader.Width * infoheader.Height * infoheader.Bits)/8;
    if (expected_bytes != infoheader.ImageSize) {
        error_code +=16;
    }
    if (expected_bytes + 14 + 40 != header.Size) {
        printf("Problem with size in header\n");
        error_code +=32;
    }
    if (infoheader.Colors != 0 || infoheader.ImportantColors != 0) {
        printf("Cannot handle color map\n");
        error_code +=64;
    }
    if (error_code != 0 && error_code != 16) {
        printf("exit with code %x\n", error_code);
        exit(EXIT_FAILURE);
    }

    printf("Reading pixels...\n");

    pixel_rows = infoheader.Height;
    pixel_cols = infoheader.Width;
    pixel_count = 0;



    int i;
    struct Pixel **pixel_transform;
    // allocate the spaces in heep to store the transformed matrix
    pixel_transform = (struct Pixel **)calloc(pixel_rows,sizeof(struct Pixel *));
    for (i = 0; i < pixel_rows; i ++) {
        pixel_transform[i] = (struct Pixel *)calloc(pixel_cols,sizeof(struct Pixel));
    }


    for (row = 0; row < pixel_rows; row++) {
        for (column = 0; column < pixel_cols; column++) {
            items_found = fread(&one_pixel, 3, 1, fpin);
            if (items_found != 1) {
                printf("failed to read pixel %d at [%d][%d]\n",
                       pixel_count, row, column);
                exit(1);
            }/*read from one_pixel to the allocated dynamic memory double structure*/
            pixel_transform[row][column].Red = one_pixel.Red;
            pixel_transform[row][column].Green = one_pixel.Green;
            pixel_transform[row][column].Blue = one_pixel.Blue;
            pixel_count++;
        }
    }
    fclose(fpin);
    printf("Read Input File:%s Successful!\n",argv[2]);
    //pixel_transform by mulitiplying the matrix with pixels and sum them up
    if ((fpout = fopen(argv[3], "w")) == NULL ) {
        printf("Cannot write to file : \"%s\"\n", argv[3]);
        exit(1);
    }
    fwrite(&header.Type, sizeof(short int), 1, fpout);
    fwrite(&header.Size, sizeof(int), 1, fpout);
    fwrite(&header.Reserved1, sizeof(short int), 1, fpout);
    fwrite(&header.Reserved2, sizeof(short int), 1, fpout);
    fwrite(&header.Offset, sizeof(int), 1, fpout);

    fwrite(&infoheader.Size, sizeof(int), 1, fpout);
    fwrite(&infoheader.Width, sizeof(int), 1, fpout);
    fwrite(&infoheader.Height, sizeof(int), 1, fpout);
    fwrite(&infoheader.Planes, sizeof(short int), 1, fpout);
    fwrite(&infoheader.Bits, sizeof(short int), 1, fpout);
    fwrite(&infoheader.Compression, sizeof(int), 1, fpout);

    fwrite(&infoheader.ImageSize, sizeof(int), 1, fpout);
    fwrite(&infoheader.xResolution, sizeof(int), 1, fpout);
    fwrite(&infoheader.yResolution, sizeof(int), 1, fpout);
    fwrite(&infoheader.Colors, sizeof(int), 1, fpout);
    fwrite(&infoheader.ImportantColors, sizeof(int), 1, fpout);

    for (row = 0; row < pixel_rows; row ++) {                       /*First loop is for rows, then for columns*/
        for (column = 0; column < pixel_cols; column ++) {
            if(row == 0||row == (pixel_rows - 1)) {                 //If it is the edge of the image, read directly
                fwrite(&pixel_transform[row][column], 3, 1, fpout); //from the transformed pixels read from input files
            } else if(column == 0||column == (pixel_cols - 1)) {    //"else if" critical, for the columns that are not
                fwrite(&pixel_transform[row][column], 3, 1, fpout); //at the first and last row, preventing seg fault
            } else {                                                //the same

                result = Matrix[0][0] * pixel_transform[row - 1][column - 1].Red + //performing the .* point multiply
                         Matrix[0][1] * pixel_transform[row - 1][column].Red + Matrix[0][2] * pixel_transform[row - 1][column + 1].Red +
                         Matrix[1][0] * pixel_transform[row][column - 1].Red + Matrix[1][1] * pixel_transform[row][column].Red +
                         Matrix[1][2] * pixel_transform[row][column + 1].Red + Matrix[2][0] * pixel_transform[row + 1][column - 1].Red +
                         Matrix[2][1] * pixel_transform[row + 1][column].Red + Matrix[2][2] * pixel_transform[row + 1][column + 1].Red ;

                if (strcmp(argv[1],"trunc") == 0) {                 //instruction : trunc, just simply let it blank
                    ;                                               //to store in new pixel. Doing the convert in
                } else if (strcmp(argv[1],"center") == 0) {         //just one whole loop makes program faster and
                    result += 128;                                  //instruction more efficient
                    if(result < 0) {                                //center and clip
                        result = 0;
                    }
                    if(result > 255) {
                        result = 255;
                    }
                } else if (strcmp(argv[1],"mag") == 0) {            //magnitude and clip
                    result = abs(result);
                    if(result > 255) {
                        result = 255;
                    }
                } else if (strcmp(argv[1],"scale") == 0) {          //scale it and center it
                    result /= 8;
                    result += 128;
                }
                one_pixel.Red = result;                             // reused int result

                result = Matrix[0][0] * pixel_transform[row - 1][column - 1].Green +
                         Matrix[0][1] * pixel_transform[row - 1][column].Green + Matrix[0][2] * pixel_transform[row - 1][column + 1].Green +
                         Matrix[1][0] * pixel_transform[row][column - 1].Green + Matrix[1][1] * pixel_transform[row][column].Green +
                         Matrix[1][2] * pixel_transform[row][column + 1].Green + Matrix[2][0] * pixel_transform[row + 1][column - 1].Green +
                         Matrix[2][1] * pixel_transform[row + 1][column].Green + Matrix[2][2] * pixel_transform[row + 1][column + 1].Green ;

                if (strcmp(argv[1],"trunc") == 0) {
                    ;
                } else if (strcmp(argv[1],"center") == 0) {
                    result += 128;
                    if (result < 0) {
                        result = 0;
                    }
                    if (result > 255) {
                        result = 255;
                    }
                } else if (strcmp(argv[1],"mag") == 0) {
                    result = abs(result);
                    if(result > 255) {
                        result = 255;
                    }
                } else if (strcmp(argv[1],"scale") == 0) {
                    result /= 8;
                    result += 128;
                }
                one_pixel.Green = result;

                result = Matrix[0][0] * pixel_transform[row - 1][column - 1].Blue +
                         Matrix[0][1] * pixel_transform[row - 1][column].Blue + Matrix[0][2] * pixel_transform[row - 1][column + 1].Blue +
                         Matrix[1][0] * pixel_transform[row][column - 1].Blue + Matrix[1][1] * pixel_transform[row][column].Blue +
                         Matrix[1][2] * pixel_transform[row][column + 1].Blue + Matrix[2][0] * pixel_transform[row + 1][column - 1].Blue +
                         Matrix[2][1] * pixel_transform[row + 1][column].Blue + Matrix[2][2] * pixel_transform[row + 1][column + 1].Blue ;

                if (strcmp(argv[1],"trunc") == 0) {
                    ;
                } else if (strcmp(argv[1],"center") == 0) {
                    result += 128;
                    if (result < 0) {
                        result = 0;
                    }
                    if (result > 255) {
                        result = 255;
                    }
                } else if (strcmp(argv[1],"mag") == 0) {
                    result = abs(result);
                    if(result > 255) {
                        result = 255;
                    }
                } else if (strcmp(argv[1],"scale") == 0) {
                    result /= 8;
                    result += 128;
                }
                one_pixel.Blue = result;

                fwrite(&one_pixel, 3, 1, fpout);                          //all done in the middle of the image!
            }

        }
    }
    fclose(fpout);
    printf("Output File:%s is Ready!\nAll Done!\n",argv[3]);
//-----------------------------------------------------------------------------------
    for(i = 0; i < pixel_rows; i ++) {                                   //Free the memory
        free(pixel_transform[i]);
        pixel_transform[i] = NULL;
    }
    free(pixel_transform);
    pixel_transform = NULL;

    return 0;
}
