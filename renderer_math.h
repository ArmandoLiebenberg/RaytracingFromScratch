//
// Created by aliebs on 23/05/23.
//

#ifndef RAYTRACINGFROMSCRATCH_RENDERER_MATH_H
#define RAYTRACINGFROMSCRATCH_RENDERER_MATH_H

class Vec3 {
public:
    float x,y,z;
    Vec3 subtract(Vec3 b) const;
    Vec3 add(Vec3 b);
    Vec3 multiplyScalar(float a);
    Vec3 flipped();
    Vec3 cross(Vec3 b);
    Vec3 normalize();
    float dot(Vec3 b) const;
    float length();
};

class Vec3i {
public:
    int r,g,b;
    Vec3i multiplyScalar(float a);
    Vec3i add(Vec3i k);
    };

class Vec2 {
public:
    float t1, t2;
};

#endif //RAYTRACINGFROMSCRATCH_RENDERER_MATH_H

