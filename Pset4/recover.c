#include <stdio.h>
#include <stdlib.h>

void createFileName();

//unsigned char buffer[512];               // One "block" contains 512 bytes

char filename[7];               // ###.jpg

int i = 0;
//unsigned int size;

FILE *jpeg = NULL;

int main (int argc, char *argv[])
{
    unsigned char buffer[512];

    // Ensure proper usage
    if(argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    // remember memory card file name
    char *infile = argv[1];

    // open the memory card file
    FILE *inptr = fopen(infile, "r");
    if(inptr == NULL)
    {
        fprintf(stderr, "File %s could not be opened.\n", infile);
        return 2;
    }

    // read in first chunk/block
    while((fread(buffer, 512, 1, inptr)) == 1)                      // As long as the block read in is 512 Bytes
    {
        //FIND START OF JPEG
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)   //good ol' bitmasking
        {
            if(i != 0)   //have we found a jpeg already?
            {
                //yes
                fclose(jpeg);       //close the previous jpeg
            }

            // now if we hadn't found a jpeg already, you'd be creating the first ever jpeg file.
            // if we had found one already, no worries. We closed the previous file up there^^^
            // and now you're safe to open newest file, go girl

            createFileName();
            jpeg = fopen(filename, "a");

            if (jpeg == NULL)
            {
                fclose(inptr);
                fprintf(stderr, "File %s could not be created.\n", filename);
                return 3;
            }
            // fwrite stuff into that jpeg file
            fwrite(buffer, 512, 1, jpeg);
            // cardfseek(inptr, 512, SEEK_CUR);                //Shift the pointer to next block
        }
        // Didn't find the start of jpeg
        else if(i != 0)   //have we found a jpeg already?
        {
            //yes, then this means that the current chunk of 512 bytes belongs to the ongoing jpeg file.
            fwrite(buffer, 512, 1, jpeg);
        }
        // size = fread(buffer, 1, 512, inptr);        //Read in the next chunk, update size in case we're reaching the end of file
    }

    fclose(jpeg);
    fclose(inptr);

    // If successful
    return 0;
}

void createFileName()
{
    /*********** NAMING THE JPEGS ***********/
    sprintf(filename,"%03i.jpg", i);
    //printf("%s\n", filename);
    i++;
}
