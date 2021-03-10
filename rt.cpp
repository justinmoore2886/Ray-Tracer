////////////////////////////////////////////////////
//
// A recursive ray tracer.
//
//////////////////////////////////////////////////////

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <map>

#include "Camera.h"
#include "KBUI.h"

#include "GeomLib.h"
#include "Color.h"
#include "Object.h"
#include "Triangle.h"
#include "Sphere.h"
#include "Hit.h"
#include "HitList.h"
#include "Tokenizer.h"
#include "Light.h"
#include "Material.h"

using namespace std;

KBUI the_ui;
Camera cam;

/////////////////////////////////////////////////////
// DECLARATIONS
////////////////////////////////////////////////////

// Forward declarations for functions in this file
void init_UI();
void setup_camera();
void check_for_resize();
Ray4 get_ray(int xDCS, int yDCS);
bool first_hit(Ray4 &ray, Hit& hit);
Color ray_color(Ray4& ray, int depth);
Vector4 mirror_direction(Vector4& L, Vector4& N);
bool refract(Vector4& L, Vector4& N, float n_in, float n_trans, Vector4& T);
Color local_illumination(Vector4& V, Vector4& N, Vector4& L, Material& mat, Color& ls);
void read_scene(const char *sceneFile);
void render();
void camera_changed();
void cam_param_changed(float);
bool get_was_window_resized();
void reset_camera(float);
void init_scene();
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods );
static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void display ();
int main(int argc, char *argv[]);

typedef unsigned char byte; // In case compiler doesn't define "byte" type

// USEFUL Flag:
// When the user clicks on a pixel, the mouse_button_callback does two things:
//   1. sets this flag.
//   2. calls ray_color() on that pixel
// This lets you check all your intersection code, for ONE ray of your choosing.
bool debugOn = false;

// FRAME BUFFER Declarations.
// The initial image is 300 x 300 x 3 bytes (3 bytes per pixel)
int winWidth  = 300;
int winHeight = 300;
byte *img = NULL;   // image is allocated by check_for_resize(), not here.

// These are the camera parameters.
// The camera position and orientation:
Point4  eye;
Point4  lookat;
Vector4 vup;

// The camera's HOME parameters, used in reset_camera()
Point4  eye_home;
Point4  lookat_home;
Vector4 vup_home;

// The clipping frustum
float clipL = -1;
float clipR = +1;
float clipB = -1;
float clipT = +1;
float clipN =  2;

// The camera's HOME frustum, also used in reset_camera()
float clip_home[5] = {-1, +1, -1, +1, 2};

vector<Object*> scene_objects; // list of objects in the scene
vector<Light> scene_lights; // list of lights in the scene
map<string, Material> materials_by_name; // named materials
Color ambient_light; // indirect light that shines when all lights are blocked
float ambient_fraction; // how much of lights is ambient

Matrix4 Mvcswcs;  // the inverse of the view matrix.
HitList hits;     // list of hit records for current ray
Hit* hitPool;

// Used to trigger render() when camera has changed.
bool frame_buffer_stale = true;

// Rays which miss all objects have this color.
const Color background_color(0.3, 0.4, 0.4); // dark blue

const int max_recursion_depth = 4;

//////////////////////////////////////////////////////////////////////
// Compute Mvcstowcs.
// YOU MUST IMPLEMENT THIS FUNCTION.
//////////////////////////////////////////////////////////////////////
void setup_camera() {
    Mvcswcs = Matrix4::Identity();

    // The camera's basis vectors
    Vector4 z = (eye - lookat).normalized();
    Vector4 x = (vup^z).normalized();
    Vector4 y = z^x;

    // The inverse of the view matrix
    Mvcswcs.set(x.X(), y.X(), z.X(), eye.X(),
                x.Y(), y.Y(), z.Y(), eye.Y(),
                x.Z(), y.Z(), z.Z(), eye.Z(),
                0.0,   0.0,   0.0,      1.0);

    check_for_resize();
}

