#include <SDL2/SDL.h>
#include<array>
#include <valarray>

#define CANVAS_WIDTH 640
#define CANVAS_HEIGHT 480
#define VIEW_WIDTH 1.0
#define VIEW_HEIGHT 1.0
#define DISTANCE 1

/* Sphere
 * ------------------------
 * A sphere object with coordinates and details about its shape and color
 */
struct Sphere {
    std::valarray<int> centre {};
    float radius {};
    std::valarray<int> colour {};
};

class Vec3 {
public:
    float x,y,z;
};

class Vec3i {
public:
    int r,g,b;
};

void draw_pixel(SDL_Renderer* renderer, int x, int y, int r, int g, int b);
Vec3 view_to_canvas(int canvas_x, int canvas_y);
void trace_ray();

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
    Sphere red_circle = {{0,-1,3}, 1, {255,0,0}};
    Sphere green_circle = {{2,0,4}, 1, {0, 255,0}};
    Sphere blue_circle = {{-2,0,4}, 1, {0,0,255}};


    std::array<Sphere,3> Scene{red_circle, green_circle, blue_circle};
    // ---------- End Model Code --------------------

    // ---------- Graphics Code ------------------------

    // place the eye and the frame as desired
    Vec3 origin = {0, 0, 0};
    // for each square on the canvas
    for (int x = 0; x < CANVAS_WIDTH; x++) {
        for (int y = 0; y < CANVAS_HEIGHT; y++) {
            // Determine which squares on the grid correspond to this square on the canvas
            Vec3 Transformed = view_to_canvas(x, y);

            // Determine the color seen through that grid square
            Vec3i color = {200, 183, 30};

            // Paint the square with that color
            draw_pixel(renderer, x, y, color.r, color.g, color.b);
        }
    }

    // ---------- Graphics Code --------------------
    // ----------End Graphics Code --------------------

    // Render The Stuff
    while (true) {
        SDL_RenderPresent(renderer);
        SDL_Delay(13);
    }

}

/* draw_pixel()
 * ----------------------
 * Draws a pixel on the screen using SDL tools
 */
void draw_pixel(SDL_Renderer* renderer, int x, int y, int r, int g, int b) {
    SDL_SetRenderDrawColor(renderer,r,g,b,255);
    SDL_RenderDrawPoint(renderer, x, y);
}

/* view_to_canvas()
 * ----------------------
 * Takes a pixel coordinate on the canvas and returns in viewport coordinates (0,1)
 */
Vec3 view_to_canvas(int canvas_x, int canvas_y) {
    Vec3 f_array{0, 0,0};
    f_array.x = static_cast<float> (canvas_x * CANVAS_WIDTH/VIEW_WIDTH);
    f_array.y = static_cast<float> (canvas_y * CANVAS_HEIGHT/VIEW_HEIGHT);
    f_array.z = DISTANCE;
    return f_array;
}

/* trace_ray()
 * -----------------------
 * Calculates ray outwards from camera through viewport
 */
void trace_ray() {

}




