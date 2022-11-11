
#include <glew.h>
#include <glfw3.h>
#include <filesystem>

#include "shader.h"
#include "stb_image.h"

#include <iostream>

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

    auto root = std::filesystem::current_path();
    auto vertFilePath = std::filesystem::path{ "shaders/shader.vs" };
    auto fragFilePath = std::filesystem::path{ "shaders/shader.fs" };

    std::string vertString = (root / vertFilePath).string();
    std::string fragString = (root / fragFilePath).string();

    const char* vertPath = vertString.c_str();
    const char* fragPath = fragString.c_str();

    Shader myShader(vertPath, fragPath);

    float vertices[] = {
        0.5f, 0.5f, 0.0f,     1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
        0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f,    0.0f, 0.0f, 1.0f,    0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;

    auto textureFilePath = std::filesystem::path{ "textures/container.jpg" };
    std::string texString = (root / textureFilePath).string();
    const char* texPath = texString.c_str();

    unsigned char* data = stbi_load(texPath, &width, &height, &nrChannels, 4);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        stbi_failure_reason();
        std::cout << "failed to load texture" << std::endl;
    }

    stbi_image_free(data);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        process_input(window);

        glBindTexture(GL_TEXTURE_2D, texture);
        //use shader prgram when we want to render an object
        myShader.use();
        glBindVertexArray(VAO);


        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // this swaps the color buffer (a large 2d bugger that contains color values for each pixel)
        // that is used too render during this render iteration, and show it as output on the screen (aka SDL present)
        glfwSwapBuffers(window);
        // checks if any input events are triggered, updates window state, calls functions (registered via callback methods)
        glfwPollEvents();
    }

    // deallocating resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(myShader.ID);

    glfwTerminate(); 
    return 0;
}