/////////////////////////////////////////////////////////
//
// Get glossy color of a ray
//
/////////////////////////////////////////////////////////
Color glossy_color(Ray4& ray, Hit& hit, Object* obj) {
    Color color = Color(0, 0, 0);

    Vector4 negatedV = -ray.direction;

    for(int i = 0; i < scene_lights.size(); i++) {
        Light light = scene_lights[i];
        Vector4 lightDirection = (light.getPos() - hit.hitPoint()).normalized();

        color += local_illumination(negatedV, hit.normal(), lightDirection, obj->getMaterial(), light.getColor());
    }
    color += obj->getMaterial().getAmbient() * ambient_light;
    return color;
}

/////////////////////////////////////////////////////////
// Get color of a ray passing through (x,y)DCS. - TODO
// Find the first object hit.
//
// When a ray hits a surface and refracts, there are two indexes of
// refraction: n_i and n_t.  Which one is which?  I suggest that you use N
// dot L, to decide if the ray is entering or exiting the material.
/////////////////////////////////////////////////////////
Color ray_color(Ray4& ray, int depth) {
    Hit hit;

    if(first_hit(ray, hit)) {

        if(hit.getObject()->getMaterial().getType() == PHONG)
            return glossy_color(ray, hit, hit.getObject());

        else {

            if(depth >= max_recursion_depth)
                return background_color;

            Vector4 R = mirror_direction(ray.direction, hit.normal());

            // Reflection
            float n_i = 0;
            float n_t = 0;

            // If the ray is entering
            if(hit.normal() * ray.direction < 0) {
                n_i = 1;
                n_t = hit.getObject()->material.refraction_index;
            }

            // If the ray is exiting
            else {
                n_t = 1;
                n_i = hit.getObject()->material.refraction_index;
            }

            Vector4 T;
            if(!refract(ray.direction, hit.normal(), n_i, n_t, T))
                return hit.getObject()->getMaterial().color;

            Ray4 rayR = Ray4(hit.hitPoint(), R);
            Ray4 rayT = Ray4(hit.hitPoint(), T);

            Color colorR = ray_color(rayR, depth + 1);
            Color colorT = ray_color(rayT, depth + 1);

            Material hitMaterial = hit.getObject()->getMaterial();
            return colorR * hitMaterial.getReflection() + colorT * hitMaterial.getTransmission() + hitMaterial.color;
        }
    }
    else
        return background_color;
}


//////////////////////////////////////////////////////////////////////
// If window size has changed, re-allocate the frame buffer
//////////////////////////////////////////////////////////////////////
void check_for_resize() {
    // Now, check if the frame buffer needs to be created,
    // or re-created.

    bool should_allocate = false;
    if (img == NULL) {
        // frame buffer not yet allocated.
        should_allocate = true;
    }
    else if (winWidth  != cam.get_win_W() ||
             winHeight != cam.get_win_H()) {

        // frame buffer allocated, but has changed size.
        delete[] img;
        should_allocate = true;
        winWidth  = cam.get_win_W();
        winHeight = cam.get_win_H();
    }

    if (should_allocate) {

        if (debugOn) {
            cout << "ALLOCATING: (W H)=(" << winWidth
                 << " " << winHeight << ")\n";
        }

        img = new byte[winWidth * winHeight * 3];
        camera_changed();
    }
}

/////////////////////////////////////////////////////////
// Initialize a ray starting at (x y)DCS.
// YOU MUST IMPLEMENT THIS FUNCTION.
/////////////////////////////////////////////////////////
Ray4 get_ray(int xDCS, int yDCS) {
    float dx = (clipR - clipL) / winWidth;
    float xVcs = clipL + (xDCS + 0.5) * dx;

    float dy = (clipT - clipB) / winHeight;
    float yVcs = clipB + (yDCS + 0.5) * dy;

    float zVcs = -clipN;
    Point4 pVcs = Point4(xVcs, yVcs, zVcs);

    Float4 F_wcs = Mvcswcs * pVcs;
    Point4 pWcs(F_wcs.X(), F_wcs.Y(), F_wcs.Z());

    Vector4 V = pWcs - eye;
    V = V.normalized();

    Ray4 ray = Ray4(pWcs, V);
    return ray;
}

