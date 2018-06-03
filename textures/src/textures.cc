#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

// This is the callback function called when the user resize the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Processes user inputs
void processInput(GLFWwindow *window, float backgroundColor[])
{
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
    }
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
    GLFWwindow* window = glfwCreateWindow(800, 600, "Uniforms", NULL, NULL);
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

    float backgroundColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
    float vertices[] = {
        //position              //color                 //texture
        -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,       0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f,      0.0f, 1.0f, 0.0f,       0.0f, 2.0f,
        0.5f, 0.5f, 0.0f,       0.0f, 0.0f, 1.0f,       2.0f, 2.0f,
        0.5f, -0.5f, 0.0f,      0.0f, 0.0f, 0.0f,       2.0f, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 2,        //first triangle
        2, 3, 0         //second triangle
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

    //element buffer object
    unsigned int ebo;
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //linking vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //linking and enabling color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

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
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    Shader shaderProgram("../src/vertex_shader.vs", "../src/fragment_shader.fs");

    shaderProgram.use();
    glUniform1i(glGetUniformLocation(shaderProgram.ID, "ourTexture1"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram.ID, "ourTexture2"), 1);

    // render loop
    while(!glfwWindowShouldClose(window)) {
        // process input
        processInput(window, backgroundColor);

        //rendering commands (we want to execute them for every frame)
        glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);   //state settin function
        glClear(GL_COLOR_BUFFER_BIT);   //state using function

        //drowing our triangle
        shaderProgram.use();
        float currTime = glfwGetTime();
        float mixValue = (cos(currTime)/2) + 0.5f;
        shaderProgram.setFloat("mixValue", mixValue);

        //using texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);   //unbinding the VAO

        //swap the front and back buffers
        glfwSwapBuffers(window);
        // polls for occurred events
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    //free resources
    glfwTerminate();
    return 0;
}
