//
// Created by aliebs on 28/05/23.
//

#include <algorithm>
#include <random>
#include "trace_path.h"
#include "objects.h"

#define LIGHTS 2
#define OBJECTS 4
#define TMIN 0.001
#define TMAX 1000

#define NUM_BOUNCES 2
#define NUM_SAMPLES 20

/* trace_path()
 * ----------------------------------------
 * Shoot a ray from a point into the direction specified. If no objects are hit
 * return black, otherwise find the point on the closest object hit and
 * calculate the direct lighting. Sample vectors in the hemisphere of that
 * point and recursively call trace path, returning the sum of the emissivity
 * of that point and all lighting sampled from further calls of trace_path()
 *
 * @param Vec3 origin
 * @param Vec3 direction
 * @param Object objects[]
 * @param Light lights[]
 * @return Vec3i color
 */
Vec3i trace_path(Vec3 origin, Vec3 direction, Sphere objects[], Light lights[], int depth) {
    Vec3i color = {0,0,0};
    Vec3i indirectDiffuse {0,0,0};

    // check if ray from origin in direction intersects with object, set the closest object
    Sphere closestObject;
    float closestT = std::numeric_limits<float>::infinity();
    if (!closest_intersection_sphere(objects, origin, direction, TMAX,  closestObject, closestT)) {
        // if no, return black
        return color;
    }

    // calculate the point hit and the unit normal from that point
    Vec3 point = origin.add(direction.multiplyScalar(closestT));  // Compute intersection
    Vec3 normal = point.subtract(closestObject.centre); // Compute sphere normal at intersection
    normal = normal.multiplyScalar(1/normal.length()); // unit normal

    // calculate direct lighting
    color = color.add(direct_lighting_sphere(origin, direction, closestObject, closestT, objects, lights));

    // check if max depth was reached
    if (depth >= NUM_BOUNCES) {
        //color = color.multiplyScalar(1.0/M_PI);
        return color;
    }

    // generate sample points on origin local coordinates hemisphere
    Vec3 normalTangent {};
    Vec3 normalBiTangent {};
    local_coordinates(normal, normalTangent, normalBiTangent);

    // Code taken from https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 generator(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<float> distribution(0.0, 1.0);

    float pdf = 1 / (2 * M_PI);

    // generate points in a hemisphere and transform to point local coordinates
    for (int i = 0; i < NUM_SAMPLES; i++) {
        float r1 = distribution(generator);
        float r2 = distribution(generator);
        Vec3 s = sample_hemisphere(r1, r2);
        Vec3 sample = {s.x * normalBiTangent.x + s.y * normal.x + s.z * normalTangent.x,
                  s.x * normalBiTangent.y + s.y * normal.y + s.z * normalTangent.y,
                  s.x * normalBiTangent.z + s.y * normal.z + s.z * normalTangent.z,};

        // recursively call trace_path and add to intensity
        Vec3i indirect_color = trace_path(point.add(sample.multiplyScalar(0.0001)), sample, objects, lights, depth+1);
        indirect_color = indirect_color.multiplyScalar(r1);
        indirectDiffuse = indirectDiffuse.add(indirect_color);

    }
    // divide by N and the constant PDF
    indirectDiffuse = indirectDiffuse.multiplyScalar(1.0/NUM_SAMPLES);

    // add indirect diffuse
    color = color.add(indirectDiffuse);

    // multiply by object albedo
    //color = color.multiplyScalar(0.18/M_PI);

    color = Vec3i {(std::clamp(color.r, 0, 255)), (std::clamp(color.g, 0, 255)), (std::clamp(color.b, 0, 255))};
    return color;
}

/* vector_hemisphere()
 * ----------------------------------------
 * We build a local coordinate system, generate two random points in [0,1]^2,
 * transform these points into a unit vector in normal cartesian plane
 * coordinates, and then transform this vector into local coordinates
 *
 * @param Vec3 point
 * @param vec3 normal
 * @return Vec3 sampledVector
 */
void vector_hemisphere(Vec3 (&sample)[NUM_SAMPLES], Vec3 point, Vec3 normal){

}

/* local_coordinates()
 * ----------------------------------------
 * We build the tangent, and the bi-tangent vectors of our point.
 * We use this to transform vectors into our point's local coordinates
 *
 * @param Vec3 normal
 * @param Vec3 normalTangent
 * @param Vec3 normalBiTangent
 */
void local_coordinates(Vec3 &normal, Vec3 &normalTangent, Vec3 &normalBiTangent) {
    if (std::fabs(normal.x) > std::fabs(normal.y)) {
        float div = sqrt(normal.x * normal.x + normal.z * normal.z);
        normalTangent = Vec3 {normal.z/div, 0, -normal.x/div};
    } else {
        float div = sqrt(normal.y * normal.y + normal.z * normal.z);
        normalTangent = Vec3 {0, -normal.z/div, normal.y/div};
    }
    normalBiTangent = normal.cross(normalTangent);
}

