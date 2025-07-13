#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int WIDTH = 800, HEIGHT = 800;

void frameBufferSizeCallback(GLFWwindow*, int width, int height);

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "window", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (!window) {
		std::cout << "FAILED TO CREATE WINDOW\n";
		glfwTerminate();
		return -1;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "FAILED TO INITIALIZE GLAD";
		glfwTerminate();
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

	std::cout << "here\n";
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}

	std::cout << "achieved partial greateness\n";

	return 0;
}

void frameBufferSizeCallback(GLFWwindow*, int width, int height) {
	glViewport(0, 0, width, height);
}
