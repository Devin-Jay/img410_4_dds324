#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "structures.h"

// function declarations
bool readProperty(FILE *file, char *property, Scene *scene);
Scene *readInputScene(char* filename);
PPMImage *raycast(Scene *scene, int width, int height);
Vector3 shoot(Scene *scene, Vector3 rayOrigin, Vector3 rayDir);
float intersectSphere(Vector3 rayOrigin, Vector3 rayDir, Object sphere);
float intersectPlane(Vector3 rayOrigin, Vector3 rayDir, Object plane);