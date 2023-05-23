//
// Created by aliebs on 23/05/23.
//

#include <algorithm>
#include <cmath>
#include "trace_ray_simple.h"

#define OBJECTS 6

/* trace_ray()
 * -----------------------
 * Calculates ray outwards from camera through viewport
 */
Vec3i trace_ray(Vec3 origin, Vec3 transformed, float tMin, float tMax, Sphere scene[], Light lights[]) {
    TracedSphere tracedSphere = {Sphere {{0,0,0}, 0, {0,0,0}},
                                 std::numeric_limits<float>::infinity(), 0};
    tracedSphere = closest_intersection(tracedSphere, scene, origin, transformed, tMin, tMax);

    if (!tracedSphere.found) {
        return Vec3i {0, 0, 0};
    }
    Vec3 point = origin.add(transformed.multiplyScalar(tracedSphere.closestT));  // Compute intersection
    Vec3 normal = point.subtract(tracedSphere.closestSphere.centre); // Compute sphere normal at intersection
    normal = normal.multiplyScalar(1/normal.length()); // unit normal

    float illumination = std::clamp(ComputeDirectLighting(tracedSphere, scene, point, normal, transformed.flipped(), lights, tracedSphere.closestSphere.specular), 0.0, 1.0);
    return tracedSphere.closestSphere.color.multiplyScalar(illumination);
}

/* intersectRaySphere()
 * ----------------------
 * Given a ray from a point with a given direction, check to see if it intersects with a given
 * sphere object
 */
Vec2 intersectRaySphere(Vec3 origin, Vec3 direction, Sphere sphere) {
    float r = sphere.radius;
    Vec3 CO = origin.subtract(sphere.centre);

    float a = direction.dot(direction);
    float b = CO.dot(direction);
    b = 2 * b;
    float c = CO.dot(CO);
    c = c - (r*r);

    float discriminant = (b*b) - (4*a*c);
    if (discriminant < 0) {
        return Vec2 {std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()};
    }

    float t1 = ((-b) + sqrt(discriminant)) / (2*a);
    float t2 = ((-b) - sqrt(discriminant)) / (2*a);
    return Vec2 {t1, t2};
}
/* ComputeDirectLighting()
 * -----------------------
 * Compute the amount of direct lighting coming from light sources to a given point on a sphere
 */
double ComputeDirectLighting(TracedSphere tracedSphere, Sphere scene[], Vec3 point, Vec3 normal, Vec3 view, Light lights[], int specular) {
    double intensity = 0.0;

    for (int i = 0; i < 3; i++) {
        Light light = lights[i];

        if (light.type == "ambient") {
            intensity += light.intensity;
        } else {
            Vec3 L{};
            float tMax = 0;
            if (light.type == "point") {
                L = light.direction.subtract(point);
                tMax = 1;
            }
            if (light.type == "directional") {
                L = light.direction;
                tMax = std::numeric_limits<float>::infinity();
            }

            // Shadow check
            TracedSphere shadow = closest_intersection(TracedSphere {.found = 0}, scene, point, L, 0.001, tMax);
            if (shadow.found) {
                continue;
            }

            // diffuse lighting
            float nDotL = normal.dot(L);
            if (nDotL > 0) {
                intensity += light.intensity * nDotL/(normal.length() * L.length());
            }

            // specular lighting
            if (specular != -1) {
                Vec3 R = ((normal.multiplyScalar(2)).multiplyScalar(normal.dot(L))).subtract(L);
                float rDotV = R.dot(view);
                if (rDotV > 0) {
                    intensity += light.intensity * pow(rDotV / (R.length() * view.length()), specular);
                }
            }
        }
    }
    return intensity;
}
/* closest_intersection()
 * -----------------------
 * Given a ray, check if it intersects with any spheres in the scene, and return the closest one
 */
TracedSphere closest_intersection(TracedSphere tracedSphere, Sphere scene[], Vec3 origin, Vec3 transformed, float tMin, float tMax) {
    for (int i = 0; i < OBJECTS; i++) {
        Sphere sphere = scene[i];
        Vec2 tValues = intersectRaySphere(origin, transformed, sphere);

        if ((tValues.t1 < tMax) && (tMin < tValues.t1) && (tValues.t1 < tracedSphere.closestT)) {
            tracedSphere.closestT = tValues.t1;
            tracedSphere.closestSphere = sphere;
            tracedSphere.found = 1;
        }

        if ((tValues.t2 < tMax) && (tMin < tValues.t2) && (tValues.t2 < tracedSphere.closestT)) {
            tracedSphere.closestT = tValues.t2;
            tracedSphere.closestSphere = sphere;
            tracedSphere.found = 1;
        }
    }
    return tracedSphere;
}
