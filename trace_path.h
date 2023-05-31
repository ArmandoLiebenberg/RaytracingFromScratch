//
// Created by aliebs on 28/05/23.
//

#ifndef RAYTRACINGFROMSCRATCH_TRACE_PATH_H
#define RAYTRACINGFROMSCRATCH_TRACE_PATH_H

#include "objects.h"

Vec3i trace_path(Vec3 point, Vec3 direction, Sphere objects[], Light lights[]);
Vec3 vector_hemisphere(Vec3 point, Vec3 normal);
Vec3 local_coordinates(Vec3 normal);
Vec3 sample_hemisphere(float &r1, float &r2);

Vec3i direct_lighting_sphere(Vec3 origin, Vec3 transformed, Sphere closestSphere, float closestT, Sphere scene[], Light lights[]);
Vec3i direct_lighting_sphere(Vec3 origin, Vec3 transformed, Sphere closestSphere, float closestT, Sphere scene[], Light lights[]);
bool intersect_ray_sphere(Vec3 origin, Vec3 direction, Sphere sphere, float &t1, float &t2);
double compute_direct_lighting_sphere(Sphere scene[], Vec3 point, Vec3 normal, Vec3 view, Light lights[], int specular);
bool closest_intersection_sphere(Sphere scene[], Vec3 origin, Vec3 transformed, float tMax, Sphere &closestSphere, float &closestT);

#endif //RAYTRACINGFROMSCRATCH_TRACE_PATH_H
