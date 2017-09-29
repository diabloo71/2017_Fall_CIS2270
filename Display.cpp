/**
*	Author: Sean Salinas
*	Email :	ssalinas6@cnm.edu
*   File  : Display.h
*/
#include "Display.h"

Display::Display(int width, int height, const std::string& title) {
	if (!glfwInit())
		perror("Error: program ending due to glfwInit issue.\n");

	//create window
	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		exit(1);
	}

	// GLFW hints
	glfwMakeContextCurrent(window);

	// GLEW lines
	glewExperimental = GL_TRUE;
	glewInit();

	// register call-backs
	glfwSetErrorCallback(glfw_error_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);		// mouse button callback
	glfwSetScrollCallback(window, mouse_scroll_callback);			// mouse scroll wheel callback
	glfwSetWindowSizeCallback(window, glfw_window_size_callback);	// window resize callback

	// get version info
	const GLubyte *renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte *version = glGetString(GL_VERSION);	 /* version as a string */
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);
}
void Display::Update() {
	glfwPollEvents();
	glfwSwapBuffers(window);
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		isClosed = true;
	}
	else if (glfwWindowShouldClose(window))
		isClosed = true;
}
void Display::Clear(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
Display::~Display() {
	glfwDestroyWindow(window);
	glfwTerminate();
}