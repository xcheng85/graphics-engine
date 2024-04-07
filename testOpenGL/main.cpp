#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <array>
#include <iostream>
#include <initializer_list>
#include <functional>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.h"
#include "vector.h"
#include "matrix.h"
#include "fp.h"
#include "quaternion.h"

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

// local space
// pos + color + tex
constexpr float vertices[] = {
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};

// world position of center of each cube
vec3f cubePositions[] = {
    vec3f(std::array{0.0f, 0.0f, 0.0f}),
    vec3f(std::array{2.0f, 5.0f, -15.0f}),
    vec3f(std::array{-1.5f, -2.2f, -2.5f}),
    vec3f(std::array{-3.8f, -2.0f, -12.3f}),
    vec3f(std::array{2.4f, -0.4f, -3.5f}),
    vec3f(std::array{-1.7f, 3.0f, -7.5f}),
    vec3f(std::array{1.3f, -2.0f, -2.5f}),
    vec3f(std::array{1.5f, 2.0f, -2.5f}),
    vec3f(std::array{1.5f, 0.2f, -1.5f}),
    vec3f(std::array{-1.3f, 1.0f, -1.5f})};

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

    // vertex inputs
    // ndc space:
    // float vertices[] = {
    //     0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    //     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    //     -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    //     -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f};

    // unsigned int indices[] = {
    //     0, 1, 3,
    //     1, 2, 3};

    // // // position and color
    // // float vertices[] = {
    // //     0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
    // //     -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    // //     0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f};

    unsigned int VBO;
    // VAO includes all the vertex attribute buffer (<=16) + element buffer
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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
    data = stbi_load("textures/dame.jpeg", &width, &height, &numChannels, 0);
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

    vec3f pod{std::array<float, 3>{0.f, 1.f, 1.f}};
    cout << pod << "\n";
    pod.normalize();
    cout << pod << "\n";
    // vector arithmatic
    pod = pod - std::array<float, 3>{-1.f, 1.f, 0.f};
    cout << pod << "\n";
    {
        // testing fp
        const auto cosVector = functor1<vec, float, 3, 16>::call(cos, pod);
        cout << cosVector << "\n";
    }
    {
        // testing quaternion
        quatf quat = QuaternionFromAxisAngle(vec3f(std::array<float, 3>{0.f, 1.f, 0.f}), 0.588f);
        const auto axis = RotationAxisFromQuaternion(quat);
        const auto angle = RotationAngleFromQuaternion(quat);
        cout << axis << "\n";
        cout << angle << "\n";
    }

    mat4x4f identity(std::array<float, 4 * 4>{
        1.f,
        0.f,
        0.f,
        0.f,
        0.f,
        1.f,
        0.f,
        0.f,
        0.f,
        0.f,
        1.f,
        0.f,
        0.f,
        0.f,
        0.f,
        1.f,
    });
    cout << identity << "\n";
    // auto type deduction
    auto m2 = MatrixMultiply4x4(identity, MatrixScale4x4(2.f, 3.f, 4.f));
    cout << m2 << "\n";

    // rotation on canonical basis
    auto rotx = MatrixMultiply4x4(MatrixRotationX4x4(0.5f), MatrixRotationX4x4(-0.5f));
    cout << rotx << "\n";
    auto roty = MatrixMultiply4x4(MatrixRotationY4x4(0.5f), MatrixRotationY4x4(-0.5f));
    cout << roty << "\n";
    auto rotz = MatrixMultiply4x4(MatrixRotationZ4x4(0.5f), MatrixRotationZ4x4(-0.5f));
    cout << rotz << "\n";

    {
        // validate rotation x
        auto mat1 = MatrixRotationAxis4x4(vec3f(std::array<float, 3>{1.f, 0, 0}), (float)M_PI / 4);
        auto mat2 = MatrixRotationX4x4(M_PI / 4);

        cout << "mat1: \n"
             << mat1 << "\n";
        cout << "mat2: \n"
             << mat2 << "\n";
    }

    // cout <<  std::invoke(std::cos, 0.f) << "\n";
    // cout <<  cos(0.f) << "\n";
    // [](){ print_num(42) };

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        update();
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // glDeleteVertexArrays(1, &VAO);
    // glDeleteBuffers(1, &VBO);
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    g_ShaderProgram.activate();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    double timeValue = glfwGetTime();
    float intensity = static_cast<float>(sin(timeValue) / 2.0 + 0.5);
    g_ShaderProgram.setVec4("intensity", intensity, intensity, intensity, intensity);

    mat4x4f identity(1.0f);
    // +20.f, move away, zoom out
    auto view1 = MatrixMultiply4x4(MatrixTranslation4x4(0.0f, 0.0f, 20.f), identity);
    cout << view1 << "\n";

    vec3f cameraPos(std::array{0.0f, 0.0f, 20.f});
    vec3f cameraLookatTarget(std::array{0.0f, 0.0f, 0.f});
    vec3f cameraWorldUp(std::array{1.0f, 0.0f, 0.f});
    auto view = ViewTransformLH4x4(cameraPos, cameraLookatTarget, cameraWorldUp);

    cout << view << "\n";

    auto persPrj = PerspectiveProjectionTransformLH(0.1f, 100.0f, 0.5f, (float)SCR_WIDTH / (float)SCR_HEIGHT);
    g_ShaderProgram.setMat4("view", view);
    g_ShaderProgram.setMat4("projection", persPrj);

    glBindVertexArray(VAO);
    for (unsigned int i = 0; i < 10; i++)
    {
        auto t = MatrixMultiply4x4(MatrixTranslation4x4(
                                           cubePositions[i][COMPONENT::X],
                                           cubePositions[i][COMPONENT::Y],
                                           cubePositions[i][COMPONENT::Z]),
                                       identity);
        float angle = 0.24f * i;
        auto r = MatrixRotationAxis4x4(vec3f(std::array{1.0f, 0.3f, 0.5f}), angle);
        auto model = MatrixMultiply4x4(r, t);

        g_ShaderProgram.setMat4("model", model);
        // 6 * 6
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
}