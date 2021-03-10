#include "Camera.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

Camera::Camera()
{
    _world_BLx = 0;
    _world_BLy = 0;
    _world_W = 1;
    _world_H = 1;
    _window_W = 100;
    _window_H = 100;
    _fb_W = 100;
    _fb_H = 100;
    _window = NULL;
}

Camera::Camera(double world_BLx, double world_BLy,
                   double world_W, double world_H,
               int window_W, int window_H,
               GLFWwindow* window)
{
    _world_BLx = world_BLx;
    _world_BLy = world_BLy;
    _world_W = world_W;
    _world_H = world_H;
    _window_W = window_W;
    _window_H = window_H;
    _fb_W = window_W;
    _fb_H = window_H;
    _window = window;
}

void Camera::mouse_to_world(int mx, int my,  double &wx, double &wy)
{
    int vx = mx;
    int vy = (int)(_window_H - my);

    viewport_to_world(vx, vy, wx, wy);
}

void Camera::viewport_to_world(int vx, int vy,  double &wx, double &wy)
{
    double x_fraction = vx / _window_W;
    double y_fraction = vy / _window_H;

//    cout << "fractions: " << x_fraction << " " << y_fraction << "\n";

    wx = _world_BLx + x_fraction * _world_W;
    wy = _world_BLy + y_fraction * _world_H;
}

void Camera::world_to_viewport(double wx, double wy,
                               int& vx, int& vy) {
    vx = (int)((wx - _world_BLx) / _world_W * _window_W + 0.5);
    vy = (int)((wx - _world_BLy) / _world_H * _window_H + 0.5);
}

void Camera::begin_drawing()
{
    glViewport(0,0, (int)_fb_W, (int)_fb_H);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double half_W = _world_W / 2;
    double half_H = _world_H / 2;
    glOrtho(-half_W, +half_W,
            -half_H, +half_H,
            0, 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(-(_world_BLx + half_W),
                 -(_world_BLy + half_H),
                 -1);
}

void Camera::check_resize()
{
    int width, height;

    glfwGetFramebufferSize(_window, &width, &height);
    _fb_W = width;
    _fb_H = height;
}

void Camera::draw_grid(double minor_spacing, double major_spacing)
{
    double x,y;

    glDisable(GL_DEPTH_TEST);

    glLineWidth(1);       // lines as thin as possible

    glBegin(GL_LINES);      // every two glVertex calls will draw a line segment

    double left   = (int)(_world_BLx - 1);
    double right  = left + _world_W + 3;
    double bottom = (int)(_world_BLy - 1);
    double top    = bottom + _world_H + 3;

    // the minor vertical grid lines
    glColor3d(0.6, 0.4, 0.4);    // darkish red
    for (x = left; x <= right; x += minor_spacing)
    {
        glVertex2d(x, bottom);
        glVertex2d(x, top);
    }
    // the minor horizontal grid lines
    for (y = bottom; y <= top; y += minor_spacing)
    {
        glVertex2d(left,  y);
        glVertex2d(right, y);
    }

    // the major vertical grid lines: darkish green
    glColor3d(0.4, 0.7, 0.4);
    for (x = left; x <= right; x += major_spacing)
    {
        glVertex2d(x, bottom);
        glVertex2d(x, top);
    }

    // the major horizontal grid lines
    for (y = bottom; y <= top; y += major_spacing)
    {
        glVertex2d(left,  y);
        glVertex2d(right, y);
    }

    // the coordinate axes: pink
    glColor3d(.9,.7,.7);
    glVertex2d(0.0, left);
    glVertex2d(0.0, right);
    glVertex2d(bottom, 0);
    glVertex2d(top, 0);

    glEnd();
}

int Camera::get_win_W() {
    return (int)_fb_W;
}

int Camera::get_win_H() {
    return (int)_fb_H;
}
