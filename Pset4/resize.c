/**
 * Resize a BMP image bit by bit
 */

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    // ensure n is within bounds
    int n;
    n = atoi(argv[1]);
    if(n < 1 || n > 100)
    {
        fprintf(stderr, "n must be between 1 and 100, inclusive\n");
        return 1;
    }
    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");           //If successful, fopen will return a pointer to the file. Otherwise, it will return NULL
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // create/open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
/***************************************************** OUTFILE HEADER SHIT ********************************************************/
    long oldWidth, oldHeight;

    // save it
    oldWidth = bi.biWidth;
    oldHeight = bi.biHeight;
    int oldPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // change it
    bi.biWidth = bi.biWidth * n;
    bi.biHeight = bi.biHeight * n;

     // determine new padding for output file
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;               //sizeof(RGBTRIPLE) = 3

    // caculate new things
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + padding) * abs(bi.biHeight);
    bf.bfSize =  bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

/**********************************************************************************************************************************/

/***************************************************** RESIZING STARTS HERE *******************************************************/

    RGBTRIPLE array[bi.biWidth];                                    //Pixel-storing array. It'll store the stretched out row

    // iterate over infile's scanlines, i.e. EACH ROW
    for (int i = 0, biHeight = labs(oldHeight); i < biHeight; i++)
    {
        int m = 0;                                                  //Pixel-storing array's index

        // iterate over pixels in scanline, i.e. EACH PIXEL
        for (int j = 0; j < oldWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            for(int k = 0; k<n; k++)
            {
                // For each pixel at [i,j] WRITE TO ARRAY N TIMES
                array[m] = triple;
                m++;                                                //Increment the index and store the same pixel again
            }
        }
        for (int k = 0; k<n; k++)
        {
            // WRITE ARRAY TO OUTFILE
            fwrite(&array, sizeof(array), 1, outptr);               //Write the stretched out row n times

            // ADD PADDING, IF ANY
            for (int l = 0; l < padding; l++)
            {
                fputc(0x00, outptr);
            }
        }

        // skip over padding, if any
        fseek(inptr, oldPadding, SEEK_CUR);
    }
/**********************************************************************************************************************************/
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
