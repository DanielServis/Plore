#include <iostream>
using namespace std;
#include <cstdlib>
#include <unistd.h>
#include <cmath>
#include <ctime>
#include <cstring>
#include <fstream>
#include <string>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLT_IMPLEMENTATION
#include "../gltext.h"
#include "../glm//glm/glm.hpp"
#include "../glm/glm/gtc/type_ptr.hpp"

#include "fops.hpp"
#include "input.hpp"
#include "models.hpp"
#include "objects.hpp"

int main()
{
    if (!glfwInit())
    {
        std::cerr << "failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(1000, 1000, "explorer", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        return -1;
    }

    if (!gltInit())
    {
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::string vertexShaderSource = load_file("shader_vertex.glsl");
    std::string fragmentShaderSource = load_file("shader_fragment.glsl");
    if (vertexShaderSource.empty() || fragmentShaderSource.empty())
    {
        return -1;
    }

    const char *vertSrc = vertexShaderSource.c_str();
    const char *fragSrc = fragmentShaderSource.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertSrc, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[1000];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 1000, NULL, infoLog);
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragSrc, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 1000, NULL, infoLog);
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 1000, NULL, infoLog);
        printf("Linking Error : %s\n", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    unsigned int objectTypeLoc = glGetUniformLocation(shaderProgram, "ID");

    glfwSetKeyCallback(window, Input::key_callback);
    glfwSetMouseButtonCallback(window, Input::mouse_button_callback);

    File *file_renders[256];
    Directory *directory_renders[256];

    std::string current_directory = "";

    std::string output = run_command("cd " + current_directory + "&& ls -1p");

    int total = 0, directories = 0, files = 0;
    std::istringstream ss(output);
    std::string line;
    while (std::getline(ss, line))
    {
        printf(".");

        if (line.empty())
            continue;
        total++;
        if (line.back() == '/')
            directories++;
        else
            files++;
    }

    std::cout << "&" << directories << ", " << files << std::endl;

    for (int i = 0; i < directories; i++)
    {
        directory_renders[i] = new Directory(-5 + (i * 1.5), 0, 1);
    }

    for (int i = 0; i < files; i++)
    {
        file_renders[i] = new File(-5 + (i * 1.5), 0, -1);
    }

    float camera_x = 0, camera_y = 0;

    while (!glfwWindowShouldClose(window))
    {
        int fbWidth, fbHeight;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        glViewport(0, 0, fbWidth, fbHeight);

        glClearColor(0.133f, 0.141f, 0.212f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);



        if (Input::get_key_down(GLFW_KEY_LEFT))
        {
            camera_x += 0.2;
        }
        else if (Input::get_key_down(GLFW_KEY_RIGHT))
        {
            camera_x -= 0.2;
        }

        if (Input::get_key_down(GLFW_KEY_UP))
        {
            camera_y += 0.2;
        }
        else if (Input::get_key_down(GLFW_KEY_DOWN))
        {
            camera_y -= 0.2;
        }

        float fov = 45.0f * 3.14159f / 180.0f;
        float aspect = fbHeight > 0 ? (float)fbWidth / (float)fbHeight : 1.0f;
        float near = 0.1f;
        float far = 1000.0f;

        glm::mat4 view = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f + camera_x, -20.0f, -20.0f + camera_y));

        glm::mat4 projection = glm::perspective(fov, aspect, near, far);

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));



        

        

        for (int i = 0; i < directories; i++)
        {
            directory_renders[i]->draw(modelLoc, objectTypeLoc);
        }

        for (int i = 0; i < files; i++)
        {
            file_renders[i]->draw(modelLoc, objectTypeLoc);
        }



        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    gltTerminate();
    glfwTerminate();
    return 0;
}
