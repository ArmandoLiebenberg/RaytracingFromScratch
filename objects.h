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

class Triangle {
public:
    Vec3 v0 {};
    Vec3 v1 {};
    Vec3 v2 {};
    Vec3 vertices[3] = {v0, v1, v2};
    Vec3 normal = get_normal(vertices);

    Vec3 get_normal(Vec3 triangle_vertices[3]) {
        Vec3 A = v1.subtract(v0);
        Vec3 B = v2.subtract(v0);
        Vec3 C = A.cross(B);
        return C = C.normalize();
    }

    bool ray_triangle_intersection(Vec3 origin, Vec3 direction, float &t) {

        // check if the ray and the triangle are parallel
        if (fabs(normal.dot(direction)) < 0.0001) {
            return false;
        }

        // compute D parameter of plane
        float d = -normal.dot(v1);

        // compute t
        t = -(normal.dot(origin) + d);

        // check if triangle is behind ray
        if (t < 0) {
            return false;
        }

        // compute the intersection point
        Vec3 P = (origin.add(direction.multiplyScalar(t)));

        // inside outside test
        Vec3 C{};

        // edge 0
        Vec3 edge0 = v1.subtract(v0);
        Vec3 vp0 = P.subtract(v0);
        C = edge0.cross(vp0);
        if (normal.dot(C) < 0) {
            return false;
        }

        // edge 1
        Vec3 edge1 = v2.subtract(v1);
        Vec3 vp1 = P.subtract(v0);
        C = edge1.cross(vp1);
        if (normal.dot(C) < 0) {
            return false;
        }

        // edge 2
        Vec3 edge2 = v0.subtract(v2);
        Vec3 vp2 = P.subtract(v2);
        C = edge2.cross(vp2);
        if (normal.dot(C) < 0) {
            return false;
        }

        return true;

    }

};

class cube {
    // 0------1  4------5
    // |      |  |      |
    // |      |  |      |
    // 2------3  6------7
    // top edges first, then bottom
public:
    Vec3 edges[8];
    Triangle triangles[12];

    void populate_triangles() {
        edges[0] = {0,1,4};
        edges[0] = {1,1,4};
        edges[0] = {0,1,3};
        edges[0] = {1,1,3};
        edges[0] = {0,0,4};
        edges[0] = {1,0,4};
        edges[0] = {0,0,3};
        edges[0] = {1,0,3};

        // top
        triangles[0] = Triangle {edges[0], edges[2], edges[3]};
        triangles[0] = Triangle {edges[0], edges[3], edges[1]};
        // bottom
        triangles[0] = Triangle {edges[4], edges[6], edges[7]};
        triangles[0] = Triangle {edges[4], edges[7], edges[5]};
        // front
        triangles[0] = Triangle {edges[2], edges[6], edges[7]};
        triangles[0] = Triangle {edges[2], edges[7], edges[3]};
        // back
        triangles[0] = Triangle {edges[1], edges[5], edges[4]};
        triangles[0] = Triangle {edges[1], edges[4], edges[0]};
        // left
        triangles[0] = Triangle {edges[0], edges[4], edges[6]};
        triangles[0] = Triangle {edges[0], edges[6], edges[2]};
        // right
        triangles[0] = Triangle {edges[3], edges[7], edges[5]};
        triangles[0] = Triangle {edges[3], edges[5], edges[1]};
    }
};

#endif //RAYTRACINGFROMSCRATCH_OBJECTS_H
