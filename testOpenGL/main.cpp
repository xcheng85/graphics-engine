#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.h"

void update();
void render();
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

constexpr uint16_t SCR_WIDTH = 1920;
constexpr uint16_t SCR_HEIGHT = 1080;
constexpr char *vertexShaderSource =
    "#version 460 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
constexpr char *fragmentShaderSource =
    "#version 460 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = ourColor;\n"
    "}\n\0";
// GLuint shaderProgramHandle;
Shader g_ShaderProgram;
unsigned int VAO;
unsigned int texture1, texture2;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Test OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    g_ShaderProgram = Shader("shaders/test.vert", "shaders/test.frag");

    // // shader
    // const auto vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);
    // glShaderSource(vertexShaderHandle, 1, &vertexShaderSource, NULL);
    // glCompileShader(vertexShaderHandle);

    // int success;
    // constexpr uint16_t byteSize = 512;
    // char infoLog[byteSize];
    // glGetShaderiv(vertexShaderHandle, GL_COMPILE_STATUS, &success);
    // if (!success)
    // {
    //     glGetShaderInfoLog(vertexShaderHandle, byteSize, nullptr, infoLog);
    //     std::cerr << "Vertex Shader Compilation Error: \n"
    //               << infoLog << std::endl;
    // }
    // const auto fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
    // glShaderSource(fragmentShaderHandle, 1, &fragmentShaderSource, NULL);
    // glCompileShader(fragmentShaderHandle);
    // glGetShaderiv(fragmentShaderHandle, GL_COMPILE_STATUS, &success);
    // if (!success)
    // {
    //     glGetShaderInfoLog(fragmentShaderHandle, byteSize, nullptr, infoLog);
    //     std::cout << "Fragment Shader Compilation Error: \n"
    //               << infoLog << std::endl;
    // }
    // shaderProgramHandle = glCreateProgram();
    // glAttachShader(shaderProgramHandle, vertexShaderHandle);
    // glAttachShader(shaderProgramHandle, fragmentShaderHandle);
    // glLinkProgram(shaderProgramHandle);
    // glGetProgramiv(shaderProgramHandle, GL_LINK_STATUS, &success);
    // if (!success)
    // {
    //     glGetProgramInfoLog(shaderProgramHandle, byteSize, nullptr, infoLog);
    //     std::cout << "Shader Program Linking Error: \n"
    //               << infoLog << std::endl;
    // }
    // glDeleteShader(vertexShaderHandle);
    // glDeleteShader(fragmentShaderHandle);

    // vertex inputs
    // ndc space:
    float vertices[] = {
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f};
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3};

    // // position and color
    // float vertices[] = {
    //     0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
    //     -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    //     0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f};

    unsigned int VBO, EBO;
    // VAO includes all the vertex attribute buffer (<=16) + element buffer
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    // glEnableVertexAttribArray(0);

    // interleave
    // two vertex attributes
    // position attribute: slot 0
    // stride = 3 + 3 + 2
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute: slot 1
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute: slot2, uv
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // textures
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, numChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("textures/dame.jpeg", &width, &height, &numChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture\n";
    }
    stbi_image_free(data);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("textures/giannis.jpeg", &width, &height, &numChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    g_ShaderProgram.activate();
    g_ShaderProgram.setInt("inTexture0", 0);
    g_ShaderProgram.setInt("inTexture1", 1);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        update();
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

// resize ?
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void update()
{
}

void render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    g_ShaderProgram.activate();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    double timeValue = glfwGetTime();
    float intensity = static_cast<float>(sin(timeValue) / 2.0 + 0.5);
    g_ShaderProgram.setVec4("intensity", intensity, intensity, intensity, intensity);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
}