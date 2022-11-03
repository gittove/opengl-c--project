
#include <iostream>

#include <glew.h>
#include <glfw3.h>


//theres ur shader:)
const char* vertexShaderSource1 = R"(#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
})";

const char* vertexShaderSource2 = R"(#version 330 core
layout (location = 2) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
})";


const char* fragShaderSource = R"(#version 330 core
out vec4 fragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
})";

bool fillShape;
bool spacePressedLastFrame = false;

void process_input(GLFWwindow* window)
{
    bool spacePressedThisFrame = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (!spacePressedLastFrame && spacePressedThisFrame)
    {
        if (fillShape)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            fillShape = false;
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            fillShape = true;
        }
    }

    spacePressedLastFrame = spacePressedThisFrame;
}

int main()
{
    glfwInit ();

    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow* window = glfwCreateWindow (480, 480, "OpenGL", NULL, NULL);
    glViewport (0, 0, 640, 640);
    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); });

    if (!window)
    {
        std::cout << "null window:/" << std::endl;
        glfwTerminate ();
        return -1;
    }

    glfwMakeContextCurrent (window);
    glfwSetWindowShouldClose (window, false);

    GLenum err = glewInit();

    if (GLEW_OK != err)
    {
        std::cout << "glew initialization failed:/" << std::endl;
    }

    unsigned int vertexShader1 = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader1, 1, &vertexShaderSource1, NULL);
    glCompileShader(vertexShader1);

    unsigned int vertexShader2 = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader2, 1, &vertexShaderSource2, NULL);
    glCompileShader(vertexShader2);

    unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragShaderSource, NULL);
    glCompileShader(fragShader);

    // link shaders
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader1);
    glAttachShader(shaderProgram, vertexShader2);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader1);
    glDeleteShader(vertexShader2);
    glDeleteShader(fragShader);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f, 
        0.0f, 0.5f, 0.0f,
    };

    unsigned int indices[] = {
        0, 1, 3,
        1,2,3
    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind vertex array object
    glBindVertexArray(VAO);

    // copy vertices array into a buffer
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        process_input(window);

        //use shader prgram when we want to render an object
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // rendering commands here :)

        // this swaps the color buffer (a large 2d bugger that contains color values for each pixel)
        // that is used too render during this render iteration, and show it as output on the screen (aka SDL present)
        glfwSwapBuffers(window);
        // checks if any input events are triggered, updates window state, calls functions (registered via callback methods)
        glfwPollEvents();
       
    }

    // deallocating resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate(); 
    return 0;
}

