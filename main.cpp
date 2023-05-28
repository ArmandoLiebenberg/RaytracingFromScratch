// Libraries
#include <SDL2/SDL.h>
#include <string>
#include <ctime>
#include <algorithm>

// Renderer
#include "trace_ray_simple.h"
#include "renderer_math.h"
#include "objects.h"

#define CANVAS_WIDTH 600
#define CANVAS_HEIGHT 600
#define VIEW_WIDTH 1.0
#define VIEW_HEIGHT 1.0
#define DISTANCE 1.0

#define LIGHTS 3
#define OBJECTS 4

#define NUM_BOUNCES 2
#define NUM_SAMPLES 20

// FUNCTION DECLARATIONS ---------------------------------------------------
void draw_pixel(SDL_Renderer* renderer, int x, int y, int r, int g, int b);
Vec3 view_to_canvas(int canvas_x, int canvas_y);

float trace_ray_path(Vec3 origin, Vec3 direction, int bounces);
double compute_direct_lighting_triangle(Cube scene[], Vec3 point, Vec3 normal, Vec3 view, Light lights[]);
bool closest_intersection_triangle(Cube scene[], Vec3 origin, Vec3 direction, float tMin, float tMax);
bool shoot_primary_ray(const Vec3& origin, const Vec3& direction, Cube* scene[], uint32_t &objectIndex, uint32_t &triangleIndex, float &tNearest);
Vec3i trace(Vec3 origin, Vec3 direction, Cube* scene[],  int bounces);

Vec3i render(Vec3 origin, Vec3 transformed, float tMin, float tMax, Sphere scene[], Light lights[]);
// -------------------------------------------------------------------------

int main() {

    // Create our SDL render objects
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    // Initialise Window
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(CANVAS_WIDTH, CANVAS_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL, &window, &renderer);
    SDL_Delay(100); // Conflict between SDL and KDE window manager, delay band-aid fix to resolve

    // Draw and clear the canvas
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);

    // ---------- Model Code ------------------------

    Sphere redCircle = {{0,-0.5,3}, 1, {255,0,0}, 500 };
    Sphere blueCircle = {{2,0.0,4}, 1, {0,0,255}, -1};
    Sphere greenCircle = {{-2,0.0,4}, 1, {0,0,255}, -1};
    Sphere yellowCircle = {{0,-5001,3}, 5000, {255,255,0}, -1};
    Sphere scene[OBJECTS] = {redCircle, greenCircle, blueCircle, yellowCircle};

    Light ambient = {std::string {"ambient"}, 0, Vec3 {0,0,0}};
    Light point = {std::string {"point"}, 0.6, Vec3 {2,1,0}};
    Light directional = {std::string {"directional"}, 0.2, Vec3 {1,4,4}};
    Light lights[LIGHTS] = {ambient, point, directional};

    Triangle whiteTriangle = {Vec3 {-1, 0, 3}, Vec3 {1, 0, 3}, Vec3 {0, 1, 3}};
    Triangle scene1[1] = {whiteTriangle};
    //Cube whiteCube = {};
    //Cube* scene[OBJECTS] = {&whiteCube};

    // ---------- End Model Code --------------------

    // ---------- Graphics Code ------------------------

    // place the eye and the frame as desired

    while(true) {
        clock_t timeStart = clock();

        Vec3 origin = {0, 0, 0};
        // for each square on the canvas
        for (int x = -CANVAS_WIDTH / 2; x < CANVAS_WIDTH / 2; x++) {
            for (int y = -CANVAS_HEIGHT / 2; y < CANVAS_HEIGHT / 2; y++) {
                // Determine which squares on the grid correspond to this square on the canvas
                Vec3 transformed = view_to_canvas(x, y);

                // Determine the color seen through that grid square
                Vec3i color = trace_ray_sphere(origin, transformed, 1, 1000, scene, lights);

                // Paint the square with that color
                draw_pixel(renderer, x, y, color.r, color.g, color.b);
            }
        }
        clock_t timeEnd = clock();
        printf("Render time: %04.2f\n", (float)(timeEnd - timeStart) / CLOCKS_PER_SEC);
        SDL_RenderPresent(renderer);
    }
}

