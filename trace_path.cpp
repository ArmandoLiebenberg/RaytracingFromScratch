//
// Created by aliebs on 28/05/23.
//

#include "trace_path.h"
#include "objects.h"

/* trace_path()
 * ----------------------------------------
 * Shoot a ray from a point into the direction specified. If no objects are hit
 * return black, otherwise find the point on the closest object hit and
 * calculate the direct lighting. Sample vectors in the hemisphere of that
 * point and recursively call trace path, returning the sum of the emissivity
 * of that point and all lighting sampled from further calls of trace_path()
 *
 * @param Vec3 point
 * @param Vec3 direction
 * @param Object objects[]
 * @param Light lights[]
 * @return Vec3i color
 */
Vec3i trace_path(Vec3 point, Vec3 direction, Sphere objects[], Light lights[]) {

}

/* direct_lighting()
 * ----------------------------------------
 * Shoot a ray from the point to all lights in the scene. If no objects are
 * between the light and point, we calculate the light intensity reflected
 * off the point to the camera and return the cumulative sum of all of these
 * lights as the direct lighting
 *
 * @param Vec3 point
 * @param Object objects[]
 * @param Light lights[]
 * @return Vec3i Color
 */
Vec3i direct_lighting(Vec3 point, Sphere objects[], Light lights[]) {

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
Vec3 vector_hemisphere(Vec3 point, Vec3 normal){

}

/* local_coordinates()
 * ----------------------------------------
 * We build a vector containing the normal of our point, the tangent, and
 * the bi-tangent. We use this to transform vectors into our point's local
 * coordinates
 *
 * @param Vec3 normal
 * @return Vec3 localCoordinates
 */
Vec3 local_coordinates(Vec3 normal){

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
Vec3 sample_hemisphere(float &r1, float &r2){

}

/* intersect_ray_object()
 * ----------------------------------------
 * for a given point and a vector projecting from that point, we determine if
 * that vector intersects with any of our objects, and if so set the 't' and
 * closestObject variables to distance along the vector and the values.
 *
 * @param[in] Vec3 point
 * @param[in] Vec3 direction
 * @param[in] Object objectList[]
 * @param[out] uint32 t
 * @param[out] Object closestObject
 * @return bool
 */
bool intersect_ray_object(Vec3 point, Vec3 direction, Sphere objectList[], Sphere closestObject, uint32_t &t){

}

/* calculate_lighting()
 * ----------------------------------------
 * For a given point and a given light, we determine the intensity of the light
 * reflected off that point towards the camera
 *
 * @param Vec3 point
 * @param Vec3 direction
 * @param Light lights
 * @return float Intensity
 */
float calculate_lighting(Vec3 point, Vec3 direction, Light lights[], float intensity){

}