/////////////////////////////////////////////////////////
// Find the first object hit by the ray, if any
// YOU MUST IMPLEMENT THIS FUNCTION.
/////////////////////////////////////////////////////////
bool first_hit(Ray4 &ray, Hit& hit) {
    hits.clear();

    for(int i = 0; i < scene_objects.size(); i++) {
        Object* currentObject = scene_objects[i];
        Hit theIntersectingHit;

        if(currentObject->intersects(ray, theIntersectingHit))
            hits.add(theIntersectingHit);
    }

    bool isThereMinHit = false;
    if(!hits.isEmpty()) {
        hit = hits.getMin();
        isThereMinHit = true;
    }
    return isThereMinHit;
}

//////////////////////////////////////////////////////
// A ray hits a mirror.
// Return the direction of the reflected ray.
//////////////////////////////////////////////////////
Vector4 mirror_direction(Vector4& V, Vector4& N) {
    float NV = N * V;
    return V - (2.0f * NV) * N;
}

//////////////////////////////////////////////////////
// A ray hits a specular surface.
// Compute the direction of the refracted ray. - TODO
// Given an incident ray’s direction, the surface’s normal, and the two indexes of refraction, set the refracted ray.
// If there is total internal refraction, return false.
// YOU MUST IMPLEMENT THIS FUNCTION.
//////////////////////////////////////////////////////
bool refract(Vector4& V, Vector4& N, float n_i, float n_t, Vector4& direction) {
    Vector4 newN = N;
    if(V*N > 0) newN = -N;

    float alpha = -V * newN;
    float beta = sqrtf(1 - alpha * alpha);
    float Y = (n_i / n_t) * beta;

    if(1 - Y * Y < 0)
        return false;

    float delta = sqrtf(1 - Y * Y);
    Vector4 A = V + alpha * newN;
    Vector4 C = -delta * newN;
    Vector4 D = (n_i / n_t) * A;

    direction = C + D;
    direction = direction.normalized();
    return true;
}


//////////////////////////////////////////////////////
// A ray hits a phong surface.
// Compute the color of the surface.
// YOU MUST IMPLEMENT THIS FUNCTION.
//////////////////////////////////////////////////////
Color local_illumination(Vector4& V, Vector4& N, Vector4& L, Material& mat, Color& ls) {
    float NL = N.dot(L);
    if (NL > 0) {
        // light on up side of surface.

        Vector4 R = mirror_direction(L, N);
        float RV = R.dot(V);
        if (RV < 0) {
            // On back side of specular blob, use 0 instead.
            RV = 0;
        }

        //
        // Fast (R.V)^n
        // Avoid pow().  It's SLOOOOWW!
        //
        float RVn = 1.0f;


        if (RV <= 0.0f)
            RVn = 0.0f;
        else {
            int n = mat.getShininess();
            while (n > 0) {
                if (n & 1)
                    RVn *= RV;
                RV *= RV;
                n >>= 1;
            }
        }

        Color& kd = mat.getDiffuse();
        Color& ks = mat.getSpecular();

        return ls * (kd * NL + ks * RVn);
    }
    else
        return Color(0,0,0);  // light on back, no illumination.
}

