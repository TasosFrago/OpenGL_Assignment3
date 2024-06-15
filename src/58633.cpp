#include <iostream>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>

#ifndef GLEW_GUARD_H
#define GLEW_GUARD_H
#include <GL/glew.h>
#endif // GLEW_GUARD_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef DEBUG
	#define GLM_ENABLE_EXPERIMENTAL
	#include <glm/gtx/string_cast.hpp>
#endif

#include "lib/windowGlfw.h"
#include "lib/dbg_assert.h"
#include "lib/shader_utl.h"
#include "lib/vertexArray/vao.h"

#include "Camera.hpp"
#include "helper.hpp"
#include "data.hpp"

#define WIDTH 800
#define HEIGHT 800

#define len(arr) (sizeof(arr)/sizeof(arr[0]))

#define AM 58633
#define U (AM % 5)

Camera myCamera(glm::vec3(0.0f, 0.0f, -20.0f));

float deltaTime = 0.0f;
float currentFrameTime = 0.0f;
float previousFrameTime = 0.0f;

float previousMousePositonX = 400.0f;
float previousMousePositonY = 300.0f;

bool firstMouse = true;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if(key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		bool *spacePressed;
		spacePressed = (bool *)glfwGetWindowUserPointer(window);
		if(*spacePressed == false) {
			*spacePressed = true;
		}
		glfwSetWindowUserPointer(window, (void *)spacePressed);
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
}

