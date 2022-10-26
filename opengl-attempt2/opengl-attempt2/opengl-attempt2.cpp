
#include <iostream>
#include <glfw3.h>
#include <glew.h>

void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main()
{
    glfwInit ();
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    float vertices[] = { 
        -0.5f, -0.5f, 0.0f, 
        0.5f, -0.5f, 0.0f,
        0.0f, .05f, 0.0f };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    GLFWwindow* window = glfwCreateWindow (640, 480, "OpenGL", NULL, NULL);
    glViewport (0, 0, 640, 400);

    if (window == NULL)
    {
        std::cout << "null window:/" << std::endl;
        glfwTerminate ();
        return -1;
    }

    glfwMakeContextCurrent (window);
    glfwSetWindowShouldClose (window, false);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        process_input(window);

        // rendering commands here :)

        // this swaps the color buffer (a large 2d bugger that contains color values for each pixel)
        // that is used too render during this render iteration, and show it as output on the screen (aka SDL present)
        glfwSwapBuffers(window);
        // checks if any input events are triggered, updates window state, calls functions (registered via callback methods)
        glfwPollEvents();
       
    }

    glfwTerminate(); 
    return 0;
}