/////////////////////////////////////////////////////////
// This function actually generates the ray-traced image.
// You don't have to change this function.
/////////////////////////////////////////////////////////
void render() {
    int x,y;
    byte r,g,b;
    int p;

    setup_camera();

    ambient_light = Color(0,0,0);
    for (auto light : scene_lights) {
        ambient_light += light.getColor() * ambient_fraction;
    }

    for (y=0; y<winHeight; y++) {
        for (x=0; x<winWidth; x++) {

            if (debugOn) {
                cout << "pixel (" << x << " " << y << ")\n";
                cout.flush();
            }

            Ray4 ray = get_ray(x, y);
            Color pixel_color = ray_color(ray, 0);
            pixel_color.clamp();

            r = (byte) (pixel_color.R() * 255.0);
            g = (byte) (pixel_color.G() * 255.0);
            b = (byte) (pixel_color.B() * 255.0);

            p = (y*winWidth + x) * 3;

            img[p]   = r;
            img[p+1] = g;
            img[p+2] = b;
        }
    }
}

//////////////////////////////////////////////////////
//
// Displays, on STDOUT, the colour of the pixel that
//  the user clicked on.
//
// THIS IS VERY USEFUL!  USE IT!
//
// You don't have to change this function.
//
//////////////////////////////////////////////////////
void mouse_button_callback( GLFWwindow* window, int button,
                            int action, int mods )
{
    if (button != GLFW_MOUSE_BUTTON_LEFT)
        return;

    if (action == GLFW_PRESS)
    {
        debugOn = true;

        // Get the mouse's position.

        double xpos, ypos;
        int W, H;
        glfwGetCursorPos(window, &xpos, &ypos);
        glfwGetWindowSize(window, &W, &H);

        // mouse position, as a fraction of the window dimensions
        // The y mouse coord increases as you move down,
        // but our yDCS increases as you move up.
        double mouse_fx = xpos / float(W);
        double mouse_fy = (W - 1 - ypos) / float(W);

        int xDCS = (int)(mouse_fx * winWidth + 0.5);
        int yDCS = (int)(mouse_fy * winHeight + 0.5);

        Ray4 ray = get_ray(xDCS, yDCS);
        Color pixelColor = ray_color(ray, 0);

        if (debugOn) {
            cout << "cursorpos:" << xpos << " " << ypos << "\n";
            cout << "Width Height: " << winWidth << " " << winHeight << "\n";
            cout << "Window Size: " << W << " " << H << "\n";
            cout << "Pixel at (x y)=(" << xDCS << " " << yDCS << ")\n";
            cout << "Pixel Color = " << pixelColor << endl;
        }
        debugOn = false;
    }
}

/////////////////////////////////////////////////////////
// Call this when scene has changed, and we need to re-run
// the ray tracer.
// You don't have to change this function.
/////////////////////////////////////////////////////////
void camera_changed() {
    float dummy=0;
    cam_param_changed(dummy);
}

/////////////////////////////////////////////////////////
// Called when user modifies a camera parameter.
// You don't have to change this function.
/////////////////////////////////////////////////////////
void cam_param_changed(float param) {
    frame_buffer_stale = true;
}

/////////////////////////////////////////////////////////
// Check if window was resized.
// You don't have to change this function.
/////////////////////////////////////////////////////////
bool get_was_window_resized() {
    int new_W = cam.get_win_W();
    int new_H = cam.get_win_H();

    if (new_W != winWidth || new_H != winHeight) {
        camera_changed();
        winWidth = new_W;
        winHeight = new_H;
        return true;
    }

    return false;
}

