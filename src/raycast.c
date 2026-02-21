#include "raycast.h"

// function that reads object's properties and updates scene objects
// returns true if property is valid, false otherwise
bool readProperty(FILE *file, char *property, Scene *scene)
{
    if (strcmp(property, "width:") == 0)
    {
        // read width value
        fscanf(file, "%f", &scene->camera.width);
        return true;
    }
    else if (strcmp(property, "height:") == 0)
    {
        // read height value
        fscanf(file, "%f", &scene->camera.height);
        return true;
    }
    else if (strcmp(property, "position:") == 0)
    {
        // read position values
        fscanf(file, "%f %f %f", &scene->objects[scene->objectNum].pos.x, &scene->objects[scene->objectNum].pos.y, &scene->objects[scene->objectNum].pos.z);
        return true;
    }
    else if (strcmp(property, "c_diff:") == 0)
    {
        // read color values
        fscanf(file, "%f %f %f", &scene->objects[scene->objectNum].color.x, &scene->objects[scene->objectNum].color.y, &scene->objects[scene->objectNum].color.z);
        return true;
    }
    else if (strcmp(property, "radius:") == 0)
    {
        // read radius value
        fscanf(file, "%f", &scene->objects[scene->objectNum].radius);
        return true;
    }
    else if (strcmp(property, "normal:") == 0)
    {
        // read normal values
        fscanf(file, "%f %f %f", &scene->objects[scene->objectNum].normal.x, &scene->objects[scene->objectNum].normal.y, &scene->objects[scene->objectNum].normal.z);
        return true;
    }

    return false; // invalid property ; object found
}

// function that reads input scene file and creates scene struct
Scene *readInputScene(char* filename)
{
    // create scene
    Scene *scene = (Scene*)malloc(sizeof(Scene));

    // open file
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return NULL;
    }

    // read scene identifier and make sure it's valid
    char identifier[20];
    fscanf(file, "%12s", identifier);

    if (strcmp(identifier, "img410scene") != 0)
    {
        fprintf(stderr, "Error: Invalid scene file format\n");
        fclose(file);
        free(scene);
        return NULL;
    }

    // initialize scene variables
    scene->objectNum = 0;
    scene->objects = (Object*)malloc(sizeof(Object) * 128);

    // check for one object
    int result = fscanf(file, "%s", identifier);
    
    if (result == EOF || strcmp(identifier, "end") == 0)
    {
        fprintf(stderr, "Error: No objects found in scene file\n");
        fclose(file);
        free(scene->objects);
        free(scene);
        return NULL;
    }

    // loop until end of file
    while (strcmp(identifier, "end") != 0)
    {
        // check for object type
        if (strcmp(identifier, "camera") == 0 || strcmp(identifier, "camera;") == 0)
        {
            // set default camera values
            scene->camera.pos.x = 0.0f;
            scene->camera.pos.y = 0.0f;
            scene->camera.pos.z = 0.0f;
            scene->camera.width = 0.0f;
            scene->camera.height = 0.0f;

            // read properties
            while (1)
            {
                result = fscanf(file, "%s", identifier);

                // if not a valid property
                if (!readProperty(file, identifier, scene))
                {
                    // check for ;
                    if (strcmp(identifier, ";") == 0)
                    {
                        result = fscanf(file, "%s", identifier);
                    }

                    // assume next object or end
                    break;
                }
            }
        }
        else if (strcmp(identifier, "sphere") == 0 || strcmp(identifier, "sphere;") == 0)
        {
            // set default sphere values
            scene->objects[scene->objectNum].type = SPHERE;
            scene->objects[scene->objectNum].pos.x = 0.0f;
            scene->objects[scene->objectNum].pos.y = 0.0f;
            scene->objects[scene->objectNum].pos.z = 0.0f;
            scene->objects[scene->objectNum].radius = 0.0f;
            scene->objects[scene->objectNum].color.x = 0.0f;
            scene->objects[scene->objectNum].color.y = 0.0f;
            scene->objects[scene->objectNum].color.z = 0.0f;

            // read properties
            while (1)
            {
                result = fscanf(file, "%s", identifier);

                // if not a valid property
                if (!readProperty(file, identifier, scene))
                {
                    // check for ;
                    if (strcmp(identifier, ";") == 0)
                    {
                        // get next object or end
                        result = fscanf(file, "%s", identifier);
                    }

                    scene->objectNum++;

                    // assume next object or end
                    break;
                }
            }
        }
        else if (strcmp(identifier, "plane") == 0)
        {
            // set default plane values
            scene->objects[scene->objectNum].type = PLANE;
            scene->objects[scene->objectNum].pos.x = 0.0f;
            scene->objects[scene->objectNum].pos.y = 0.0f;
            scene->objects[scene->objectNum].pos.z = 0.0f;
            scene->objects[scene->objectNum].normal.x = 0.0f;
            scene->objects[scene->objectNum].normal.y = 0.0f;
            scene->objects[scene->objectNum].normal.z = 0.0f;
            scene->objects[scene->objectNum].color.x = 0.0f;
            scene->objects[scene->objectNum].color.y = 0.0f;
            scene->objects[scene->objectNum].color.z = 0.0f;

            // read properties
            while (1)
            {
                result = fscanf(file, "%s", identifier);

                // if not a valid property
                if (!readProperty(file, identifier, scene))
                {
                    // check for ;
                    if (strcmp(identifier, ";") == 0)
                    {
                        result = fscanf(file, "%s", identifier);
                    }

                    scene->objectNum++;

                    // assume next object or end
                    break;
                }
            }
        }
        else
        {
            fprintf(stderr, "Error: Invalid object type '%s' in scene file\n", identifier);
            fclose(file);
            free(scene->objects);
            free(scene);
            return NULL;
        }

        if (result == EOF) {
            break;
        }
    }

    fclose(file);

    return scene;
}