/* draw_pixel()
 * ----------------------
 * Draws a pixel on the screen using SDL tools
 */
void draw_pixel(SDL_Renderer* renderer, int x, int y, int r, int g, int b) {
    x = CANVAS_WIDTH/2 + x;
    y = CANVAS_HEIGHT/2 - y - 1;

    if (x < 0 || x >= CANVAS_WIDTH || y < 0 || y >= CANVAS_HEIGHT) {
        return;
    }
    SDL_SetRenderDrawColor(renderer,r,g,b,255);
    SDL_RenderDrawPoint(renderer, x, y);
}

/* view_to_canvas()
 * ----------------------
 * Takes a pixel coordinate on the canvas and returns in viewport coordinates (0,1)
 */
Vec3 view_to_canvas(int canvas_x, int canvas_y) {
    Vec3 f_array{0, 0,0};
    f_array.x = (canvas_x * VIEW_WIDTH/CANVAS_WIDTH);
    f_array.y = (canvas_y * VIEW_HEIGHT/CANVAS_HEIGHT);
    f_array.z = DISTANCE;
    return f_array;
}
























/* trace_ray_path()
 * -----------------------
 * Calculates ray outwards from camera through viewport
 */
Vec3i trace(Vec3 origin, Vec3 direction, Cube* scene[],  int bounces = 0) {
    Vec3i color = {0,0,0};
    uint32_t objectIndex;
    uint32_t triangleIndex;
    float tNearest;

    if (shoot_primary_ray(origin, direction, scene, objectIndex, triangleIndex, tNearest)) {
        color.add(scene[objectIndex]->triangles[triangleIndex].color);
    } else {
        return Vec3i {0, 0, 0}; // return black if doesn't intersect object
    }

    //if (bounces < NUM_BOUNCES) {
        // generate a set of points
        // map those points to a hemisphere
        // map the hemisphere to the world coordinates
        // shoot rays out of that hemisphere
        // if it intersects with an object, add that object's emissivity and call trace_ray_path again, use the formula for diffuse materials
        // else return 0;
    //}
    return color;
}

bool shoot_primary_ray(const Vec3& origin, const Vec3& direction, Cube* scene[], uint32_t &objectIndex, uint32_t &triangleIndex, float &tNearest) {
    tNearest = std::numeric_limits<float>::infinity();  //initialize to infinity
    bool intersect = false;
    for (uint32_t k = 0; k < OBJECTS; ++k) {
        for (uint32_t n = 0; n < 12; ++n) {
            float t;
            if ((scene[k]->triangles[n]).ray_triangle_intersection(origin, direction, &t) && t < tNearest) {
                objectIndex = k;
                triangleIndex = n;
                tNearest = t;  //update tNearest with the distance to the closest intersect point found so far
                intersect |= true;
            }
        }
    }

    return intersect;
}

double compute_direct_lighting_triangle(Cube scene[], Vec3 point, Vec3 normal, Vec3 view, Light lights[]) {
    double intensity = 0.0;

    for (int i = 0; i < LIGHTS; i++) {
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
            if (closest_intersection_triangle(scene, point, L, 0.001, tMax)) {
                continue;
            }

            // diffuse lighting
            float nDotL = normal.dot(L);
            if (nDotL > 0) {
                intensity += light.intensity * nDotL/(normal.length() * L.length());
            }
        }
    }
    return intensity;
}

bool closest_intersection_triangle(Cube scene[], Vec3 origin, Vec3 direction, float tMin, float tMax) {
    for (int i = 0; i < OBJECTS; i++) {
        for (int j = 0; i < 12; i++) {
            Triangle triangle = (scene[i]).triangles[j];

            float t;
            if (triangle.ray_triangle_intersection( origin, direction, &t)) {
                return true;
            }
        }
    }
    return false;
}