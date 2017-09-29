/**
*	Author: Sean Salinas
*	Email :	ssalinas6@cnm.edu
*   File  : CallbackFunctions.cpp
*/
#include "CallbackFunctions.h"
DIRECTION prev_direction = POS;

void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) 
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		if (x_speed == SPEED_FAST) 
		{
			x_speed = y_speed = SPEED_SLOW;
		}
		else if (x_speed == SPEED_SLOW) 
		{
			x_speed = y_speed = SPEED_STOP;
			prev_direction = POS;
		}
		else if (x_speed == (SPEED_FAST * -1.0f))
		{
			x_speed = y_speed = SPEED_SLOW * -1.0f;
		}
		else if (x_speed == (SPEED_SLOW * -1.0f))
		{
			x_speed = y_speed = SPEED_STOP * -1.0f;
			prev_direction = NEG;
		}
		else 
		{
			x_speed = y_speed = SPEED_FAST * prev_direction;
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		color_index = (color_index + 1) % COLORS_ARRAY_SIZE;

		/* To change colors in the reverse order
		color_index = (color_index - 1) % COLORS_ARRAY_SIZE;
		if (color_index < 0)
			color_index = COLORS_ARRAY_SIZE - 1;*/
	}
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
	{
		// code for middle mouse button press
		printf("Middle mouse button pressed.\n");
	}
}
mat4 test;
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) 
{
	// code for mouse scroll
	for (int i = 0; i < 17; i++)
		test.m[i] = trans_matrix[i];
	test = rotate_z_deg(test, yoffset > 0 ? 10 : -10);
	for (int i = 0; i < 17; i++)
		trans_matrix[i] = test.m[i];
}

void glfw_window_size_callback(GLFWwindow *window, int width, int height) 
{
	g_gl_width = width;
	g_gl_height = height;
	printf("width %i height %i\n", width, height);
}
