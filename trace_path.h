//
// Created by aliebs on 28/05/23.
//

#ifndef RAYTRACINGFROMSCRATCH_TRACE_PATH_H
#define RAYTRACINGFROMSCRATCH_TRACE_PATH_H

#include "objects.h"

Vec3i trace_path(Vec3 point, Vec3 direction, Sphere objects[], Light lights[]);
Vec3i direct_lighting(Vec3 point, Sphere objects[], Light lights[]);
Vec3 vector_hemisphere(Vec3 point, Vec3 normal);
Vec3 local_coordinates(Vec3 normal);
Vec3 sample_hemisphere(float &r1, float &r2);
bool intersect_ray_object(Vec3 point, Vec3 direction, Sphere objectList[], Sphere closestObject, uint32_t &t);
float calculate_lighting(Vec3 point, Vec3 direction, Light lights[], float intensity);

#endif //RAYTRACINGFROMSCRATCH_TRACE_PATH_H
