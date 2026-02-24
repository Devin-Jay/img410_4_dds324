#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "v3math.h"
#include "utils.h"

typedef enum {SPHERE, PLANE} ObjectType;

typedef struct
{
    Vector3 pos;
    float width;
    float height;
} Camera;

typedef struct
{
    ObjectType type;
    Vector3 pos;
    Vector3 normal; // For planes
    float radius;   // For spheres
    Vector3 color;
    Vector3 specular;
} Object;

typedef struct
{
    Vector3 pos;
    Vector3 color;
    Vector3 direction;
    float radiala0;
    float radiala1;
    float radiala2;
    float theta;
    float angulara0;
} Light;

typedef struct 
{
    Object* objects;
    Light* lights;
    Camera camera;
    int objectNum;
    int lightNum;
} Scene;