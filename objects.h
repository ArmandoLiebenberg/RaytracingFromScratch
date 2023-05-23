//
// Created by aliebs on 23/05/23.
//

#ifndef RAYTRACINGFROMSCRATCH_OBJECTS_H
#define RAYTRACINGFROMSCRATCH_OBJECTS_H

#include "renderer_math.h"
#include <string>
#include  <limits>

/* Sphere
 * ------------------------
 * A sphere object with coordinates and details about its shape and color
 */
typedef struct Sphere {
    Vec3 centre {};
    float radius {};
    Vec3i color {};
    int specular{};
    float emission{};
} Sphere;

typedef struct Light {
    std::string type;
    float intensity;
    Vec3 direction;
} Light;

typedef struct TracedSphere {
    Sphere closestSphere {};
    float closestT {std::numeric_limits<float>::infinity()};
    int found {0};
} TracedSphere;

#endif //RAYTRACINGFROMSCRATCH_OBJECTS_H
