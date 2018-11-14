#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <stdio.h>
#include <GL/glew.h>
#undef GLFW_DLL
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

#define n_voxels (3 * 1000)

void safeExit() {
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}
	
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

void windowResizeHandler(GLFWwindow *window, int width, int height){

    // Use the entire window for rendering.
    glViewport(0, 0, width, height);
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
		safeExit();
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

	//Create screen quad
	static const GLfloat gVertices[] = {
		-1, -1, 0,
		-1,  1, 0,
		 1,  1, 0,
		 1, -1, 0
	};
	static const GLubyte gIndices[] = {
		0, 1, 2,
		0, 3, 4
	};

	//Compile shaders
	GLuint programID = LoadShaders("../resource/shaders/raytrace.vert", "../resource/shaders/raytrace.frag");
	glUseProgram(programID);

	//Voxel data into texture buffer
	float tboData[4] = {1.0, 0.5, 1.0, 1.0};
	GLuint bufferID;
	GLuint textureID;
	GLuint loc = glGetUniformLocation(programID, "voxels");
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_TEXTURE_BUFFER, bufferID);
	glBufferData(GL_TEXTURE_BUFFER, sizeof(tboData), tboData, GL_STATIC_DRAW);
	glGenTextures(1, &textureID);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);
	//glBindTexture(GL_TEXTURE_BUFFER, textureID);
	//glTextureBuffer(textureID, GL_R8, bufferID);

	//View
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glm::vec3 camera(0, 0, 0);
	glm::mat4 viewMatrix = glm::translate(glm::mat4(), camera);
	glm::mat4 projMatrix = glm::perspective(glm::radians(75.0), (double)width / (double)height, 0.1, 100.0);
	glm::mat4 viewProj = projMatrix * viewMatrix;

	//Handlers
	glfwSetWindowSizeCallback(window, windowResizeHandler);

	//Render loop
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS
		&& !glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(programID);
		//Buffer texture voxel data
		//glBufferSubData(GL_TEXTURE_BUFFER, 0, sizeof(voxels), voxels);
		glActiveTexture(GL_TEXTURE0);
		//glBindBuffer(GL_TEXTURE_BUFFER, bufferID);
		glBindTexture(GL_TEXTURE_BUFFER, textureID);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RG32F, bufferID);
		glUniform1i(loc, 0);

		//Set uniforms
		glUniformMatrix4fv(glGetUniformLocation(programID, "viewProj"), 1, GL_FALSE, &viewProj[0][0]);

		//Draw the view quad
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, gVertices);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, gIndices);
		glDisableClientState(GL_VERTEX_ARRAY);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Success
	safeExit();
}

