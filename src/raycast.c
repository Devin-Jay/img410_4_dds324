#include "raycast.h"

// function that reads object's properties and updates scene objects
// returns true if property is valid, false otherwise
bool readProperty(FILE *file, char *property, Scene *scene, bool isLight)
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
        if (isLight) {
            // read light position values
            fscanf(file, "%f %f %f", &scene->lights[scene->lightNum].pos.x, &scene->lights[scene->lightNum].pos.y, &scene->lights[scene->lightNum].pos.z);
        } else {
            // read object position values
            fscanf(file, "%f %f %f", &scene->objects[scene->objectNum].pos.x, &scene->objects[scene->objectNum].pos.y, &scene->objects[scene->objectNum].pos.z);
        }
        return true;
    }
    else if (strcmp(property, "c_diff:") == 0)
    {
        // read color values
        fscanf(file, "%f %f %f", &scene->objects[scene->objectNum].color.x, &scene->objects[scene->objectNum].color.y, &scene->objects[scene->objectNum].color.z);
        return true;
    }
    else if (strcmp(property, "c_spec:") == 0)
    {
        // read specular color values
        fscanf(file, "%f %f %f", &scene->objects[scene->objectNum].specular.x, &scene->objects[scene->objectNum].specular.y, &scene->objects[scene->objectNum].specular.z);
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
    else if (strcmp(property, "color:") == 0)
    {
        // read color values
        fscanf(file, "%f %f %f", &scene->lights[scene->lightNum].color.x, &scene->lights[scene->lightNum].color.y, &scene->lights[scene->lightNum].color.z);
        return true;
    }
    else if (strcmp(property, "direction:") == 0)
    {
        // read direction values
        fscanf(file, "%f %f %f", &scene->lights[scene->lightNum].direction.x, &scene->lights[scene->lightNum].direction.y, &scene->lights[scene->lightNum].direction.z);
        return true;
    }
    else if (strcmp(property, "radial_a0:") == 0)
    {
        // read radiala0 value
        fscanf(file, "%f", &scene->lights[scene->lightNum].radiala0);
        return true;
    }
    else if (strcmp(property, "radial_a1:") == 0)
    {
        // read radiala1 value
        fscanf(file, "%f", &scene->lights[scene->lightNum].radiala1);
        return true;
    }
    else if (strcmp(property, "radial_a2:") == 0)
    {
        // read radiala2 value
        fscanf(file, "%f", &scene->lights[scene->lightNum].radiala2);
        return true;
    }
    else if (strcmp(property, "theta:") == 0)
    {
        // read theta value
        fscanf(file, "%f", &scene->lights[scene->lightNum].theta);
        return true;
    }
    else if (strcmp(property, "angular_a0:") == 0)
    {
        // read angulara0 value
        fscanf(file, "%f", &scene->lights[scene->lightNum].angulara0);
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
    scene->lightNum = 0;
    scene->lights = (Light*)malloc(sizeof(Light) * 128);

    // check for one object
    int result = fscanf(file, "%s", identifier);
    
    if (result == EOF || strcmp(identifier, "end") == 0)
    {
        fprintf(stderr, "Error: No objects found in scene file\n");
        fclose(file);
        free(scene->objects);
        free(scene->lights);
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
                if (!readProperty(file, identifier, scene, false))
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
            scene->objects[scene->objectNum].specular.x = 0.0f;
            scene->objects[scene->objectNum].specular.y = 0.0f;
            scene->objects[scene->objectNum].specular.z = 0.0f;

            // read properties
            while (1)
            {
                result = fscanf(file, "%s", identifier);

                // if not a valid property
                if (!readProperty(file, identifier, scene, false))
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
        else if (strcmp(identifier, "plane") == 0 || strcmp(identifier, "plane;") == 0)
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
            scene->objects[scene->objectNum].specular.x = 0.0f;
            scene->objects[scene->objectNum].specular.y = 0.0f;
            scene->objects[scene->objectNum].specular.z = 0.0f;

            // read properties
            while (1)
            {
                result = fscanf(file, "%s", identifier);

                // if not a valid property
                if (!readProperty(file, identifier, scene, false))
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
        else if (strcmp(identifier, "light") == 0 || strcmp(identifier, "light;") == 0)
        {
            // set default light values
            scene->lights[scene->lightNum].pos.x = 0.0f;
            scene->lights[scene->lightNum].pos.y = 0.0f;
            scene->lights[scene->lightNum].pos.z = 0.0f;
            scene->lights[scene->lightNum].color.x = 0.0f;
            scene->lights[scene->lightNum].color.y = 0.0f;
            scene->lights[scene->lightNum].color.z = 0.0f;
            scene->lights[scene->lightNum].direction.x = 0.0f;
            scene->lights[scene->lightNum].direction.y = 0.0f;
            scene->lights[scene->lightNum].direction.z = 0.0f;
            scene->lights[scene->lightNum].radiala0 = 0.0f;
            scene->lights[scene->lightNum].radiala1 = 0.0f;
            scene->lights[scene->lightNum].radiala2 = 0.0f;
            scene->lights[scene->lightNum].theta = 0.0f;
            scene->lights[scene->lightNum].angulara0 = 0.0f;

            // read properties
            while (1)
            {
                result = fscanf(file, "%s", identifier);

                // if not a valid property
                if (!readProperty(file, identifier, scene, true))
                {
                    // check for ;
                    if (strcmp(identifier, ";") == 0)
                    {
                        result = fscanf(file, "%s", identifier);
                    }

                    scene->lightNum++;

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
            free(scene->lights);
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
            float r = fminf(1.0f, fmaxf(0.0f, pixColor.x));
            float g = fminf(1.0f, fmaxf(0.0f, pixColor.y));
            float b = fminf(1.0f, fmaxf(0.0f, pixColor.z));

            img->pixels[3 * (y * img->width + x)]     = (uint8_t)(r * 255.0f);
            img->pixels[3 * (y * img->width + x) + 1] = (uint8_t)(g * 255.0f);
            img->pixels[3 * (y * img->width + x) + 2] = (uint8_t)(b * 255.0f);
        }
    }

    return img;
}

// function that shoots ray from camera position towards pixel and returns color of closest intersecting object
// lightning and shadows applied
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

    Object obj = scene->objects[closestObjIndex];

    // Compute intersection point
    Vector3 intersectionPoint = {
        rayOrigin.x + closestDist * rayDir.x,
        rayOrigin.y + closestDist * rayDir.y,
        rayOrigin.z + closestDist * rayDir.z
    };

    // Compute surface normal
    Vector3 normal;

    // check if object is sphere or plane and compute normal accordingly
    if (obj.type == SPHERE)
    {
        normal = v3_normalize(v3_subtract(intersectionPoint, obj.pos));
    }
    else
    {
        normal = v3_normalize(obj.normal);
    }

    // Flip normal if necessary
    if (v3_dot_product(normal, rayDir) > 0)
    {
        normal = v3_scale(normal, -1.0f);
    }

    // View direction
    Vector3 viewDir = v3_normalize(v3_subtract(rayOrigin, intersectionPoint));

    // initialize final color
    Vector3 finalColor = {0.0f, 0.0f, 0.0f};

    // Loop through lights
    for (int i = 0; i < scene->lightNum; i++)
    {
        Light light = scene->lights[i];

        Vector3 lightVec = v3_subtract(light.pos, intersectionPoint);
        float distToLight = v3_length(lightVec);
        Vector3 lightDir = v3_normalize(lightVec);

        // Shadow Ray
        Vector3 shadowOrigin = v3_add(intersectionPoint, v3_scale(normal, 0.0001f));

        int inShadow = 0;

        // loop through objects to check if shadow ray is blocked
        for (int j = 0; j < scene->objectNum; j++)
        {
            float shadowDist;

            if (scene->objects[j].type == SPHERE)
            {
                shadowDist = intersectSphere(shadowOrigin, lightDir, scene->objects[j]);
            }
            else
            {
                shadowDist = intersectPlane(shadowOrigin, lightDir, scene->objects[j]);
            }

            if (shadowDist > 0 && shadowDist < distToLight)
            {
                inShadow = 1;
                break;
            }
        }

        if (inShadow)
            continue;

        // Radial attenuation
        float denom = light.radiala0 +
                        light.radiala1 * distToLight +
                        light.radiala2 * distToLight * distToLight;

        float frad = (denom > 0.0f) ? (1.0f / denom) : 1.0f;

        float fang = 1.0f;

        if (light.theta > 0.0f)
        {
            Vector3 spotDir = v3_normalize(light.direction);

            // Light points OUTWARD, so reverse it
            float cosAlpha = v3_dot_product(v3_scale(lightDir, -1.0f), spotDir);

            float cosTheta = cosf(light.theta * M_PI / 180.0f);

            if (cosAlpha < cosTheta)
            {
                // outside spotlight cone
                continue;
            }

            fang = powf(cosAlpha, light.angulara0);
            frad *= fang;
        }

        // Diffuse
        float diffFactor = fmaxf(0.0f, v3_dot_product(normal, lightDir));

        Vector3 diffuse = {
            obj.color.x * light.color.x * diffFactor,
            obj.color.y * light.color.y * diffFactor,
            obj.color.z * light.color.z * diffFactor
        };

        // Specular
        Vector3 reflectDir = v3_subtract(v3_scale(normal, 2.0f * v3_dot_product(normal, lightDir)), lightDir);

        reflectDir = v3_normalize(reflectDir);

        float specFactor = powf(fmaxf(0.0f, v3_dot_product(reflectDir, viewDir)), 20.0f);

        Vector3 specular = {
            obj.specular.x * light.color.x * specFactor,
            obj.specular.y * light.color.y * specFactor,
            obj.specular.z * light.color.z * specFactor
        };

        // Accumulate light contribution
        finalColor.x += frad * (diffuse.x + specular.x);
        finalColor.y += frad * (diffuse.y + specular.y);
        finalColor.z += frad * (diffuse.z + specular.z);
    }

    return finalColor;
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