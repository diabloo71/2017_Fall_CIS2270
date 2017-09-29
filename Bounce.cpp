/**
*	Author: Sean Salinas
*	Email :	ssalinas6@cnm.edu
*   File  : Bounce.cpp
*   Desc  : Project #1, draw a bouncing square.
*			Left mouse press = change box speed (fast -> slow -> stop....)
*			Right mouse press = change color
*			Scroll wheel = rotate box on Z axis
*/
#include <GL/glew.h>  /* include GLEW and new version of GL on Windows */
#include <GL/glfw3.h> /* GLFW helper library */
#include "MathFuncs.h"
#include "Display.h"

const int   COLORS_ARRAY_SIZE = 7;     //number of colors in colors array
const float SPEED_FAST        = 2.0f;  //Fast speed (initial speed setting)
const float SPEED_SLOW        = SPEED_FAST/2.0f; //Slow speed
const float SPEED_STOP        = 0.0f;  //Stop speed

int   g_gl_width  = 640;
int   g_gl_height = 480;
int   color_index = 0;
float x_speed     = SPEED_FAST;
float y_speed     = SPEED_FAST;
float trans_matrix[] = { // bind to vertex_shader's matrix
	1.0f, 0.0f, 0.0f, 0.0f, // first column 
	0.0f, 1.0f, 0.0f, 0.0f, // second column 
	0.0f, 0.0f, 1.0f, 0.0f, // third column 
	0.0f, 0.0f, 0.0f, 1.0f  // fourth column 
};

int main() {
	GLuint vbo = 0;
	GLuint vao = 0;
	GLuint shader_programme;
	const GLfloat SQUARE_WIDTH = .5f;
	GLfloat points[] = {
		-0.25f,  0.25f, 0.0f,
		 0.25f,  0.25f, 0.0f,
		-0.25f, -0.25f, 0.0f,

		 0.25f,  0.25f, 0.0f,
		 0.25f, -0.25f, 0.0f,
		-0.25f, -0.25f, 0.0f,
	};
	vec4 colors[] = { // bind to vertex_shader's newColor
		vec4(  1.0f        ,  0.0f         ,  0.0f        ,1.0f),//red
		vec4(  0.0f        ,  1.0f         ,  0.0f        ,1.0f),//green
		vec4(  0.0f        ,  0.0f         ,  1.0f        ,1.0f),//blue
		vec4((255.0f / 255), (20.0f / 255) ,(147.0f / 255),1.0f),//deep pink
		vec4((128.0f / 255),  0.0f         ,  0.0f        ,1.0f),//maroon
		vec4((128.0f / 255),  0.0f         ,(128.0f / 255),1.0f),//purple
		vec4((255.0f / 255),(182.0f / 255) ,(193.0f / 255),1.0f) //hot pink
	};
	/* these are the strings of code for the shaders
	the vertex shader positions each vertex point */
	const char *vertex_shader = "#version 410\n" //define which gl shading language to use
		"in vec3 vertex_position;"
		"uniform mat4 matrix;"
		"uniform vec4 newColor;"
		"out vec4 colour;"
		"void main () {"
		"	gl_Position = matrix * vec4(vertex_position, 1.0f);"
		"   colour = newColor;"
		"}";
	/* the fragment shader colours each fragment (pixel-sized area of the
	triangle) */
	const char *fragment_shader = "#version 410\n"
		"in vec4 colour;"
		"out vec4 frag_colour;"
		"void main () {"
		"	frag_colour = colour;"
		"}";
	/* GL shader objects for vertex and fragment shader [components] */
	GLuint vert_shader, frag_shader;
	
	/* start GL context and O/S window using the GLFW helper library */
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	Display display(g_gl_width, g_gl_height, "Sean Salinas - Program 1 Bounce");

	/* tell GL to only draw onto a pixel if the shape is closer to the viewer
	than anything already drawn at that pixel */
	glEnable(GL_DEPTH_TEST); /* enable depth-testing */
							 /* with LESS depth-testing interprets a smaller depth value as meaning "closer" */
	glDepthFunc(GL_LESS);

	// Set up vertices in the VBO
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW); //allocate space for points array
	
	// Set up vertices in the VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	// Compile vertex shader
	vert_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert_shader, 1, &vertex_shader, NULL);
	glCompileShader(vert_shader);
	
	// Compile fragment shader
	frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader, 1, &fragment_shader, NULL);
	glCompileShader(frag_shader);
	
	// Link vertex and fragment shaders
	shader_programme = glCreateProgram();
	glAttachShader(shader_programme, frag_shader);
	glAttachShader(shader_programme, vert_shader);
	glLinkProgram(shader_programme);

	GLuint matrix_location = glGetUniformLocation(shader_programme, "matrix");
	GLuint colorUniform = glGetUniformLocation(shader_programme, "newColor");
	glUseProgram(shader_programme);
	glUniformMatrix4fv(matrix_location, GL_TRUE, GL_FALSE, trans_matrix);

	const int X_TRANSLATION_INDEX = 12;
	const int Y_TRANSLATION_INDEX = 13;
	float     last_x_position     =  0.0f;
	float     last_y_position     =  0.0f;
	while (!display.IsClosed()) {
		// add a timer for doing animation
		static double previous_seconds = glfwGetTime();
		double current_seconds = glfwGetTime();
		double elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;

		// reverse direction if gone too far left or right
		if (x_speed > 0)
			x_speed = fabs(last_x_position + (SQUARE_WIDTH / 2)) > 1.0f ? -x_speed : x_speed;
		else
			x_speed = fabs(last_x_position - (SQUARE_WIDTH / 2)) > 1.0f ? -x_speed : x_speed;
		// reverse direction if gone too far up or down
		if (y_speed > 0)
			y_speed = fabs(last_y_position + (SQUARE_WIDTH / 2)) > 1.0f ? -y_speed : y_speed;
		else
			y_speed = fabs(last_y_position - (SQUARE_WIDTH / 2)) > 1.0f ? -y_speed : y_speed;

		// update the matrix to move the object on screen
		trans_matrix[X_TRANSLATION_INDEX] = ((float)elapsed_seconds*x_speed) + last_x_position;
		last_x_position = trans_matrix[X_TRANSLATION_INDEX];
		trans_matrix[Y_TRANSLATION_INDEX] = ((float)elapsed_seconds*y_speed) + last_y_position;
		last_y_position = trans_matrix[Y_TRANSLATION_INDEX];
		
		glUseProgram(shader_programme);

		// update box location using the transformation matrix
		glUniformMatrix4fv(matrix_location, GL_TRUE, GL_FALSE, trans_matrix);

		display.Clear(0.5f, 0.5f, 0.5f, 1.0f);
		
		// update box color
		glUniform4fv(colorUniform, GL_TRUE, colors[color_index].v);

		glUseProgram(shader_programme);
		glViewport(0, 0, g_gl_width, g_gl_height);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		display.Update();
	}

	return 0;
}
