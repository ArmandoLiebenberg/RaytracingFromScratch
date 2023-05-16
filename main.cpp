#include <SDL2/SDL.h>
#include <valarray>
#include <string>

#define CANVAS_WIDTH 720
#define CANVAS_HEIGHT 680
#define VIEW_WIDTH 1.0
#define VIEW_HEIGHT 1.0
#define DISTANCE 1.0
#define OBJECTS 6

class Vec3 {
public:
    float x,y,z;

    Vec3 subtract(Vec3 b) {
        return Vec3 {x - b.x, y - b.y, z - b.z};
    }

    Vec3 add(Vec3 b) {
        return Vec3 {x + b.x, y + b.y, z + b.z};
    }

    Vec3 multiplyScalar(float a) {
        return Vec3 {x * a, y * a, z * a};
    }

    Vec3 flipped() {
        return Vec3 {-x, -y, -z};
    }

    float dot(Vec3 b) {
        return (x * b.x) + (y * b.y) + (z * b.z);
    }

    float length() {
        return sqrt(x*x + y*y + z*z);
    }
};

class Vec3i {
public:
    int r,g,b;

    Vec3i multiplyScalar(float a) {
        return Vec3i {static_cast<int>(r * a), static_cast<int>(g * a), static_cast<int>(b * a)};
    }
};

class Vec2 {
public:
    float t1, t2;
};

/* Sphere
 * ------------------------
 * A sphere object with coordinates and details about its shape and color
 */
typedef struct Sphere {
    Vec3 centre {};
    float radius {};
    Vec3i color {};
    int specular{};
} Sphere;

typedef struct Light {
    std::string type;
    float intensity;
    Vec3 direction;
} Light;

typedef struct Object {
    struct object;
    std::string objectType;
} Object;


// FUNCTION DECLARATIONS ---------------------------------------------------
void draw_pixel(SDL_Renderer* renderer, int x, int y, int r, int g, int b);
Vec3 view_to_canvas(int canvas_x, int canvas_y);
Vec3i trace_ray(Vec3 origin, Vec3 transformed, float tMin, float tMax, Sphere scene[], Light lights[]);
Vec2 intersectRaySphere(Vec3 origin, Vec3 direction, Sphere sphere);
double ComputeLighting(Vec3 point, Vec3 normal, Vec3 view, Light lights[], int specular);
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
    Sphere redCircle = {{0,-0.5,3}, 0.5, {255,0,0}, 500 };
    Sphere blueCircle = {{0,0.0,3}, 0.5, {0,0,255}, -1};
    Sphere greenCircle = {{0,0.5,3}, 0.5, {0, 255,0}, 10};
    Sphere yellowCircle = {{0,-5001,3}, 5000, {255,255,0}, -1};
    Sphere eye1 = {{0.2,0.5,2.5}, 0.1, {255,30,125}, 5000};
    Sphere eye2 = {{-0.2,0.5,2.5}, 0.1, {255,30,125}, 5000};
    Sphere scene[OBJECTS] = {redCircle, greenCircle, blueCircle, yellowCircle, eye1, eye2};

    Light ambient = {std::string {"ambient"}, 0.2, Vec3 {0,0,0}};
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

/* trace_ray()
 * -----------------------
 * Calculates ray outwards from camera through viewport
 */
Vec3i trace_ray(Vec3 origin, Vec3 transformed, float tMin, float tMax, Sphere scene[], Light lights[]) {
    float closestT = std::numeric_limits<float>::infinity();
    Sphere closestSphere = {{0,0,0}, 0, {0,0,0}};
    int found = 0;

    for (int i = 0; i < 6; i++) {
        Sphere sphere = scene[i];
        Vec2 tValues = intersectRaySphere(origin, transformed, sphere);

        if ((tValues.t1 < tMax) && (tMin < tValues.t1) && (tValues.t1 < closestT)) {
            closestT = tValues.t1;
            closestSphere = sphere;
            found = 1;
        }

        if ((tValues.t2 < tMax) && (tMin < tValues.t2) && (tValues.t2 < closestT)) {
            closestT = tValues.t2;
            closestSphere = sphere;
            found = 1;
        }
    }

    if (!found) {
        return Vec3i {255, 255, 255};
    }

    Vec3 point = origin.add(transformed.multiplyScalar(closestT));  // Compute intersection
    Vec3 normal = point.subtract(closestSphere.centre); // Compute sphere normal at intersection
    normal = normal.multiplyScalar(1/normal.length()); // unit normal

    return closestSphere.color.multiplyScalar(std::clamp(ComputeLighting(point, normal, transformed.flipped(), lights, closestSphere.specular), 0.0, 1.0));
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

double ComputeLighting(Vec3 point, Vec3 normal, Vec3 view, Light lights[], int specular) {
    double intensity = 0.0;

    for (int i = 0; i < 3; i++) {
        Light light = lights[i];

        if (light.type == "ambient") {
            intensity += light.intensity;
        } else {
            Vec3 L{};
            if (light.type == "point") {
                L = light.direction.subtract(point);
            }
             if (light.type == "directional") {
                L = light.direction;
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