//////////////////////////////////////////////////////
// This function sets up a simple scene.
// YOU MUST IMPLEMENT THIS FUNCTION.
/////////////////////////////////////////////////////
void read_scene(const char *filename) {
    ambient_light.set(0,0,0);
    float r,g,b;
    float x,y,z;
    int nMaterials;
    int nLights;
    int nObjects;

    Tokenizer toker(filename);

    while (!toker.eof()) {
        string keyword = toker.next_string();

        // cout << "keyword:" << keyword << "\n";

        if (keyword == string("")) {
            continue; // skip blank lines
        }
        else if (keyword == string("#materials")) {
            nMaterials = toker.next_number();
            // Don't do anything (materials are now in a map)
        }
        else if (keyword == string("#lights")) {
            nLights = toker.next_number();
            scene_lights.reserve(nLights);
        }
        else if (keyword == string("#objects")) {
            nObjects = toker.next_number();
            scene_objects.reserve(nObjects);
            hitPool = new Hit[nObjects*2];
        }
        else if (keyword == string("light")) {
            Color c;
            Point4 p;

            string name = toker.next_string();

            toker.match("color");
            r = toker.next_number();
            g = toker.next_number();
            b = toker.next_number();
            c.set(r,g,b);

            ambient_light += c * ambient_fraction;

            toker.match("position");
            x = toker.next_number();
            y = toker.next_number();
            z = toker.next_number();
            p.set(x,y,z);

            Light l(c, p);
            l.name = name;

            scene_lights.push_back(l);
        }

        else if (keyword == string("camera_eye")) {
            double x,y,z;
            x = toker.next_number();
            y = toker.next_number();
            z = toker.next_number();
            eye = Point4(x,y,z);
            eye_home = eye;
        }
        else if (keyword == string("camera_lookat")) {
            double x,y,z;
            x = toker.next_number();
            y = toker.next_number();
            z = toker.next_number();
            lookat = Point4(x,y,z);
            lookat_home = lookat;
        }
        else if (keyword == string("camera_vup")) {
            double x,y,z;
            x = toker.next_number();
            y = toker.next_number();
            z = toker.next_number();
            vup = Vector4(x,y,z);
            vup_home = vup;
        }
        else if (keyword == string("camera_clip")) {
            clipL = toker.next_number();
            clipR = toker.next_number();
            clipB = toker.next_number();
            clipT = toker.next_number();
            clipN = toker.next_number();
            clip_home[0] = clipL;
            clip_home[1] = clipR;
            clip_home[2] = clipB;
            clip_home[3] = clipT;
            clip_home[4] = clipN;
        }

        /////////////////////////////////////////////////////
        // IMPLEMENT THE CODE BELOW
        // |    |    |    |    |
        // v    v    v    v    v

        else if (keyword == string("material")) {
            Material newMaterial;
            string name = toker.next_string();
            newMaterial.name = name;

            keyword = toker.next_string();
            string materialType = toker.next_string();

            if(materialType == "phong") {
                newMaterial.surface_type = PHONG;

                keyword = toker.next_string();
                r = toker.next_number();
                g = toker.next_number();
                b = toker.next_number();
                Color ambient = Color(r,g,b);

                keyword = toker.next_string();
                r = toker.next_number();
                g = toker.next_number();
                b = toker.next_number();
                Color diffuse = Color(r,g,b);

                keyword = toker.next_string();
                r = toker.next_number();
                g = toker.next_number();
                b = toker.next_number();
                Color specular = Color(r,g,b);

                keyword = toker.next_string();
                auto shininess = (int)toker.next_number();

                newMaterial.set(ambient, diffuse, specular, shininess);
                materials_by_name[name] = newMaterial;
            }
            else if(materialType == "specular") {
                newMaterial.surface_type = SPECULAR;

                keyword = toker.next_string();
                float index = toker.next_number();

                keyword = toker.next_string();
                r = toker.next_number();
                g = toker.next_number();
                b = toker.next_number();
                Color tau = Color(r,g,b);

                keyword = toker.next_string();
                r = toker.next_number();
                g = toker.next_number();
                b = toker.next_number();
                Color rho = Color(r,g,b);

                keyword = toker.next_string();
                r = toker.next_number();
                g = toker.next_number();
                b = toker.next_number();
                Color color = Color(r,g,b);

                newMaterial.set(index, tau, rho, color);
                materials_by_name[name] = newMaterial;
            }
            else {
                newMaterial.surface_type = NO_SURFACE;
                cerr << "Parse error: unrecognized material type \"" << materialType << "\"\n";
                exit(EXIT_FAILURE);
            }
        }
        else if (keyword == string("sphere")) {
            string sphereName = toker.next_string();

            keyword = toker.next_string();
            x = toker.next_number();
            y = toker.next_number();
            z = toker.next_number();
            Point4 center = Point4(x,y,z);

            keyword = toker.next_string();
            float radius = toker.next_number();

            keyword = toker.next_string();
            string materialName = toker.next_string();
            Material material;

            for (std::map<string,Material>::iterator it = materials_by_name.begin(); it != materials_by_name.end(); ++it) {
                if(it->first == materialName) {
                    material = it->second;
                    break;
                }
            }
            scene_objects.push_back(new Sphere(center, radius, material));
        }
        else if (keyword == string("triangle")) {
            string triangleName = toker.next_string();

            keyword = toker.next_string();
            x = toker.next_number();
            y = toker.next_number();
            z = toker.next_number();
            Point4 v1 = Point4(x,y,z);

            keyword = toker.next_string();
            x = toker.next_number();
            y = toker.next_number();
            z = toker.next_number();
            Point4 v2 = Point4(x,y,z);

            keyword = toker.next_string();
            x = toker.next_number();
            y = toker.next_number();
            z = toker.next_number();
            Point4 v3 = Point4(x,y,z);

            keyword = toker.next_string();
            string materialName = toker.next_string();
            Material material;

            for (std::map<string,Material>::iterator it = materials_by_name.begin(); it != materials_by_name.end(); ++it) {
               if(it->first == materialName) {
                   material = it->second;
                   break;
               }
            }
            scene_objects.push_back(new Triangle(v1,v2,v3,material));
        }
        else {
            cerr << "Parse error: unrecognized keyword \"" << keyword << "\"\n";
            exit(EXIT_FAILURE);
        }
    }
}

