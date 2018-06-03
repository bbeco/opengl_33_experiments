#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

int width = 800, height = 600;
glm::vec3 cameraPos = glm::vec3(1.0f, 1.0f, 3.0);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastMouseX, lastMouseY;
float yaw, pitch;

bool firstMouse = true;

glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

// Processes user inputs
void processInput(GLFWwindow *window)
{
	float cameraSpeed = 1.0f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	} else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	} else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += cameraSpeed*cameraFront;
	} else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= cameraSpeed*cameraFront;
	} else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -=
				cameraSpeed*glm::normalize(glm::cross(cameraFront, cameraUp));
	} else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos +=
				cameraSpeed*glm::normalize(glm::cross(cameraFront, cameraUp));
	} else if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
		lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	} else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		lightColor = glm::vec3(1.0f, 0.0f, 0.0f);
	} else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		lightColor = glm::vec3(0.0f, 1.0f, 0.0f);
	} else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		lightColor = glm::vec3(0.0f, 0.0f, 1.0f);
	}
}

//called when the mouse moves. It updates the view direction
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse) {
		lastMouseX = xpos;
		lastMouseY = ypos;
		yaw = asin(cameraFront.z);
		pitch = asin(cameraFront.y);
		firstMouse = false;
		return;
	}
	float xoffset = xpos - lastMouseX;
	float yoffset = lastMouseY - ypos;
	lastMouseX = xpos;
	lastMouseY = ypos;

	float sensitivity = 0.002f;
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

	cameraFront.x = cos(pitch)*cos(yaw);
	cameraFront.y = sin(pitch);
	cameraFront.z = cos(pitch)*sin(yaw);
	cameraFront = glm::normalize(cameraFront);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create the window
	GLFWwindow* window = glfwCreateWindow(800, 600, "Basic Colors", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	// make the context of this window the current (for this thread).
	glfwMakeContextCurrent(window);

	// Initialize glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}
	// Initialize the view port
	glViewport(0, 0, 800, 600);

	// registering resizing callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//capture mouse movements
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//registering mouse movements callback
	glfwSetCursorPosCallback(window, mouse_callback);

	//background
	glm::vec3 backgroundColor{0.2f, 0.3f, 0.3f};

	float vertices[] = {
		//vertex			//normal
		//back
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,

		//front
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,

		//left
		-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,

		//right
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,

		//bottom
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,  0.0f, 1.0f,

		//top
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  0.0f, 1.0f
	};

	unsigned vao;
	glGenVertexArrays(1, &vao);

	unsigned vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned lightVao;
	glGenVertexArrays(1, &lightVao);
	glBindVertexArray(lightVao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	Shader shaderProgram("../vertex.vs", "../fragment.fs");
	shaderProgram.use();
	shaderProgram.setFloat("ambientStrength", 0.1);
	shaderProgram.setFloat("specularStrength", 0.5);
	shaderProgram.setVec3("objectColor", 1.0f, 0.5f, 0.31f);

	Shader lampShader("../lamp.vs", "../lamp.fs");
	lampShader.use();


	glm::mat4 projection =
			glm::perspective(glm::radians(45.0f), (float)width/(float)height,
							 0.1f, 100.0f);

	glm::vec3 lampPosition(0.8f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);

	while(!glfwWindowShouldClose(window)) {
		//measuring deltaTime
		float currTime = glfwGetTime();
		deltaTime = currTime - lastFrame;
		lastFrame = currTime;

		processInput(window);

		glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Drawing the lamp
		glm::mat4 lampModel(1.0f);
		lampModel = glm::translate(lampModel, lampPosition);
		lampModel = glm::scale(lampModel, glm::vec3(0.1f));
		glm::mat4 view;
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glm::mat4 transformation = projection*view*lampModel;
		lampShader.use();
		unsigned transfLoc = glGetUniformLocation(lampShader.ID, "transformation");
		glUniformMatrix4fv(transfLoc, 1, GL_FALSE,
									   glm::value_ptr(transformation));
		unsigned lampPositionLoc = glad_glGetUniformLocation(shaderProgram.ID, "lampPosition");

		lampShader.setVec3("lampColor", lightColor);

		glBindVertexArray(lightVao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// Drawing the test object
		glm::mat4 model(1.0f);

		transformation = projection*view*model;
		shaderProgram.use();
		transfLoc =
				glGetUniformLocation(shaderProgram.ID, "transformation");
		glUniformMatrix4fv(transfLoc, 1, GL_FALSE,
						   glm::value_ptr(transformation));
		unsigned modelLoc =
				glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE,
						   glm::value_ptr(model));

		shaderProgram.setVec3("lightPosition", lampPosition);
		shaderProgram.setVec3("lightColor", lightColor);
		shaderProgram.setVec3("cameraPos", cameraPos);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		lampPosition.x = cos(currTime/2);
		lampPosition.z = sin(currTime/2);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteVertexArrays(1, &lightVao);
	glDeleteBuffers(1, &vbo);

	glfwTerminate();
	return 0;
}
