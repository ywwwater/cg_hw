#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include <iostream>
#include "hw8.h"



using std::cout;
using std::endl;

void framebuffer__size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

int width, height;
float yaw, pitch;
bool firstMouse = true;
bool cursorEnable = true;


const int HEIGHT = 600;
const int WIDTH = 800;

double mousePosX;
double mousePosY;


HW8 hw8;
int main() {
	yaw = 0;
	pitch = 0;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "hw", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to create GLF window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}
	const char* glsl_version = "#version 330";
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGui::StyleColorsClassic();

	
	glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer__size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	hw8.init();

	while (!glfwWindowShouldClose(window))
	{
		if (cursorEnable) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		processInput(window);
		glfwPollEvents();
		glfwMakeContextCurrent(window);
		glfwGetWindowSize(window, &width, &height);
		float point_size = width < height ? width / (float)10 : height / (float)10;

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		hw8.render(glfwGetTime());


		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}

void framebuffer__size_callback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		cursorEnable = true;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		hw8.startBonus();
	}

}

void mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		float x = (mousePosX - WIDTH / 2) * 2 / WIDTH;
		float y = -(mousePosY - HEIGHT / 2) * 2 / HEIGHT;
		hw8.addPoint(x, y);

	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		hw8.removeLastPoint();
	}
	
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	mousePosX = xpos;
	mousePosY = ypos;

}
