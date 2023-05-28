//
// Created by aliebs on 23/05/23.
//

#include <cmath>
#include "renderer_math.h"

Vec3 Vec3::subtract(Vec3 b) const {
    return Vec3 {x - b.x, y - b.y, z - b.z};
}

Vec3 Vec3::add(Vec3 b) {
    return Vec3 {x + b.x, y + b.y, z + b.z};
}

Vec3 Vec3::multiplyScalar(float a) {
    return Vec3 {x * a, y * a, z * a};
}

Vec3 Vec3::flipped() {
    return Vec3 {-x, -y, -z};
}

Vec3 Vec3::normalize() {
    return Vec3 {x/length(), y/length(), z/length()};
}

Vec3 Vec3::cross(Vec3 b) {
    return Vec3 {y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x};
}

float Vec3::dot(Vec3 b) const {
    return (x * b.x) + (y * b.y) + (z * b.z);
}

float Vec3::length() {
    return std::sqrt(x*x + y*y + z*z);
}


Vec3i Vec3i::multiplyScalar(float a) {
    return Vec3i {static_cast<int>(r * a), static_cast<int>(g * a), static_cast<int>(b * a)};
}

Vec3i Vec3i::add(Vec3i k) {
    return Vec3i {r + k.r, g + k.g, b + k.b};
}