///////////////////////////////////////////////////
// Resets the camera parameters.
// You don't have to change this function.
///////////////////////////////////////////////////
void reset_camera(float dummy) {
    eye = eye_home;
    lookat = lookat_home;
    vup = vup_home;

    clipL = clip_home[0];
    clipR = clip_home[1];
    clipB = clip_home[2];
    clipT = clip_home[3];
    clipN = clip_home[4];

    camera_changed();
}


/////////////////////////////////////////////////////////
// Called on a GLFW error.
// You don't have to change this function.
/////////////////////////////////////////////////////////
static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

//////////////////////////////////////////////////////
// Quit if the user hits "q" or "ESC".
// All other key presses are passed to the UI.
// You don't have to change this function.
//////////////////////////////////////////////////////

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_Q ||
        key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    else if (action == GLFW_RELEASE) {
        the_ui.handle_key(key);
    }
}


//////////////////////////////////////////////////////
// Show the image.
// You don't have to change this function.
//////////////////////////////////////////////////////
void display () {
    glClearColor(.1f,.1f,.1f, 1.f);   /* set the background colour */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cam.begin_drawing();

    glRasterPos3d(0.0, 0.0, 0.0);

    glPixelStorei(GL_PACK_ALIGNMENT,1);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);

    if (frame_buffer_stale) {
        //
        // Don't re-render the scene EVERY time display() is called.
        // It might get called if the window is moved, or if it
        // is exposed.  Only re-render if the window is RESIZED.
        // Resizing triggers a call to handleReshape, which sets
        // frameBufferStale.
        //
        render();
        frame_buffer_stale = false;
    }

    //
    // This paints the current image buffer onto the screen.
    //
    glDrawPixels(winWidth,winHeight,
                 GL_RGB,GL_UNSIGNED_BYTE,img);

    glFlush();
}