// function that raycasts scene and creates output image
PPMImage *raycast(Scene *scene, int width, int height)
{
    // create new image
    PPMImage *img = (PPMImage*)malloc(sizeof(PPMImage));
    img->width = width;
    img->height = height;
    img->pixels = (uint8_t*)malloc(3 * img->width * img->height);

    // initialize vars for raycasting
    Vector3 camPos = scene->camera.pos;
    Vector3 pixColor, rayDir;

    // loop through each pixel in the image
    for (int y = 0; y < img->height; y++)
    {
        for (int x = 0; x < img->width; x++)
        {
            // compute pixel position in world space and ray direction
            float u = (x + 0.5f) / img->width;
            float v = (y + 0.5f) / img->height;

            rayDir.x = -scene->camera.width  / 2 + u * scene->camera.width;
            rayDir.y =  scene->camera.height / 2 - v * scene->camera.height;  // FLIPPED
            rayDir.z = -1.0f;

            // shoot ray from camera position towards pixel and get color of closest intersecting object
            pixColor = shoot(scene, camPos, v3_normalize(rayDir));

            // set pix color to color of closest intersecting object
            img->pixels[3 * (y * img->width + x)] = (uint8_t)(pixColor.x * 255.0f);
            img->pixels[3 * (y * img->width + x) + 1] = (uint8_t)(pixColor.y * 255.0f); 
            img->pixels[3 * (y * img->width + x) + 2] = (uint8_t)(pixColor.z * 255.0f);
        }
    }

    return img;
}

// function that shoots ray from camera position towards pixel and returns color of closest intersecting object
Vector3 shoot(Scene *scene, Vector3 rayOrigin, Vector3 rayDir)
{
    // initialize closest object and distance
    int closestObjIndex = -1;
    float closestDist = INFINITY;

    // loop through objects in scene and check for intersection with ray
    for (int i = 0; i < scene->objectNum; i++)
    {
        float dist;

        if (scene->objects[i].type == SPHERE)
        {
            dist = intersectSphere(rayOrigin, rayDir, scene->objects[i]);
        }
        else if (scene->objects[i].type == PLANE)
        {
            dist = intersectPlane(rayOrigin, rayDir, scene->objects[i]);
        }

        // if ray intersects object and is closer than previous closest object, update closest object and distance
        if (dist > 0 && dist < closestDist)
        {
            closestDist = dist;
            closestObjIndex = i;
        }
    }

    // if no objects were intersected, return background color
    if (closestObjIndex == -1)
    {
        Vector3 bgColor = {0.0f, 0.0f, 0.0f};
        return bgColor;
    }

    // otherwise, return color of closest object
    return scene->objects[closestObjIndex].color;
}

// function that checks for intersection between ray and sphere object
// returns distance to intersection or -1 if no intersection
float intersectSphere(Vector3 rayOrigin, Vector3 rayDir, Object sphere)
{
    Vector3 oc = v3_subtract(rayOrigin, sphere.pos);
    float a = v3_dot_product(rayDir, rayDir);
    float b = 2.0f * v3_dot_product(oc, rayDir);
    float c = v3_dot_product(oc, oc) - sphere.radius * sphere.radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
    {
        // no intersection
        return -1.0f;
    }
    else
    {
        float t1 = (-b - sqrtf(discriminant)) / (2.0f * a);
        float t2 = (-b + sqrtf(discriminant)) / (2.0f * a);

        if (t1 > 0)
        {
            // return closer intersection
            return t1;
        }
        else if (t2 > 0)
        {
            // return farther intersection
            return t2;
        }
        else
        {
            // both intersections are behind the ray origin
            return -1.0f;
        }
    }
}

// function that checks for intersection between ray and plane object
// returns distance to intersection or -1 if no intersection
float intersectPlane(Vector3 rayOrigin, Vector3 rayDir, Object plane)
{
    float denom = v3_dot_product(plane.normal, rayDir);

    // check if ray is not parallel to plane
    if (fabs(denom) > 1e-6)
    {
        Vector3 p0l0 = v3_subtract(plane.pos, rayOrigin);
        float t = v3_dot_product(p0l0, plane.normal) / denom;

        // return intersection distance or -1 if behind ray origin
        return (t >= 0) ? t : -1.0f;
    }

    // no intersection, ray is parallel to plane
    return -1.0f;
}