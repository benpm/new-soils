#include <iostream>
#include <stdio.h>
#include <GL/glew.h>
#undef GLFW_DLL
#include <GLFW/glfw3.h>

void safe_exit() {
	glfwTerminate();
	exit(EXIT_FAILURE);
}

int main(int argc, char const *argv[]) {
	//Init GLFW
	if (!glfwInit()) {
		puts("GLFW3 failed to initialize");
		exit(EXIT_FAILURE);
	}

	//Init GLFW window
	GLFWwindow* window = glfwCreateWindow(640, 480, "New Soils", NULL, NULL);
	if (!window) {
		puts("Could not open GLFW3 window");
		safe_exit();
	}
	glfwMakeContextCurrent(window);

	//Extension handler GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	//Get and display version information
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	//Setup for OpenGL
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

	//Success
	exit(EXIT_SUCCESS);
}

