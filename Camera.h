#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//-----------------------------------------------------------------------
// This class defines a 2D camera, which uses OpenGL to define a
// rectangular window for drawing.  It uses GLFW to handle window events
// Author: Alejo Hausner
// Date: Sept. 2017
//-----------------------------------------------------------------------

class Camera {
private:
    // Bottom-left corner of world rectangle
    double _world_BLx, _world_BLy;
    // Dimensions of world rectangle
    double _world_W,   _world_H;
    // Dimensions of window rectangle (pixels)
    double _window_W,  _window_H;
    // Dimensions of window rectangle (screen units)
    double _fb_W,      _fb_H;
    // GLFW handle for current drawing window
    GLFWwindow* _window;

public:
    // Default constructor
    Camera();

    // Explicit constructor
    Camera(double world_BLx, double world_BLy,
           double world_W, double world_H,
           int window_W, int window_H,
           GLFWwindow* window);

    // Coordinate transformations: world-to-screen, and screen-to-world
    void viewport_to_world(int vx, int vy,
                           double& wx, double& wy);

    void world_to_viewport(double wx, double wy,
                           int& vx, int& vy);

    // Convenience method: mouse to world coords (flips y axis)
    void mouse_to_world(int mx, int my,  double &wx, double &wy);


    // Call this to update the camera, in case the
    // window has been resized;
    void check_resize();

    // Call this before you start any drawing commands
    void begin_drawing();

    // Draw coordinate lines covering the world rectangle
    void draw_grid(double minor_spacing, double major_spacing);

    // Query width and height
    int get_win_W();
    int get_win_H();
};

#endif
