//
// Created by aliebs on 23/05/23.
//

#ifndef RAYTRACINGFROMSCRATCH_TRACE_RAY_SIMPLE_H
#define RAYTRACINGFROMSCRATCH_TRACE_RAY_SIMPLE_H

#include "renderer_math.h"
#include "objects.h"

/* trace_ray()
 * -----------------------
 * Calculates ray outwards from camera through viewport
 */
Vec3i trace_ray(Vec3 origin, Vec3 transformed, float tMin, float tMax, Sphere scene[], Light lights[]);

/* intersectRaySphere()
 * ----------------------
 * Given a ray from a point with a given direction, check to see if it intersects with a given
 * sphere object
 */
Vec2 intersectRaySphere(Vec3 origin, Vec3 direction, Sphere sphere);

/* ComputeDirectLighting()
 * -----------------------
 * Compute the amount of direct lighting coming from light sources to a given point on a sphere
 */
double ComputeDirectLighting(TracedSphere tracedSphere, Sphere scene[], Vec3 point, Vec3 normal, Vec3 view, Light lights[], int specular);

/* closest_intersection()
 * -----------------------
 * Given a ray, check if it intersects with any spheres in the scene, and return the closest one
 */
TracedSphere closest_intersection(TracedSphere tracedSphere, Sphere scene[], Vec3 origin, Vec3 transformed, float tMin, float tMax);

#endif //RAYTRACINGFROMSCRATCH_TRACE_RAY_SIMPLE_H
