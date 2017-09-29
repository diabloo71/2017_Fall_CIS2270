#pragma once
/**
*	Author: Sean Salinas
*	Email :	ssalinas6@cnm.edu
*   File  : CallbackFunctions.h
*/
#include <GL/glew.h>  /* include GLEW and new version of GL on Windows */
#include <GL/glfw3.h> /* GLFW helper library */
#include <stdio.h>
#include "MathFuncs.h"

// Extern variables
extern const int COLORS_ARRAY_SIZE;
extern const float SPEED_FAST;
extern const float SPEED_SLOW;
extern const float SPEED_STOP;
extern int color_index;
extern int g_gl_width;
extern int g_gl_height;
extern float x_speed;
extern float y_speed;
extern float trans_matrix[];

// Global variables
enum DIRECTION { NEG = -1, POS = 1 };


void glfw_error_callback(int error, const char* description);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void glfw_window_size_callback(GLFWwindow *window, int width, int height);