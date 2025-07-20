#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <Widgets.h>

static bool firstMouse = true;
static float lastX, lastY, lastTime = 0.0f;

static unsigned long long ticks = 0, lastTick = 0;

int WIDTH = 800, HEIGHT = 800;

void frameBufferSizeCallback(GLFWwindow*, int width, int height);
void cursor_callback(GLFWwindow* window, double xpos, double ypos);
void button_callback(GLFWwindow* window, int button, int action, int mods);

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
	glfwSetCursorPosCallback(window, cursor_callback);
	glfwSetMouseButtonCallback(window, button_callback);

	glClearColor(0.35f, 0.0f, 0.6f, 0.0f);

	TextLib textLib(Shader(SHADER_PATH"/textShaderVertex.txt", SHADER_PATH"/textShaderFragment.txt"));
	TimerClock Timer(textLib);

	float start = glfwGetTime();

	//glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		float time = glfwGetTime();
		ticks += (time - lastTime) * 1000'000;
		
		lastTime = time;

		if (ticks - lastTick >= 16'667) {
			lastTick = ticks;
			//std::cout << "asd";
			
			glClear(GL_COLOR_BUFFER_BIT);

			Timer.draw();
			glfwSwapBuffers(window);
		}

		//std::cout << lastX << " " << lastY << "\n";		
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
	}
	float end = glfwGetTime();
	std::cout << "APP RUN FOR [" << end - start << "] SECONDS\n";
	std::cout << "APP RUN FOR [" << ticks / 1e6 << "] SECONDS\n";

	return 0;
}

void frameBufferSizeCallback(GLFWwindow*, int width, int height) {
	glViewport(0, 0, width, height);
}

void cursor_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = 0;
	}

	lastX = xpos;
	lastY = ypos;
}

void button_callback(GLFWwindow* window, int button, int action, int mods) {

}

