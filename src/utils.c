#include "utils.h"

// function to write PPMImage struct to output ppm file
void writeOutputPPMFile(char *filename, PPMImage* img)
{
    // implementation for writing PPM file
    printf("Writing PPM file: %s\n", filename);
    FILE *fh = fopen(filename, "w");

    // write header
    fprintf(fh, "P3\n");
    fprintf(fh, "%d %d\n", img->width, img->height);
    fprintf(fh, "255\n");

    int pixel_count = 3 * img->width * img->height;

    // write pixel data
    for (size_t i = 0; i < pixel_count; i++)
    {
        fprintf(fh, "%d ", img->pixels[i]);
        if ((i + 1) % (img->width * 3) == 0)
        {
            fprintf(fh, "\n");
        }
    }

    fclose(fh);
}

