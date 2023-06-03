// Libraries
#include <SDL2/SDL.h>
#include <string>
#include <ctime>
#include <algorithm>

// Renderer
#include "trace_ray_simple.h"
#include "renderer_math.h"
#include "objects.h"
#include "trace_path.h"

#define CANVAS_WIDTH 100
#define CANVAS_HEIGHT 100
#define VIEW_WIDTH 1.0
#define VIEW_HEIGHT 1.0
#define DISTANCE 1.0

#define LIGHTS 3
#define OBJECTS 4

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
    Light directional = {std::string {"directional"}, 0.0, Vec3 {1,4,4}};
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
                Vec3i color = trace_path(origin, transformed, scene, lights, 0);

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