/* sample_hemisphere()
 * ----------------------------------------
 * We calculate a sample unit vector in the hemisphere of the standard
 * cartesian coordinates
 *
 * @param[out] Float r1
 * @param[out] Float r2
 * @return Vec3 randomSample
 */
Vec3 sample_hemisphere(const float &r1, const float &r2){
    Vec3 randomSample = {};
    float sinTheta = sqrt(1 - r1 * r1);
    float phi = 2 * M_PI * r2;

    randomSample.x = sinTheta * cos(phi);
    randomSample.y = r1;
    randomSample.z = sinTheta * sin(phi);

    return randomSample;
}

/* direct_lighting_sphere()
 * ----------------------------------------
 * Shoot a ray from the point to all lights in the scene. If no objects are
 * between the light and point, we calculate the light intensity reflected
 * off the point to the camera and return the cumulative sum of all of these
 * lights as the direct lighting
 *
 * @param Vec3 point
 * @param Vec3 transformed
 * @param Object closestSphere
 * @param float closestT
 * @param Sphere scene[]
 * @param Light lights[]
 * @return Vec3i Color
 */
Vec3i direct_lighting_sphere(Vec3 origin, Vec3 transformed, Sphere closestSphere, float closestT, Sphere scene[], Light lights[]) {

    Vec3 point = origin.add(transformed.multiplyScalar(closestT));  // Compute intersection
    Vec3 normal = point.subtract(closestSphere.centre); // Compute sphere normal at intersection
    normal = normal.multiplyScalar(1/normal.length()); // unit normal

    float illumination = std::clamp(compute_direct_lighting_sphere(scene, point, normal, transformed.flipped(), lights, closestSphere.specular), 0.0, 1.0);
    return closestSphere.color.multiplyScalar(illumination);
}

/* intersect_ray_sphere()
 * ----------------------
 * for a given point and a vector projecting from that point, we determine if
 * that vector intersects with a given sphere, and if so set the 't1', 't2'
 * variables to distance along the vector and the values.
 *
 * @param[in] Vec3 origin
 * @param[in] Vec3 direction
 * @param[in] Sphere sphere
 * @param[out] float t1
 * @param[out] float t2
 * @return bool
 */
bool intersect_ray_sphere(Vec3 origin, Vec3 direction, Sphere sphere, float &t1, float &t2) {
    float r = sphere.radius;
    Vec3 CO = origin.subtract(sphere.centre);

    float a = direction.dot(direction);
    float b = CO.dot(direction);
    b = 2 * b;
    float c = CO.dot(CO);
    c = c - (r*r);

    float discriminant = (b*b) - (4*a*c);
    if (discriminant < 0) {
        return false;
    }

    t1 = ((-b) + sqrt(discriminant)) / (2*a);
    t2 = ((-b) - sqrt(discriminant)) / (2*a);
    return true;
}

/* compute_direct_lighting_sphere()
 * -----------------------
 * Compute the amount of direct lighting coming from light sources to a given point on a sphere
 *
 * @param Sphere scene[]
 * @param Vec3 point
 * @param Vec3 normal
 * @param Vec3 view
 * @param Light lights[]
 * @param int specular
 * @return float intensity
 */

double compute_direct_lighting_sphere(Sphere scene[], Vec3 point, Vec3 normal, Vec3 view, Light lights[], int specular) {
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
            Sphere closestSphere {};
            float closestT = std::numeric_limits<float>::infinity();

            if (closest_intersection_sphere(scene, point, L, 1000, closestSphere, closestT)) {
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
 * Given a ray, check if it intersects with any spheres in the scene within a
 * given range along the ray. Set the object and the distance if so.
 *
 * @param[in] Sphere scene[]
 * @param[in] Vec3 origin
 * @param[in] Vec3 transformed
 * @param[in] float tMax
 * @param[out] Sphere closestSphere
 * @param[out] float closestT
 * @return bool
 */
bool closest_intersection_sphere(Sphere scene[], Vec3 origin, Vec3 transformed, float tMax, Sphere &closestSphere, float &closestT) {
    bool found = false;
    for (int i = 0; i < OBJECTS; i++) {
        Sphere sphere = scene[i];
        float t1, t2;
        if(!intersect_ray_sphere(origin, transformed, sphere, t1, t2)) {
            continue;
        }

        if ((t1 < tMax) && (TMIN < t1) && (t1 < closestT)) {
            closestT = t1;
            closestSphere = sphere;
            found = true;
        }

        if ((t2 < tMax) && (TMIN < t2) && (t2 < closestT)) {
            closestT = t2;
            closestSphere = sphere;
            found = true;
        }
    }
    return found;
}
