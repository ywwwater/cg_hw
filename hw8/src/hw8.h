#ifndef __HW8__
#define __HW8__
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "shader.h"
#include "imgui.h"
#include <iostream>
#include <string>
#include <vector>
#include <cmath>


using std::cout;
using std::endl;
using std::vector;

struct pos {
	float x;
	float y;
	pos(float x, float y):x(x), y(y){}
};

pos operator+(const pos &p1, const pos &p2) {
	return pos(p1.x + p2.x, p1.y + p2.y);
}

pos operator*(const pos &p1, const float x) {
	return pos(p1.x * x, p1.y * x);
}

pos operator-(const pos &p1, const pos &p2) {
	return pos(p1.x - p2.x, p1.y - p2.y);
}

class HW8
{
public:
	HW8();
	~HW8();
	void render(const double now);
	void init();
	void addPoint(float x, float y);
	void use_gui();
	void removeLastPoint();
	void startBonus();
private:
	unsigned int VAO_POINT, VBO_POINT, VAO_BEZIER, VBO_BEZIER, VAO_BONUS, VBO_BONUS;
	bool bonusEnable;
	bool isStarted;
	void plotAllAndConnect();
	void drawBezier();
	void calculateBezierPoint();
	void transferPointData();
	

	void renderBezierState(const double now);
	double startTime;

	Shader *shader;
	vector<pos> points;
	vector<pos> bezierPoints;
	vector<pos> bonusPoints;
	float BFunc(const int i, const int n, const float t) const;
	pos QFunc(const float t) const;
};

HW8::HW8()
{
	bonusEnable = false;
	isStarted = false;
	startTime = 0;
}

HW8::~HW8()
{
	delete shader;
	glDeleteVertexArrays(1, &VAO_POINT);
	glDeleteBuffers(1, &VBO_POINT);

	glDeleteVertexArrays(1, &VAO_BEZIER);
	glDeleteBuffers(1, &VBO_BEZIER);

	glDeleteVertexArrays(1, &VAO_BONUS);
	glDeleteBuffers(1, &VBO_BONUS);
}

inline void HW8::render(const double now) {
	plotAllAndConnect();
	drawBezier();
	if (bonusEnable) {
		if (!isStarted) {
			isStarted = true;
			startTime = now;
		}
		renderBezierState(now);
	}
	else {
		isStarted = false;
	}
}

inline void HW8::plotAllAndConnect()
{
	shader->use();
	glBindVertexArray(VAO_POINT);
	glPointSize(10);
	glDrawArrays(GL_POINTS, 0, points.size());
	glDrawArrays(GL_LINE_STRIP, 0, points.size());
}

inline void HW8::drawBezier()
{
	shader->use();
	glBindVertexArray(VAO_BEZIER);
	glDrawArrays(GL_LINE_STRIP, 0, bezierPoints.size());
}

inline void HW8::calculateBezierPoint()
{
	bezierPoints.clear();
	const int num = 50;
	float detaT = float(1) / 50;
	float t = 0;
	for (int i = 0; i <= num; i++, t+= detaT)
	{
		bezierPoints.push_back(QFunc(t));
	}
}

inline void HW8::transferPointData()
{
	if (points.empty()) return;
	glBindVertexArray(VAO_POINT);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_POINT);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(pos), &points[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	glBindVertexArray(VAO_BEZIER);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_BEZIER);
	glBufferData(GL_ARRAY_BUFFER, bezierPoints.size() * sizeof(pos), &bezierPoints[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}


inline void HW8::renderBezierState(const double now)
{
	double duration = 20.0f;
	float t = (now - startTime) / duration;
	if (t > 1.0f || points.size() < 2) {
		bonusEnable = false;
		return;
	}

	bonusPoints = points;
	int start = 0;
	const int size = points.size();
	const int totalSize = (size + 1) * size / 2;
	bonusPoints.reserve(totalSize);

	for (int s = size; s >= 2; s--) {
		for (int j = 0; j < s - 1; j++) {
			pos temp = (bonusPoints[start + j + 1] - bonusPoints[start + j]) * t + bonusPoints[start + j];
			bonusPoints.push_back(temp);
		}
		start += s;
	}

	shader->use();

	glBindVertexArray(VAO_BONUS);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_BONUS);
	glBufferData(GL_ARRAY_BUFFER, (totalSize - size) * sizeof(pos), &bonusPoints[size], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	int pos = 0;
	for (int s = size - 1; s >= 1; s--)
	{
		glDrawArrays(GL_POINTS, pos, s);
		glDrawArrays(GL_LINE_STRIP, pos, s);
		pos += s;
	}
}

inline float HW8::BFunc(const int i, const int n, const float t) const
{
	int sum1 = 1, sum2 = 1;
	for (int j = 0; j < i; j++) {
		sum1 *= (n - j);
	}
	for (int j = 1; j <= i; j++) {
		sum2 *= j;
	}
	return (float)sum1 / sum2 * pow(t, i) * pow(1 - t, n - i);
}

inline pos HW8::QFunc(const float t) const
{
	pos result(0.0f, 0.0f);
	int n = points.size() - 1;
	for (int i = 0; i <= n; i++) {
		result = result + points[i] * BFunc(i, n, t);
	}
	return result;
}

inline void HW8::init()
{
	const char grid_shader_vertex[] =
		"#version 330 core\n"
		"layout (location = 0) in vec2 aPos;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(aPos, 0.0f, 1.0f);\n"
		"}\n\0";
	const char grid_shader_fragment[] =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
		"}\n\0";

	shader = new Shader(grid_shader_vertex, grid_shader_fragment);

	glGenVertexArrays(1, &VAO_POINT);
	glGenBuffers(1, &VBO_POINT);

	glGenVertexArrays(1, &VAO_BEZIER);
	glGenBuffers(1, &VBO_BEZIER);

	glGenVertexArrays(1, &VAO_BONUS);
	glGenBuffers(1, &VBO_BONUS);
}
inline void HW8::addPoint(float x, float y)
{
	bonusEnable = false;
	points.push_back(pos(x, y));
	calculateBezierPoint();
	transferPointData();
}
inline void HW8::use_gui()
{
	ImGui::Begin("hw3");
	ImGui::Checkbox("enable Bonus", &bonusEnable);
	ImGui::End();
}
inline void HW8::removeLastPoint()
{
	if (points.empty()) return;
	bonusEnable = false;
	points.pop_back();
	calculateBezierPoint();
	transferPointData();
}
inline void HW8::startBonus()
{
	bonusEnable = true;
}
#endif 
