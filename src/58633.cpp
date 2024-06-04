#include <iostream>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#ifndef GLEW_GUARD_H
#define GLEW_GUARD_H
#include <GL/glew.h>
#endif // GLEW_GUARD_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "lib/windowGlfw.h"
#include "lib/dbg_assert.h"
#include "lib/shader_utl.h"
#include "lib/vertexArray/vao.h"

#include "Camera.hpp"
#include "helper.hpp"

#define WIDTH 800
#define HEIGHT 800

#define AM 58633
#define U (AM % 5)


Camera myCamera(glm::vec3(0.0f, 0.0f, 3.0f));

float deltaTime = 0.0f;
float currentFrameTime = 0.0f;
float previousFrameTime = 0.0f;

float previousMousePositonX = 400.0f;
float previousMousePositonY = 300.0f;

bool firstMouse = true;

void FrameBufferSizeCallback(GLFWwindow* givenWindow, int givenWidth, int givenHeight)
{
	glViewport(0, 0, givenWidth, givenHeight);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void ProcessInput(GLFWwindow* givenWindow)
{
	if(glfwGetKey(givenWindow, GLFW_KEY_W) == GLFW_PRESS)
		myCamera.ProcessKeyboard(FORWARD, deltaTime);

	if(glfwGetKey(givenWindow, GLFW_KEY_S) == GLFW_PRESS)
		myCamera.ProcessKeyboard(BACKWARD, deltaTime);

	if(glfwGetKey(givenWindow, GLFW_KEY_A) == GLFW_PRESS)
		myCamera.ProcessKeyboard(LEFT, deltaTime);

	if(glfwGetKey(givenWindow, GLFW_KEY_D) == GLFW_PRESS)
		myCamera.ProcessKeyboard(RIGHT, deltaTime);

	if(glfwGetKey(givenWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		// Get the globall state of the <TAB> key
		bool *disabled;
		disabled = (bool *)glfwGetWindowUserPointer(givenWindow);
		*disabled = true;
		glfwSetWindowUserPointer(givenWindow, (void *)disabled);
	}
	if(glfwGetKey(givenWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE) {
		// Get the globall state of the <TAB> key
		bool *disabled;
		disabled = (bool *)glfwGetWindowUserPointer(givenWindow);
		*disabled = false;
		glfwSetWindowUserPointer(givenWindow, (void *)disabled);
	}
}

void MousPositionCallback(GLFWwindow* givenWindow, double givenMousePositionX, double givenMousePositionY)
{
	//std::cout << givenMousePositionX << std::endl;
	//std::cout << givenMousePositionY << std::endl;
	bool *disabled;
	disabled = (bool *)glfwGetWindowUserPointer(givenWindow);
	if(!*disabled) {

		if (firstMouse) {
			previousMousePositonX = givenMousePositionX;
			previousMousePositonY = givenMousePositionY;

			firstMouse = false;
		}

		float offsetX = givenMousePositionX - previousMousePositonX;
		float offsetY = previousMousePositonY - givenMousePositionY;
		previousMousePositonX = givenMousePositionX;
		previousMousePositonY = givenMousePositionY;

		myCamera.ProcessMouseMovement(offsetX, offsetY);
	}
}

void ScrollCallback(GLFWwindow* givenWindow, double givenScrollOffsetX, double givenScrollOffsetY)
{
	myCamera.ProcessMouseScroll(givenScrollOffsetY);
}

int main()
{
	window_t window = {
		.width=WIDTH,
		.height=HEIGHT
	};
	createWindow(&window);
	DBG_ASSERT(window.win_ptr != NULL);

	// Set Window state for toogling tab key
	bool disabled = false;
	glfwSetWindowUserPointer(window.win_ptr, (void *)&disabled);
	glfwSetKeyCallback(window.win_ptr, key_callback);

	// Set the callback function
	glfwSetCursorPosCallback(window.win_ptr, MousPositionCallback);
	glfwSetScrollCallback(window.win_ptr, ScrollCallback);
	glfwSetKeyCallback(window.win_ptr, key_callback);

	glfwSetInputMode(window.win_ptr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	std::cout << glGetString(GL_VERSION) << std::endl;

	auto IDs = getLightSrcIds((Unum)U);
	std::vector<int> pointLightsIds;
	std::vector<int> dirLightsIds;
	for(auto ids : IDs) {
		if(lightSrcsTable[ids].type == POSITIONAL) {
			pointLightsIds.push_back(ids);
		} else if(lightSrcsTable[ids].type == DIRECTIONAL) {
			dirLightsIds.push_back(ids);
		}
	}

	//Data

	//--Vertex Data
	float vertices[] =
	{
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

	glm::vec3 cubePositions[] =
	{
	  glm::vec3(0.0f,  0.0f,  0.0f),
	  glm::vec3(2.0f,  5.0f, -15.0f),
	  glm::vec3(-1.5f, -2.2f, -2.5f),
	  glm::vec3(-3.8f, -2.0f, -12.3f),
	  glm::vec3(2.4f, -0.4f, -3.5f),
	  glm::vec3(-1.7f,  3.0f, -7.5f),
	  glm::vec3(1.3f, -2.0f, -2.5f),
	  glm::vec3(1.5f,  2.0f, -2.5f),
	  glm::vec3(1.5f,  0.2f, -1.5f),
	  glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	uint32_t shaderProg = shaderLoadProgram("./shaders/VertexShader_21.glsl", "./shaders/FragmentShader_21.glsl");
	DBG_ASSERT(shaderProg != 0);
	uint32_t lightShaderProg = shaderLoadProgram("./shaders/Vertex_light_21.glsl", "./shaders/Fragment_light_21.glsl");
	DBG_ASSERT(lightShaderProg != 0);

	VAO_t vao;
	vaoGen(&vao);
	vaoBind(&vao);

	VBO_t vbo;
	vboGen(&vbo, vertices, sizeof(vertices), GL_STATIC_DRAW);

	VBLayout_t vbl;
	vbl_new(&vbl, 6 * sizeof(float));
	vbl_push_float(&vbl, 3);
	vbl_push_float(&vbl, 3);

	vaoAddBuffer(&vao, &vbo, &vbl);
	DBG_GLCHECKERROR();

	vbl_destroy(&vbl);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//glm::vec4 point = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	//glm::mat4 transform = glm::mat4(1.0f);
	//transform = glm::translate(transform, glm::vec3(2.0f, 1.0f, 0.0f));
	//point = transform * point;

	//std::cout << point.x << point.y << point.z << std::endl;

	glm::mat4 identity = glm::mat4(1.0f);

	static float radius = 5.0f;
	static float speed = 2.5f;
	static int linearAtten = 2;
	bool pointLightsToggle[] = { 1, 1, 1, 1, 1, 1 };

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Game loop
	glm::mat4 currentTrans1(1.0f), currentTrans2(1.0f);
	while (!glfwWindowShouldClose(window.win_ptr))
	{
		glfwPollEvents();
		ProcessInput(window.win_ptr);

		bool *disabled;
		disabled = (bool *)glfwGetWindowUserPointer(window.win_ptr);

		if(*disabled)
			glfwSetInputMode(window.win_ptr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(window.win_ptr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


		float time = glfwGetTime();

		currentFrameTime = time;
		deltaTime = currentFrameTime - previousFrameTime;
		previousFrameTime = currentFrameTime;

		// std::cout << std::fixed;
		// std::cout << std::setprecision(2);
		// std::cout << 1/deltaTime << " fps \r";

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render Using the shader program
		shaderUse(shaderProg);

		glUniform3fv(glGetUniformLocation(shaderProg, "cameraPosition"), 1, &myCamera.Position[0]);

		// Light Source
		glm::vec3 lightSourceColor(1.0f, 1.0f, 1.0f);

		// Point Light
		Allocator ar;
		glUniform1i(glGetUniformLocation(shaderProg, "pointLightsLength"), pointLightsIds.size() + 1);
		int i = 0;
		for(auto id : pointLightsIds) {
			glUniform3fv(glGetUniformLocation(shaderProg, f(ar, "pointLights[%d].Position", i)), 1, &(lightSrcsTable[id].position)[0]);

			glUniform1f(glGetUniformLocation(shaderProg, f(ar, "pointLights[%d].constant", i)), 1.0f);
			glUniform1f(glGetUniformLocation(shaderProg, f(ar, "pointLights[%d].linear", i)), linearAttenMap[linearAtten].linear);
			glUniform1f(glGetUniformLocation(shaderProg, f(ar, "pointLights[%d].quadratic", i)), 0.032f);

			glUniform3fv(glGetUniformLocation(shaderProg, f(ar, "pointLights[%d].ambient", i)), 1, &(glm::vec3(1.0f, 1.0f, 1.0f))[0]);
			glUniform3fv(glGetUniformLocation(shaderProg, f(ar, "pointLights[%d].diffuse", i)), 1, &(glm::vec3(1.0f, 1.0f, 1.0f))[0]);
			glUniform3fv(glGetUniformLocation(shaderProg, f(ar, "pointLights[%d].specular", i)), 1, &(glm::vec3(1.0f, 1.0f, 1.0f))[0]);

			i++;
		}
		ar.freeAll();

		// Rotating light source
		glm::mat4 model2 = updateModelTrans((CubeRotationAttribs){
							.RotationSpeed=speed,
							.SpinSpeed=speed,
							.RotationToggle=true,
							.SpinToggle=true,
							.RotationRadius=radius,
							.scaleVec=SCALE_M(0.5f)
						}, currentTrans1, currentTrans2, deltaTime);

		int index = pointLightsIds.size();
		glm::vec3 movingPosition = glm::vec3(model2 * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		glUniform3fv(glGetUniformLocation(shaderProg, f(ar, "pointLights[%d].Position", index)), 1, &movingPosition[0]);

		glUniform1f(glGetUniformLocation(shaderProg, f(ar, "pointLights[%d].constant", index)), 1.0f);
		glUniform1f(glGetUniformLocation(shaderProg, f(ar, "pointLights[%d].linear", index)), linearAttenMap[linearAtten].linear);
		glUniform1f(glGetUniformLocation(shaderProg, f(ar, "pointLights[%d].quadratic", index)), 0.032f);

		glUniform3fv(glGetUniformLocation(shaderProg, f(ar, "pointLights[%d].ambient", index)), 1, &(glm::vec3(1.0f, 1.0f, 1.0f))[0]);
		glUniform3fv(glGetUniformLocation(shaderProg, f(ar, "pointLights[%d].diffuse", index)), 1, &(glm::vec3(1.0f, 1.0f, 1.0f))[0]);
		glUniform3fv(glGetUniformLocation(shaderProg, f(ar, "pointLights[%d].specular", index)), 1, &(glm::vec3(1.0f, 1.0f, 1.0f))[0]);

		// Dir Light
		Allocator ar2;
		glUniform1i(glGetUniformLocation(shaderProg, "dirLightsLength"), dirLightsIds.size());
		i = 0;
		for(auto id : dirLightsIds) {
			glUniform3fv(glGetUniformLocation(shaderProg, f(ar2, "dirLights[%d].direction", i)), 1, &(lightSrcsTable[id].direction)[0]);

			glUniform3fv(glGetUniformLocation(shaderProg, f(ar2, "dirLights[%d].ambient", i)), 1, &(glm::vec3(1.0f, 1.0f, 1.0f))[0]);
			glUniform3fv(glGetUniformLocation(shaderProg, f(ar2, "dirLights[%d].diffuse", i)), 1, &(glm::vec3(1.0f, 1.0f, 1.0f))[0]);
			glUniform3fv(glGetUniformLocation(shaderProg, f(ar2, "dirLights[%d].specular", i)), 1, &(glm::vec3(1.0f, 1.0f, 1.0f))[0]);

			i++;
		}
		ar2.freeAll();

		glm::mat4 view;
		view = myCamera.GetViewMatrix(); // now we get the view matrix form the camrera class -------------
		glUniformMatrix4fv(glGetUniformLocation(shaderProg, "view"), 1, GL_FALSE, &view[0][0]);

		glm::mat4 projection;

		// 2D
		// projection = glm::ortho(-5.0f, 5.0f, -6.0f, 6.0f, -15.0f, 18.0f); // the values were chosen to fit the cube positions in each axis

		// 3D
		projection = glm::perspective(glm::radians(myCamera.Zoom), 800.0f / 600.0f, 0.3f, 100.0f); // now we get the zoom form the camera class ---------------------

		glUniformMatrix4fv(glGetUniformLocation(shaderProg, "projection"), 1, GL_FALSE, &projection[0][0]);

		// Material
		for (int i = 0; i < 10; i++) {
			glUniform3fv(glGetUniformLocation(shaderProg, "material.ambient"), 1, &(materials[i].ambient)[0]);
			glUniform3fv(glGetUniformLocation(shaderProg, "material.diffuse"), 1, &(materials[i].diffuse)[0]);
			glUniform3fv(glGetUniformLocation(shaderProg, "material.specular"), 1, &(materials[i].specular)[0]);
			glUniform1f(glGetUniformLocation(shaderProg, "material.shininess"), 128.0f * materials[i].shininess);

			glm::mat4 model;

			model = glm::translate(identity, cubePositions[i]);
			model = glm::rotate(model, glm::radians(20.0f) * i, glm::vec3(1.0f, 0.3f, 0.5f));

			glUniformMatrix4fv(glGetUniformLocation(shaderProg, "model"), 1, GL_FALSE, &model[0][0]);

			glm::mat3 normalTransformation = glm::transpose(glm::inverse(glm::mat3(model)));
			glUniformMatrix3fv(glGetUniformLocation(shaderProg, "normalTransformation"), 1, GL_FALSE, &normalTransformation[0][0]);

			glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices)/sizeof(vertices[0]));
			DBG_GLCHECKERROR();
		}

		// Render Using the light source shader program
		shaderUse(lightShaderProg);

		glUniform3fv(glGetUniformLocation(lightShaderProg, "lightSourceColor"), 1, &lightSourceColor[0]);
		glUniformMatrix4fv(glGetUniformLocation(lightShaderProg, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(lightShaderProg, "projection"), 1, GL_FALSE, &projection[0][0]);

		for(auto id : IDs) {
			glm::vec3 lightSourcePosition = lightSrcsTable[id].position;
			glm::mat4 model = glm::translate(identity, lightSourcePosition);
			glUniformMatrix4fv(glGetUniformLocation(lightShaderProg, "model"), 1, GL_FALSE, &model[0][0]);

			glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices)/sizeof(vertices[0]));
			DBG_GLCHECKERROR();
		}


		glUniformMatrix4fv(glGetUniformLocation(lightShaderProg, "model"), 1, GL_FALSE, &model2[0][0]);
		// Draw the model
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices)/sizeof(vertices[0]));

		glfwSwapBuffers(window.win_ptr);
	}

	vaoDelete(&vao);
	destroyWindow(&window);

	glfwTerminate();
	return 0;
}