void MousePositionCallback(GLFWwindow* givenWindow, double givenMousePositionX, double givenMousePositionY)
{
	//std::cout << givenMousePositionX << std::endl;
	//std::cout << givenMousePositionY << std::endl;

	if(firstMouse) {
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

	// Set the callback function
	glfwSetKeyCallback(window.win_ptr, key_callback);

	glfwSetCursorPosCallback(window.win_ptr, MousePositionCallback);
	glfwSetScrollCallback(window.win_ptr, ScrollCallback);
	glfwSetInputMode(window.win_ptr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glfwSetInputMode(window.win_ptr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	std::cout << glGetString(GL_VERSION) << std::endl;

	uint32_t shaderProg = shaderLoadProgram("./shaders/VertexShader_21.glsl", "./shaders/FragmentShader_21.glsl");
	DBG_ASSERT(shaderProg != 0);
	uint32_t shaderALight = shaderLoadProgram("./shaders/VertexALight.glsl", "./shaders/FragmentALight.glsl");
	DBG_ASSERT(shaderProg != 0);

	VAO_t vao;
	vaoGen(&vao);
	vaoBind(&vao);

	VBO_t vbo;
	vboGen(&vbo, vertices, sizeof(vertices), GL_STATIC_DRAW);

	VBLayout_t vbl;
	vbl_new(&vbl, 8 * sizeof(float));
	vbl_push_float(&vbl, 3);
	vbl_push_float(&vbl, 3);
	vbl_push_float(&vbl, 2);

	vaoAddBuffer(&vao, &vbo, &vbl);
	DBG_GLCHECKERROR();

	vbl_destroy(&vbl);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	uint32_t diffuseMap = loadTexture("./textures/container2.png");
	uint32_t specularMap = loadTexture("./textures/container2_specular.png");
	uint32_t movingMap = loadTexture("./textures/matrix.jpg");

	glm::mat4 identity = glm::mat4(1.0f);

	shaderUse(shaderProg);
	glUniform1i(glGetUniformLocation(shaderProg, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(shaderProg, "material.specular"), 1);
	glUniform1i(glGetUniformLocation(shaderProg, "material.noLight"), 2);

	uint32_t seed = 0;
	enum randomEventStates {
		NO_CUBE = 0,
		FIRST_CUBE = 1,
		SECOND_CUBE = 2,
		THIRD_CUBE = 3,
	};
	randomEventStates rdEventState = NO_CUBE;
	// randomEventStates rdEventState = FIRST_CUBE;
	glm::vec3 rdEventPos;

	bool spacePressed = false;
	glfwSetWindowUserPointer(window.win_ptr, &spacePressed);

	float light = 0.25f;
	float speed = 2.0f;
	float accumulatedTime = 0.0f;

	glm::mat4 model2 = identity;
	glm::mat4 model = identity;
	// Game loop
	while (!glfwWindowShouldClose(window.win_ptr)) {
		glfwPollEvents();

		ProcessInput(window.win_ptr);

		float time = glfwGetTime();
		// std::cout << time << std::endl;

		if(spacePressed == true) {
			rdEventState = FIRST_CUBE;
		}

		if(spacePressed == true && rdEventState == FIRST_CUBE) {
			seed = (uint32_t)(time * 1000);
			std::srand(seed);
			float randX = (float)((std::rand() % (15 + 15 + 1)) - 15);
			std::srand(seed + 5);
			float randZ = (float)((std::rand() % (15 + 15 + 1)) - 15);
			rdEventPos = glm::vec3(randX, 15, randZ);

			spacePressed = false;
		}

		currentFrameTime = time;
		deltaTime = currentFrameTime - previousFrameTime;
		previousFrameTime = currentFrameTime;
		accumulatedTime += deltaTime;

		// glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// VIEW
		glm::mat4 view;
		// glm::vec3 cameraPos = glm::vec3(-10.0f, 0.0f, 80.0f);
		// glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		// glm::vec3 cameraUp = glm::vec3(1.0f, 0.0f, 0.0f);
		// view = glm::lookAt(cameraPos, cameraTarget, cameraUp);

		// view = glm::translate(identity, glm::vec3(0.0f, 0.0f, 21.0f));
		view = myCamera.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(shaderProg, "view"), 1, GL_FALSE, &view[0][0]);

		// PROJECTION
		glm::mat4 projection;

		// 2D
		// projection = glm::ortho(-40.0f, 40.0f, -40.0f, 40.0f, -27.0f, 23.0f);

		// 3D
		// projection = glm::perspective(glm::radians(170.0f), (float)(WIDTH)/(float)(HEIGHT), 0.3f, 1000.0f);
		projection = glm::perspective(glm::radians(myCamera.Zoom), (float)(WIDTH)/(float)(HEIGHT), 0.3f, 5000.0f);

		glUniformMatrix4fv(glGetUniformLocation(shaderProg, "projection"), 1, GL_FALSE, &projection[0][0]);

		// Light Source
		glm::vec3 lightSourceColor(1.0f, 1.0f, 1.0f);

		switch(rdEventState) {
		case NO_CUBE:
			shaderUse(shaderProg);
			glUniform1f(glGetUniformLocation(shaderProg, "material.shininess"), 32.0f);

			glUniform3fv(glGetUniformLocation(shaderProg, "viewPos"), 1, &myCamera.Position[0]);
			{ // Dir Light
				size_t lightsLen = len(dirLightSources);
				Allocator ar; // arena Allocator
				glUniform1i(glGetUniformLocation(shaderProg, "dirLightsLength"), lightsLen);
				for(size_t i = 0; i < lightsLen; i++) {
					glUniform3fv(glGetUniformLocation(shaderProg, f(ar, "dirLights[%d].direction", i)), 1, &(dirLightSources[i])[0]);

					glUniform3fv(glGetUniformLocation(shaderProg, f(ar, "dirLights[%d].ambient", i)), 1, &(glm::vec3(light))[0]);
					glUniform3fv(glGetUniformLocation(shaderProg, f(ar, "dirLights[%d].diffuse", i)), 1, &(glm::vec3(light))[0]);
					glUniform3fv(glGetUniformLocation(shaderProg, f(ar, "dirLights[%d].specular", i)), 1, &(glm::vec3(light))[0]);

				}
				ar.freeAll();
			}
			glUniform1i(glGetUniformLocation(shaderProg, "enablePointLight"), false);
			break;
		case FIRST_CUBE:
			shaderUse(shaderProg);
			glUniform1i(glGetUniformLocation(shaderProg, "dirLightsLength"), 0);
			model2 = glm::translate(identity, rdEventPos);

			if(accumulatedTime < 0.5) {
				glUniform1i(glGetUniformLocation(shaderProg, "enablePointLight"), true);
				// glm::vec3 movingPosition = glm::vec3(model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
				// glm::vec4 localCoords = glm::inverse(model) * glm::vec4(movingPosition, 1.0f);
				// glm::vec3 movingPotion = glm::vec3(localCoords.x, localCoords.y, localCoords.z);
				glm::vec3 movingPos = glm::vec3(glm::inverse(model) * glm::vec4(rdEventPos, 1.0f));

				glUniform3fv(glGetUniformLocation(shaderProg, "pointLight.Position"), 1, &(movingPos)[0]);

				glUniform1f(glGetUniformLocation(shaderProg, "pointLight.constant"), 1.0f);
				glUniform1f(glGetUniformLocation(shaderProg, "pointLight.linear"), 0.09f);
				glUniform1f(glGetUniformLocation(shaderProg, "pointLight.quadratic"), 0.032f);

				glUniform3fv(glGetUniformLocation(shaderProg, "pointLight.ambient"), 1, &(glm::vec3(1.0f, 1.0f, 1.0f))[0]);
				glUniform3fv(glGetUniformLocation(shaderProg, "pointLight.diffuse"), 1, &(glm::vec3(1.0f, 1.0f, 1.0f))[0]);
				glUniform3fv(glGetUniformLocation(shaderProg, "pointLight.specular"), 1, &(glm::vec3(1.0f, 1.0f, 1.0f))[0]);
			} else if(accumulatedTime >= 1.0) {
				glUniform1i(glGetUniformLocation(shaderProg, "enablePointLight"), false);
				accumulatedTime = 0.0f;
			}

			shaderUse(shaderALight);

			glUniformMatrix4fv(glGetUniformLocation(shaderALight, "view"), 1, GL_FALSE, &view[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(shaderALight, "projection"), 1, GL_FALSE, &projection[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(shaderALight, "model"), 1, GL_FALSE, &model2[0][0]);

			vaoBind(&vao);
			glDrawArrays(GL_TRIANGLES, 0, len(vertices));
			DBG_GLCHECKERROR();

			if(rdEventPos.y > 1.0f) {
				rdEventPos.y -= deltaTime * speed;
			} else if(rdEventPos.z > 20) {
				rdEventState = NO_CUBE;
			}
			rdEventPos.z += deltaTime * speed;
			break;
		}

		// bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		// bind specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, movingMap);

		shaderUse(shaderProg);
		glUniform1f(glGetUniformLocation(shaderProg, "time"), time);

		vaoBind(&vao);

		model = glm::scale(identity, glm::vec3(6.4f, 1.0f, 6.4f));

		glUniformMatrix4fv(glGetUniformLocation(shaderProg, "model"), 1, GL_FALSE, &model[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, len(vertices));
		DBG_GLCHECKERROR();


		glfwSwapBuffers(window.win_ptr);
	}

	vaoDelete(&vao);
	destroyWindow(&window);

	glfwTerminate();
	return 0;
}
