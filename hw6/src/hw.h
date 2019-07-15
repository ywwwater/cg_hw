#pragma once
#ifndef __HW5__
#define __HW5__

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <my_tool/shader.h>
#include "imgui.h"




#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <my_tool/camera.h>

class HW
{
public:
	HW();
	~HW();
	void show_assignment(const float width, const float height, const Camera& camera);
	void use_gui(bool& cursorEnable);
private:
	unsigned int VBO, VAO, lightVAO;
	Shader* boxShaderPS, *boxShaderGS, *lightShader;
	int selected;
	int assignment_num;
	int as1_num;
	bool test_what;
	glm::vec3 lightpos;
	float ambientStrength;
	float specularStrength;
	float diffuseStrength;
	int Shininess;
};

HW::HW() : selected(1), assignment_num(1), as1_num(1), test_what(false)
{
	ambientStrength = 0.1;
	specularStrength = 0.5;
	diffuseStrength = 0.5;
	Shininess = 32;
	
	lightpos = glm::vec3(1.2f, 1.0f, -2.0f);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	//shader prepare
	const char cubefsPS[] =
		"#version 330 core\n"
		"in vec3 Normal;\n"
		"in vec3 FragPos;\n"
		"out vec4 FragColor;\n"
		"uniform vec3 objectColor ;\n"
		"uniform vec3 lightColor ;\n"
		"uniform vec3 lightPos;\n"
		"uniform vec3 viewPos;\n"
		"uniform float ambientStrength;\n"
		"uniform float specularStrength;\n"
		"uniform float diffuseStrength;\n"
		"uniform int Shininess;\n"
		"void main()\n"
		"{\n"
		"	vec3 ambient = ambientStrength * lightColor;\n"
		"	vec3 norm = normalize(Normal);\n"
		"	vec3 lightDir = normalize(lightPos - FragPos);\n"
		"	float diff = max(dot(norm, lightDir), 0.0);\n"
		"	vec3 diffuse = diffuseStrength * diff * lightColor;\n"
		"	vec3 viewDir = normalize(viewPos - FragPos);\n"
		"	vec3 reflectDir = reflect(-lightDir, norm);\n"
		"	float spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess);\n"
		"	vec3 specular = specularStrength * spec * lightColor;\n"
		"	vec3 result = (ambient + diffuse + specular) * objectColor;\n"
		"	FragColor = vec4(result, 1.0);\n"
		"}\n\0";
	const char cubevsPS[] =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aNormal;\n"
		"out vec3 FragPos;\n"
		"out vec3 Normal;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
		"	FragPos = vec3(model * vec4(aPos, 1.0));\n"
		"	Normal = mat3(transpose(inverse(model))) * aNormal;\n"
		"}\n\0";

	const char cubefsGS[] =
		"#version 330 core\n"
		"in vec3 result;\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vec4(result, 1.0);\n"
		"}\n\0";
	const char cubevsGS[] =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aNormal;\n"
		"out vec3 result;\n"
		"uniform vec3 objectColor ;\n"
		"uniform vec3 lightColor ;\n"
		"uniform vec3 lightPos;\n"
		"uniform vec3 viewPos;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"uniform float ambientStrength;\n"
		"uniform float specularStrength;\n"
		"uniform float diffuseStrength;\n"
		"uniform int Shininess;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
		"	vec3 FragPos = vec3(model * vec4(aPos, 1.0));\n"
		"	vec3 Normal = mat3(transpose(inverse(model))) * aNormal;\n"
		"	vec3 ambient = ambientStrength * lightColor;\n"
		"	vec3 norm = normalize(Normal);\n"
		"	vec3 lightDir = normalize(lightPos - FragPos);\n"
		"	float diff = max(dot(norm, lightDir), 0.0);\n"
		"	vec3 diffuse = diffuseStrength * diff * lightColor;\n"
		"	vec3 viewDir = normalize(viewPos - FragPos);\n"
		"	vec3 reflectDir = reflect(-lightDir, norm);\n"
		"	float spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess);\n"
		"	vec3 specular = specularStrength * spec * lightColor;\n"
		"	result = (ambient + diffuse + specular) * objectColor;\n"
		"}\n\0";

	const char lightvs[] =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
		"}\n\0";

	const char lightfs[] =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vec4(1.0);\n"
		"}\n\0";
	boxShaderPS = new Shader(cubevsPS, cubefsPS);
	boxShaderGS = new Shader(cubevsGS, cubefsGS);
	lightShader = new Shader(lightvs, lightfs);
}

HW::~HW()
{
	delete boxShaderPS;
	delete lightShader;
	delete boxShaderGS;
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
}

inline void HW::show_assignment(const float width, const float height, const Camera& camera)
{
	static float angles = 0;
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);

	if (assignment_num == 1) {
		boxShaderPS->use();
	}
	else {
		boxShaderGS->use();
	}

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = camera.getViewMat();
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));
	projection = glm::perspective(glm::radians(45.0f), width / height, 0.1F, 100.0f);

	glUniformMatrix4fv(boxShaderPS->getLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(boxShaderPS->getLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(boxShaderPS->getLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glUniform3f(boxShaderPS->getLocation("objectColor"), 1.0f, 0.5f, 0.31f);
	glUniform3f(boxShaderPS->getLocation("lightColor"), 1.0f, 1.0f, 1.0f);

	glUniform3fv(boxShaderPS->getLocation("lightPos"), 1, glm::value_ptr(lightpos));
	glUniform3fv(boxShaderPS->getLocation("viewPos"), 1, glm::value_ptr(camera.getCameraPos()));
	glUniform1f(boxShaderPS->getLocation("ambientStrength"), ambientStrength);
	glUniform1f(boxShaderPS->getLocation("specularStrength"), specularStrength);
	glUniform1f(boxShaderPS->getLocation("diffuseStrength"), diffuseStrength);
	glUniform1i(boxShaderPS->getLocation("Shininess"), Shininess);

	glBindVertexArray(VAO);

	//glUniform3f(shader->getLocation("our_color"), 1.0f, 0.0f, 0.0f);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	lightShader->use();
	model = glm::translate(model, lightpos);
	model = glm::scale(model, glm::vec3(0.2f));
	glUniformMatrix4fv(lightShader->getLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(lightShader->getLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(lightShader->getLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(lightVAO);

	
	glDrawArrays(GL_TRIANGLES, 0, 36);

}

inline void HW::use_gui(bool& cursorEnable)
{
	ImGui::Begin("hw6");
	ImGui::Checkbox("Enable camera move", &cursorEnable);
	ImGui::SliderFloat3("choose pos of light", glm::value_ptr(lightpos), -10.0f, 10.0f);
	ImGui::SliderFloat("ambientStrength", &ambientStrength, 0.0f, 1.0f);
	ImGui::SliderFloat("specularStrength", &specularStrength, 0.0f, 1.0f);
	ImGui::SliderFloat("diffuseStrength", &diffuseStrength, 0.0f, 1.0f);
	ImGui::SliderInt("Shininess", &Shininess, 0, 128);

	if (ImGui::CollapsingHeader("Select Shading")) {
		if (ImGui::Selectable("Phong Shading", 1 == assignment_num)) {
			assignment_num = 1;
		}
		if (ImGui::Selectable("Gouraud Shading", 2 == assignment_num)) {
			assignment_num = 2;
		}
	}


	ImGui::End();
}



#endif
