#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <camera.h>

using namespace std;

// This is the callback function called when the user resize the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastMouseX, lastMouseY;
float yaw, pitch;

bool firstMouse = true;

// Processes user inputs
void processInput(GLFWwindow *window, float backgroundColor[], Shader shaderProgram, unsigned int width, unsigned int height)
{
    float cameraSpeed = 1.0f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    } else if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
        backgroundColor[0] = 0.0f;
        backgroundColor[1] = 0.0f;
        backgroundColor[2] = 0.0f;
        backgroundColor[3] = 1.0f;
    } else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        backgroundColor[0] = 0.2f;
        backgroundColor[1] = 0.3f;
        backgroundColor[2] = 0.3f;
        backgroundColor[3] = 1.0f;
    } else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        //ortographic projection matrix
        cout << "Switched to orthographic projection" << endl;
        shaderProgram.use();
        glm::mat4 ortho = glm::ortho(0.0f, (float)width, 0.0f, (float)height, 0.1f, 100.0f);
        unsigned int projLocation = glGetUniformLocation(shaderProgram.ID, "projection");
        glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(ortho));
    } else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        //perspective projection matrix
        cout << "Switched to perspective projection" << endl;
        shaderProgram.use();
        glm::mat4 pers = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);
        unsigned int projLocation = glGetUniformLocation(shaderProgram.ID, "projection");
        glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(pers));
    } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += cameraSpeed*cameraFront;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= cameraSpeed*cameraFront;
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos -= cameraSpeed*glm::normalize(glm::cross(cameraFront, cameraUp));
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += cameraSpeed*glm::normalize(glm::cross(cameraFront, cameraUp));
    }
}

//called when the mouse moves. It updates the view direction
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse) {
        lastMouseX = xpos;
        lastMouseY = ypos;
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

void usage()
{
    const char *s = "available commands\n\t0/1: change background color\n\tf/p: change polygon filling mode\n\tESC: close program\n";
    cout << s;
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Camera Navigation", NULL, NULL);
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

    usage();

    // Initialize the view port
    glViewport(0, 0, 800, 600);

    // registering resizing callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //capture mouse movements
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //registering mouse movements callback
    glfwSetCursorPosCallback(window, mouse_callback);

    float backgroundColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
      glm::vec3( 0.0f,  0.0f,  0.0f),
      glm::vec3( 2.0f,  5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3( 2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f,  3.0f, -7.5f),
      glm::vec3( 1.3f, -2.0f, -2.5f),
      glm::vec3( 1.5f,  2.0f, -2.5f),
      glm::vec3( 1.5f,  0.2f, -1.5f),
      glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    //vertex attribute object
    unsigned int vao;
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    // Vertex buffer object
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //linking vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //creating texture object
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    //activate the first texture unit
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    //setting texture wrapping mode and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //generate texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load("../data/container.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    } else {
        cout << "Error loading texture" << endl;
    }

    glGenTextures(1, &texture2);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    //setting texture wrapping mode and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //generate texture
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("../data/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    } else {
        cout << "Error loading texture" << endl;
    }

    //linking and enabling texture coordinates attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    Shader shaderProgram("../src/vertex_shader.vs", "../src/fragment_shader.fs");

    shaderProgram.use();
    glUniform1i(glGetUniformLocation(shaderProgram.ID, "ourTexture1"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram.ID, "ourTexture2"), 1);

    //perspective projection matrix
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);
    unsigned int projLocation = glGetUniformLocation(shaderProgram.ID, "projection");
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projection));

    //enable depth test
    glEnable(GL_DEPTH_TEST);

    float cameraAngle = 0.0f;
    float deltaAngle = 1.0f;
    // render loop
    while(!glfwWindowShouldClose(window)) {
        //measuring deltaTime
        float currTime = glfwGetTime();
        deltaTime = currTime - lastFrame;
        lastFrame = currTime;

        // process input
        processInput(window, backgroundColor, shaderProgram, width, height);

        //rendering commands (we want to execute them for every frame)
        glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);   //state settin function
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   //state using function

        //drowing our triangle
        shaderProgram.use();

        //model matrix
        glm::mat4 model, view;
        //view matrix
        //cameraAngle += deltaAngle;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        unsigned int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        //using texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(vao);
        for (size_t i = 0; i < 10; i++) {
            model = glm::translate(glm::mat4(1.0f), cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(0.5f, 0.5f, 0.5f));
            //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
            unsigned int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);   //unbinding the VAO

        //swap the front and back buffers
        glfwSwapBuffers(window);
        // polls for occurred events
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    //free resources
    glfwTerminate();
    return 0;
}