///////////////////////////////////////////////////////////////////
// Set up the keyboard UI.
// No need to change this.
//////////////////////////////////////////////////////////////////
void init_UI() {
    // These variables will trigger a call-back when they are changed.
    the_ui.add_variable("Eye X", &eye.X(), -10, 10, 0.2, cam_param_changed);
    the_ui.add_variable("Eye Y", &eye.Y(), -10, 10, 0.2, cam_param_changed);
    the_ui.add_variable("Eye Z", &eye.Z(), -10, 10, 0.2, cam_param_changed);

    the_ui.add_variable("Ambient Fraction", &ambient_fraction, 0, 1, 0.1,
        cam_param_changed);

    the_ui.add_variable("Ref X", &lookat.X(), -10, 10, 0.2, cam_param_changed);
    the_ui.add_variable("Ref Y", &lookat.Y(), -10, 10, 0.2, cam_param_changed);
    the_ui.add_variable("Ref Z", &lookat.Z(), -10, 10, 0.2, cam_param_changed);

    the_ui.add_variable("Vup X", &vup.X(), -10, 10, 0.2, cam_param_changed);
    the_ui.add_variable("Vup Y", &vup.Y(), -10, 10, 0.2, cam_param_changed);
    the_ui.add_variable("Vup Z", &vup.Z(), -10, 10, 0.2, cam_param_changed);

    the_ui.add_variable("Clip L", &clipL,   -10, 10, 0.2, cam_param_changed);
    the_ui.add_variable("Clip R", &clipR,   -10, 10, 0.2, cam_param_changed);
    the_ui.add_variable("Clip B", &clipB,   -10, 10, 0.2, cam_param_changed);
    the_ui.add_variable("Clip T", &clipT,   -10, 10, 0.2, cam_param_changed);
    the_ui.add_variable("Clip N", &clipN,   -10, 10, 0.2, cam_param_changed);

    static float dummy2=0;
    the_ui.add_variable("Reset Camera", &dummy2,0,100, 0.001, reset_camera);

    the_ui.done_init();

}

void test_surface(Vector4& V, Vector4& N) {
    Vector4 R, T;

    R = mirror_direction(V, N);
    cout << "R:" << R << "\n";
    if (refract(V, N, 1.0, 1.3, T)) {
        cout << "T:" << T << "\n";
    }
    else {
        cout << "total internal reflection\n";
    }
}


//////////////////////////////////////////////////////
// Main program.
// You don't have to change this function.
//////////////////////////////////////////////////////
int main(int argc, char *argv[]) {

//
//    Vector4 V(0,0,-1);
//    Vector4 N(0,0,1);
//    test_surface(V, N);
//
//    V.set(0,0,-1);
//    N.set(0,0,-1);
//    test_surface(V, N);
//
//    V = Vector4(1,0,-1);
//    V = V.normalized();
//    N = Vector4(0,0,1);
//    test_surface(V, N);
//
//    V = Vector4(0.9999,0,0.0001);
//    V = V.normalized();
//    N = Vector4(0,0,1);
//    test_surface(V, N);
//
//    exit(0);

    init_UI();
    if (argc < 2) {
        cerr << "Usage:\n";
        cerr << "  rt <scene-file.txt>\n";
        exit(EXIT_FAILURE);
    }

    read_scene(argv[1]);

    GLFWwindow* window;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        cerr << "glfwInit failed!\n";
        cerr << "PRESS Control-C to quit\n";
        char line[100];
        cin >> line;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(winWidth, winHeight,
                              "Ray Traced Scene", NULL, NULL);

    if (!window)
    {
        cerr << "glfwCreateWindow failed!\n";
        cerr << "PRESS Control-C to quit\n";
        char line[100];
        cin >> line;

        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    int w = winWidth;
    int h = winHeight;

    cam = Camera(0,0, w,h, w, h, window);

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);

    float dummy=0;
    reset_camera(dummy);

    while (!glfwWindowShouldClose(window))
    {
        cam.check_resize();
        setup_camera();

        display();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
