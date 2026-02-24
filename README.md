# img410_3_dds324

# Authors
Devin Jay San Nicolas
dds324@nau.edu

# Usage
Download/clone repo.

Make

main.c contains programs top level flow. Read input -> raycast -> output to ppm

raycast.c contains functions to read input scene and raycast.

raycast.c also utilizes past projects to do vector3 math.

raycast.c now implements lighting and shadows when calculating a pixel's color.

structures.h contains various datatypes created.

Run with ./raycast 200 200 input.scene output.ppm

Make clean to delete object files and .exe files.

# Known Issues
None
