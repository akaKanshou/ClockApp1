#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <Widgets.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

int WIDTH = 800, HEIGHT = 800;

void frameBufferSizeCallback(GLFWwindow*, int width, int height);

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

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
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	std::cout << "here\n";
	TimerClock Timer;

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		Timer.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	std::cout << "achieved partial greateness\n";

	return 0;
}

void frameBufferSizeCallback(GLFWwindow*, int width, int height) {
	glViewport(0, 0, width, height);
}

