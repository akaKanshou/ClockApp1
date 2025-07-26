#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <Widgets.h>
#include <exception>

int WIDTH = 800, HEIGHT = 800;

void frameBufferSizeCallback(GLFWwindow*, int width, int height);
void cursor_callback(GLFWwindow* window, double xpos, double ypos);
void button_callback(GLFWwindow* window, int button, int action, int mods);

int main() {
	try {
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
		glfwSetCursorPosCallback(window, cursor_callback);
		glfwSetMouseButtonCallback(window, button_callback);

		glClearColor(0.35f, 0.0f, 0.6f, 0.0f);

		TextLib textLib(Shader(SHADER_PATH"/textShaderVertex.txt", SHADER_PATH"/textShaderFragment.txt"));
		//TimerClock Timer(textLib);
		Screen::pointer thisScreen = Screen::getScreen(textLib);

		double start = glfwGetTime();

		//glEnable(GL_DEPTH_TEST);

		while (!glfwWindowShouldClose(window)) {

			if (thisScreen->doDraw(glfwGetTime())) {
				glClear(GL_COLOR_BUFFER_BIT);

				thisScreen->drawTimer();
				glfwSwapBuffers(window);
			}

			//std::cout << lastX << " " << lastY << "\n";		
			glfwPollEvents();
			thisScreen->processInputs(window);
		}
		double end = glfwGetTime();

		std::cout << "APP RAN FOR [" << end - start << "] SECONDS\n";
		std::cout << "APP RAN FOR [" << thisScreen->runTime() << "] SECONDS\n";

	}
	catch (std::exception& e) {
		std::cout << e.what() << " \n";
	}
	
	glfwTerminate();

	char c; std::cin >> c;

	return 0;
}

void frameBufferSizeCallback(GLFWwindow*, int width, int height) {
	glViewport(0, 0, width, height);
}

void cursor_callback(GLFWwindow* window, double xpos, double ypos) {
	Screen::currentScreen->cursor_callback(window, xpos, ypos);
}

void button_callback(GLFWwindow* window, int button, int action, int mods) {
	Screen::currentScreen->Screen::button_callback(window, button, action, mods);
}

