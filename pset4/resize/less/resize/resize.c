#include <math.h>
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

    // remember arguments
    int factor = atoi(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    // check for valid factor
    if (factor < 1 || factor > 100)
    {
        fprintf(stderr, "Error resize factor must be in the range [1, 100].\n");
        return 5;
    }

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // create output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fprintf(stderr, "Could not create %s.\n", outfile);
        fclose(inptr);
        return 3;
    }

    // copy the headers from the infile
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(inptr);
        fclose(outptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // store the old proportions of the bitmap
    int biWidth_old = bi.biWidth;
    int biHeight_old = bi.biHeight;

    // calculate the new proportions of the bitmap
    bi.biWidth *= factor;
    bi.biHeight *= factor;

    // calculate the padding
    int padding_old = (4 - (biWidth_old * sizeof(RGBTRIPLE)) % 4) % 4;
    int padding_new = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // calculate the new sizes of the bitmap
    bi.biSizeImage = abs(bi.biHeight) * (bi.biWidth * sizeof(RGBTRIPLE) + padding_new);
    bf.bfSize = bf.bfOffBits + bi.biSizeImage;

    // write the new headers to the outfile
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0; i < abs(biHeight_old); ++i)
    {
        // write each scanline factor times
        for (int l = 0; l < factor; ++l)
        {
            // loop through each pixel in the scanline
            for (int j = 0; j < biWidth_old; ++j)
            {
                // temporary storage
                RGBTRIPLE temp;

                // read RGBTRIPLE from infile
                fread(&temp, sizeof(RGBTRIPLE), 1, inptr);

                // copy RGBTRIPLE factor times to new_scanline
                for (int k = 0; k < factor; ++k)
                {
                    // write the RGBTRIPLE in the outfile
                    fwrite(&temp, sizeof(RGBTRIPLE), 1, outptr);
                }
            }


            // skip over padding if any
            fseek(inptr, padding_old, SEEK_CUR);

            // add padding to outptr
            for (int j = 0; j < padding_new; ++j)
            {
                fputc(0x00, outptr);
            }

            // return the file pointer to the start of the line if line needs to be copied again
            if (l != factor - 1)
            {
                fseek(inptr, -(biWidth_old * sizeof(RGBTRIPLE) + padding_old), SEEK_CUR);
            }
        }
    }

    // close files
    fclose(inptr);
    fclose(outptr);

    return 0;
}