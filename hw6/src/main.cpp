#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <my_tool/shader.h>
#include <my_tool/camera.h>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

#include <iostream>
#include "hw.h"







using std::cout;
using std::endl;

void framebuffer__size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
float lastFrame = 0.0f;
float deltaTime = 0.0f;
int width, height;
float yaw, pitch;
bool firstMouse = true;
bool cursorEnable = false;
Camera camera;

int main() {
	int hw_num = 2;
	yaw = -90.0f;
	pitch = 0;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "hw", NULL, NULL);
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

	
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer__size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	
	HW hw;
	bool use_gui = true;
	while (!glfwWindowShouldClose(window))
	{
		if (cursorEnable) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}


		processInput(window);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwPollEvents();
		glfwMakeContextCurrent(window);
		glfwGetWindowSize(window, &width, &height);

		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		hw.show_assignment(width, height, camera);
		hw.use_gui(cursorEnable);
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

	deltaTime = (float)glfwGetTime() - lastFrame;
	lastFrame = (float)glfwGetTime();
	float cameraSpeed = 2.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.moveForward(cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.moveBack(cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.moveLeft(cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.moveRight(cameraSpeed);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	static float lastX;
	static float lastY;
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}
	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos;

	lastX = (float)xpos;
	lastY = (float)ypos;

	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}
	camera.rotate(pitch, yaw);
}
