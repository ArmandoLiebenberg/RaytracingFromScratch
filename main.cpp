// Libraries
#include <SDL2/SDL.h>
#include <valarray>
#include <string>

// Renderer
#include "renderer_math.h"
#include "objects.h"

#define CANVAS_WIDTH 1920
#define CANVAS_HEIGHT 1080
#define VIEW_WIDTH 1.9
#define VIEW_HEIGHT 1.0
#define DISTANCE 1.0
#define OBJECTS 6

#define NUM_BOUNCES 2
#define NUM_SAMPLES 20

// FUNCTION DECLARATIONS ---------------------------------------------------
void draw_pixel(SDL_Renderer* renderer, int x, int y, int r, int g, int b);
Vec3 view_to_canvas(int canvas_x, int canvas_y);
Vec3i trace_ray(Vec3 origin, Vec3 transformed, float tMin, float tMax, Sphere scene[], Light lights[]);
Vec2 intersectRaySphere(Vec3 origin, Vec3 direction, Sphere sphere);
double ComputeDirectLighting(TracedSphere tracedSphere, Sphere scene[], Vec3 point, Vec3 normal, Vec3 view, Light lights[], int specular);
TracedSphere closest_intersection(TracedSphere tracedSphere, Sphere scene[], Vec3 origin, Vec3 transformed, float tMin, float tMax);
float trace_ray_path();
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
    Sphere greenCircle = {{-2,0.0,4}, 1, {0, 255,0}, 10};
    Sphere yellowCircle = {{0,-5001,3}, 5000, {255,255,0}, -1};
    Sphere scene[OBJECTS] = {redCircle, greenCircle, blueCircle, yellowCircle};

    Light ambient = {std::string {"ambient"}, 0, Vec3 {0,0,0}};
    Light point = {std::string {"point"}, 0.6, Vec3 {2,1,0}};
    Light directional = {std::string {"directional"}, 0.2, Vec3 {1,4,4}};
    Light lights[3] = {ambient, point, directional};

    // ---------- End Model Code --------------------

    // ---------- Graphics Code ------------------------

    // place the eye and the frame as desired
    Vec3 origin = {0, 0, 0};
    // for each square on the canvas
    for (int x = -CANVAS_WIDTH/2; x < CANVAS_WIDTH/2; x++) {
        for (int y = -CANVAS_HEIGHT/2; y < CANVAS_HEIGHT/2; y++) {
            // Determine which squares on the grid correspond to this square on the canvas
            Vec3 transformed = view_to_canvas(x, y);

            // Determine the color seen through that grid square
            Vec3i color = trace_ray(origin, transformed, 1, 1000, scene, lights);

            // Paint the square with that color
            draw_pixel(renderer, x, y, color.r, color.g, color.b);
        }
    }

    // Render The Stuff
    while (true) {
        SDL_RenderPresent(renderer);
        SDL_Delay(200);
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
float trace_ray_path(int bounces = 0) {
    float illumination = 0.0;

    if (bounces < NUM_BOUNCES) {
        // generate a set of points in a square [1,1] x [1,1]
        // map those points to a hemisphere
        // shoot rays out of that hemisphere
        // if it intersects with an object, add that object's emissivity and call trace_ray_path again
        // else return 0;
    }
    return illumination;
}