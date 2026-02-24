#include "raycast.h"

// argv[1] -> width
// argv[2] -> height
// argv[3] -> input file
// argv[4] -> output file
// argc -> 5

int main(int argc, char* argv[])
{
    // Handle improper arguments
    if (argc < 5)
    {
        fprintf(stderr, "Error: Missing input/output filenames\n");
        return 1;
    }

    // parse input scene file and create scene struct
    Scene *scene = readInputScene(argv[3]);

    // if scene input fails, exit with error
    if (!scene)
    {
        return 1;
    }

    // raycast scene and create output image
    PPMImage *result = raycast(scene, atoi(argv[1]), atoi(argv[2]));

    // output image to ppm file
    writeOutputPPMFile(argv[4], result);

    // free allocated memory
    free(scene->objects);
    free(scene->lights);
    free(scene);
    free(result->pixels);
    free(result);

	return 0;
}