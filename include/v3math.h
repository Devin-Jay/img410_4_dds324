#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

typedef struct
{
    float x;
    float y;
    float z;
} Vector3;

Vector3 v3_from_points(Vector3 a, Vector3 b);
Vector3 v3_add(Vector3 a, Vector3 b);
Vector3 v3_subtract(Vector3 a, Vector3 b);
float v3_dot_product(Vector3 a, Vector3 b);
Vector3 v3_cross_product(Vector3 a, Vector3 b);
Vector3 v3_scale(Vector3 a, float s);
float v3_angle(Vector3 a, Vector3 b);
float v3_angle_quick(Vector3 a, Vector3 b);
Vector3 v3_reflect(Vector3 v, Vector3 n);
float v3_length(Vector3 a);
Vector3 v3_normalize(Vector3 a);