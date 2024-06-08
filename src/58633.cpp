#include <iostream>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>

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


float deltaTime = 0.0f;
float currentFrameTime = 0.0f;
float previousFrameTime = 0.0f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
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

	// glfwSetInputMode(window.win_ptr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	std::cout << glGetString(GL_VERSION) << std::endl;

	uint32_t shaderProg = shaderLoadProgram("./shaders/VertexShader_21.glsl", "./shaders/FragmentShader_21.glsl");
	DBG_ASSERT(shaderProg != 0);
	// uint32_t movingShader = shaderLoadProgram("./shaders/VertexNoLight.glsl", "./shaders/FragmentNoLight.glsl");
	// DBG_ASSERT(movingShader != 0);

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

	uint32_t diffuseMap = loadTextureStatic("./textures/container2.png");
	uint32_t specularMap = loadTextureStatic("./textures/container2_specular.png");
	TexData animMap = loadTextureAnim("./textures/matrix.jpg");

	glm::mat4 identity = glm::mat4(1.0f);

	shaderUse(shaderProg);
	glUniform1i(glGetUniformLocation(shaderProg, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(shaderProg, "material.specular"), 1);
	glUniform1i(glGetUniformLocation(shaderProg, "material.noLight"), 2);

	float light = 0.25f;
	// Game loop
	glm::mat4 currentTrans1(1.0f), currentTrans2(1.0f);
	while (!glfwWindowShouldClose(window.win_ptr)) {
		glfwPollEvents();

		float time = glfwGetTime();

		currentFrameTime = time;
		deltaTime = currentFrameTime - previousFrameTime;
		previousFrameTime = currentFrameTime;

		// std::cout << std::fixed;
		// std::cout << std::setprecision(2);
		// std::cout << 1/deltaTime << " fps \r";

		// glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render Using the shader program
		shaderUse(shaderProg);
		glUniform1f(glGetUniformLocation(shaderProg, "material.shininess"), 32.0f);

		// Light Source
		glm::vec3 lightSourceColor(1.0f, 1.0f, 1.0f);

		// Dir Light
		Allocator ar; // arena allocator
		// size_t lightsLen = len(dirLightSources);
		size_t lightsLen = 0;
		glUniform1i(glGetUniformLocation(shaderProg, "dirLightsLength"), lightsLen);
		for(size_t i = 0; i < lightsLen; i++) {
			glUniform3fv(glGetUniformLocation(shaderProg, f(ar, "dirLights[%d].direction", i)), 1, &(dirLightSources[i])[0]);

			glUniform3fv(glGetUniformLocation(shaderProg, f(ar, "dirLights[%d].ambient", i)), 1, &(glm::vec3(light))[0]);
			glUniform3fv(glGetUniformLocation(shaderProg, f(ar, "dirLights[%d].diffuse", i)), 1, &(glm::vec3(light))[0]);
			glUniform3fv(glGetUniformLocation(shaderProg, f(ar, "dirLights[%d].specular", i)), 1, &(glm::vec3(light))[0]);

		}

		// Point Light
		// glUniform1i(glGetUniformLocation(shaderProg, "pointLightsLength"), len(dirLightSources));
		// for(int i = 0; i < len(dirLightSources); i++) {
		// 	glUniform3fv(glGetUniformLocation(shaderProg, f(ar, "pointLights[%d].Position", i)), 1, &(lightSrcsTable[id].position)[0]);

		// 	glUniform1f(glGetUniformLocation(shaderProg, f(ar, "pointLights[%d].constant", i)), 1.0f);
		// 	glUniform1f(glGetUniformLocation(shaderProg, f(ar, "pointLights[%d].linear", i)), linearAttenMap[linearAtten].linear);
		// 	glUniform1f(glGetUniformLocation(shaderProg, f(ar, "pointLights[%d].quadratic", i)), 0.032f);

		// 	glUniform3fv(glGetUniformLocation(shaderProg, f(ar, "pointLights[%d].ambient", i)), 1, &(glm::vec3(1.0f, 1.0f, 1.0f))[0]);
		// 	glUniform3fv(glGetUniformLocation(shaderProg, f(ar, "pointLights[%d].diffuse", i)), 1, &(glm::vec3(1.0f, 1.0f, 1.0f))[0]);
		// 	glUniform3fv(glGetUniformLocation(shaderProg, f(ar, "pointLights[%d].specular", i)), 1, &(glm::vec3(1.0f, 1.0f, 1.0f))[0]);
		// }

		ar.freeAll();

		glm::mat4 view;
		// glm::vec3 cameraPos = glm::vec3(-10.0f, 0.0f, 80.0f);
		// glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		// glm::vec3 cameraUp = glm::vec3(1.0f, 0.0f, 0.0f);
		// view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
		view = glm::translate(identity, glm::vec3(0.0f, 0.0f, -30.0f));
		glUniformMatrix4fv(glGetUniformLocation(shaderProg, "view"), 1, GL_FALSE, &view[0][0]);

		glm::mat4 projection;

		// 2D
		// projection = glm::ortho(-40.0f, 40.0f, -40.0f, 40.0f, -13.0f, 13.0f);

		// 3D
		projection = glm::perspective(glm::radians(170.0f), (float)(WIDTH)/(float)(HEIGHT), 0.3f, 1000.0f);

		glUniformMatrix4fv(glGetUniformLocation(shaderProg, "projection"), 1, GL_FALSE, &projection[0][0]);

		// bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		// bind specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, animMap.ID);

		glUniform1f(glGetUniformLocation(shaderProg, "time"), time);

		vaoBind(&vao);

		glm::mat4 model;
		model = glm::scale(identity, glm::vec3(20.f, 20.0f, 1.0f));

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
