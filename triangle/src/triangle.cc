#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

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
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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
    // float vertices[] = {
    //     -0.5f, -0.5f, 0.0f, //bottom left
    //      -0.5f, 0.5f, 0.0f, //top left
    //      0.5f,  0.5f, 0.0f, //top right
    //      0.5f, -0.5f, 0.0f  //bottom right
    // };
    float vertices[] = {
        -1.0f, -0.5f, 0.0f,
        0.0f, -0.5f, 0.0f,
        1.0f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f
    };

    unsigned int *indices[2];
    unsigned int verticesSet1[] = {
        0, 1, 3     //first triangle
    };
    unsigned int verticesSet2[] = {
        1, 2, 4     //second triangle
    };
    indices[0] = verticesSet1;
    indices[1] = verticesSet2;

    //vertex attribute object
    unsigned int vao[2];
    glGenVertexArrays(2, vao);

    glBindVertexArray(vao[0]);

    // Vertex buffer object
    unsigned int vbo[2];
    glGenBuffers(2, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //element buffer object
    unsigned int ebo[2];
    glGenBuffers(2, ebo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*sizeof(unsigned int), indices[0], GL_STATIC_DRAW);

    //linking vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // creating second VAO
    glBindVertexArray(vao[1]);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*sizeof(unsigned int), indices[1], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //vertex shader
    const char *vertexShaderSource = "#version 330 core\nlayout (location = 0) in vec3 aPos;\nvoid main()\n{\ngl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n}";
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    //attaching code to the shader obj and compiling it
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //checking compilation status
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
        return -1;
    }

    //fragment Shader
    const char *fragmentShaderSource1 = "#version 330 core\nout vec4 FragColor;\nvoid main()\n{\nFragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n}";
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource1, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
        return -1;
    }

    //creating the shader program (it links the shaders toghether)
    unsigned int shaderProgram[2];
    shaderProgram[0] = glCreateProgram();
    glAttachShader(shaderProgram[0], vertexShader);
    glAttachShader(shaderProgram[0], fragmentShader);
    glLinkProgram(shaderProgram[0]);

    //checking linking status
    glGetProgramiv(shaderProgram[0], GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram[0], 512, NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
        return -1;
    }

    // creating a second shader program
    const char *fragmentShaderSource2 = "#version 330 core\nout vec4 FragColor;\nvoid main()\n{\nFragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n}";
    glShaderSource(fragmentShader, 1, &fragmentShaderSource2, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
        return -1;
    }

    shaderProgram[1] = glCreateProgram();
    glAttachShader(shaderProgram[1], vertexShader);
    glAttachShader(shaderProgram[1], fragmentShader);
    glLinkProgram(shaderProgram[1]);
    //checking linking status
    glGetProgramiv(shaderProgram[1], GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram[1], 512, NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
        return -1;
    }
    
    //we can remove the shader objects now
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // render loop
    while(!glfwWindowShouldClose(window)) {
        // process input
        processInput(window, backgroundColor);

        //rendering commands (we want to execute them for every frame)
        glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);   //state settin function
        glClear(GL_COLOR_BUFFER_BIT);   //state using function

        //drowing our triangle
        glUseProgram(shaderProgram[0]);
        glBindVertexArray(vao[0]);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);   //unbinding the VAO

        glUseProgram(shaderProgram[1]);
        glBindVertexArray(vao[1]);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        //swap the front and back buffers
        glfwSwapBuffers(window);
        // polls for occurred events
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(2, vao);
    glDeleteBuffers(2, vbo);
    glDeleteBuffers(2, ebo);

    //free resources
    glfwTerminate();
    return 0;
}
