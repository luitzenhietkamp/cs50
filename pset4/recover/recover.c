#include <stdbool.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    // check whether infile can be opened
    FILE *infile = fopen(argv[1], "rb");
    if (infile == NULL)
    {
        fprintf(stderr, "Failed to open %s.\n", argv[1]);
        return 2;
    }

    // set up some variables
    unsigned char buffer[512];
    char file_name[8] = "000.jpg";
    int file_number = 0;
    bool write = false;
    FILE *outfile = fopen("", "wb");

    // scan through raw data
    while (fread(buffer, sizeof(buffer), 1, infile))
    {
        // if signature is found...
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            buffer[3] > 0xdf && buffer[3] < 0xf0)
        {
            // set up the correct outfile name
            file_name[0] = '0' + file_number / 100;
            file_name[1] = '0' + (file_number / 10) % 10;
            file_name[2] = '0' + file_number % 10;
            ++file_number;

            // close open outfiles and open new outfile
            if (outfile)
            {
                fclose(outfile);
            }
            outfile = fopen(file_name, "wb");

            // turn on write mode
            write = true;
        }

        // write the data to file
        if (write)
        {
            fwrite(buffer, sizeof(buffer), 1, outfile);
        }
    }

    // close files
    fclose(infile);
    if (outfile)
    {
        fclose(outfile);
    }

    return 0;
}