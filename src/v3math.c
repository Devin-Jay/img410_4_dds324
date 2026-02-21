#include "v3math.h"

// DEVIN
Vector3 v3_from_points(Vector3 a, Vector3 b) // form v3 from a to b
{
    Vector3 result;
    result.x = b.x - a.x;
    result.y = b.y - a.y;
    result.z = b.z - a.z;
    return result;
}

Vector3 v3_add(Vector3 a, Vector3 b)
{
    Vector3 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

Vector3 v3_subtract(Vector3 a, Vector3 b)
{
    Vector3 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

float v3_dot_product(Vector3 a, Vector3 b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

Vector3 v3_cross_product(Vector3 a, Vector3 b)
{
    Vector3 result;
    result.x = a.y*b.z - a.z*b.y;
    result.y = a.z*b.x - a.x*b.z;
    result.z = a.x*b.y - a.y*b.x;
    return result;
}

Vector3 v3_scale(Vector3 a, float s)
{
    Vector3 result;
    result.x = a.x * s;
    result.y = a.y * s;
    result.z = a.z * s;
    return result;
}

float v3_angle(Vector3 a, Vector3 b) // angle between a and b
{
    return acosf(v3_dot_product(a, b) / (v3_length(a) * v3_length(b)));
}

float v3_angle_quick(Vector3 a, Vector3 b) // angle between a and b; no cos-1
{
    //dot product / length of a * length of b = cos0
    //do I need to check to make sure I don't divide by 0?
    
    return v3_dot_product(a, b) / (v3_length(a) * v3_length(b));
}

Vector3 v3_reflect(Vector3 v, Vector3 n)
{
    Vector3 result;
    float dot = v3_dot_product(v, n);
    result.x = v.x - 2 * dot * n.x;
    result.y = v.y - 2 * dot * n.y;
    result.z = v.z - 2 * dot * n.z;
    return result;
}

float v3_length(Vector3 a)
{
    //this is getting the length squared (aax + aay + aaz) and then getting the sqrt of that, which is length
    return sqrtf(v3_dot_product(a, a));
}

Vector3 v3_normalize(Vector3 a)
{
    Vector3 result;
    float length = v3_length(a);
    if (length == 0.0) {
        result.x = 0.0;
        result.y = 0.0;
        result.z = 0.0;
        return result;
    }

    result.x = a.x / length;
    result.y = a.y / length;
    result.z = a.z / length;
    return result;
}