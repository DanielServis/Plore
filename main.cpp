#include <iostream>
using namespace std;
#include <filesystem>
namespace fs = std::filesystem;
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

float delta_time = 0;
float last_time = 0.0f;

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

#ifndef SHADER_DIR
#define SHADER_DIR "."
#endif

    std::string vertex_path = std::string(SHADER_DIR) +"/shader_vertex.glsl";
    std::string fragment_path = std::string(SHADER_DIR) + "/shader_fragment.glsl";

    std::string vertexShaderSource = load_file(vertex_path.c_str());
    std::string fragmentShaderSource = load_file(fragment_path.c_str());
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

    std::string current_directory = "/home";
    int total = 0, directories = 0, files = 0, binaries = 0;
    Center *center = new Center(0, 0, 0, 0, 0, 0);
    Binary *binary_renders[256];
    std::string binary_names[256];
    File *file_renders[256];
    std::string file_names[256];
    Directory *directory_renders[256];
    std::string directory_names[256];
    const float selected_elevation = 1;
    float file_selected = -1;
    bool selecting = false;

    GLTtext *directory_text = gltCreateText();
    GLTtext *file_text = gltCreateText();

    float camera_x = 0, camera_y = 0;

    while (!glfwWindowShouldClose(window))
    {
        float current_time = glfwGetTime();
        delta_time = current_time - last_time;
        last_time = current_time;

        int fbWidth, fbHeight;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        glViewport(0, 0, fbWidth, fbHeight);

        glClearColor(0.133f, 0.141f, 0.212f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        gltSetText(directory_text, current_directory.c_str());

        gltBeginDraw();
        gltColor(1.0f, 1.0f, 1.0f, 1.0f);
        gltDrawText2D(directory_text, 10.0f, 30.0f, 1.5f);
        gltDrawText2D(file_text, 10.0f, 60.0f, 1.5f);
        gltEndDraw();

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
            free(directory_renders[i]);
            directory_renders[i] = nullptr;
        }

        for (int i = 0; i < files; i++)
        {
            free(file_renders[i]);
            file_renders[i] = nullptr;
        }

        for (int i = 0; i < binaries; i++)
        {
            free(binary_renders[i]);
            binary_renders[i] = nullptr;
        }

        total = 0;
        directories = 0;
        files = 0;
        binaries = 0;

        for (const auto &entry : fs::directory_iterator(current_directory))
        {
            total++;
            if (entry.is_directory())
            {
                directory_names[directories] = entry.path().filename().string();
                directories++;
            }
            else if (is_binary(entry.path().string()))
            {
                binary_names[binaries] = entry.path().filename().string();
                binaries++;
            }
            else if (entry.is_regular_file())
            {
                file_names[files] = entry.path().filename().string();
                files++;
            }
        }

        const float spacing = 1.5f;
        const float startDist = 3.0f;

        auto trianglePos = [&](int i, float yaw, float &x, float &y, float &z)
        {
            int row = (int)((sqrtf(8.0f * i + 1.0f) - 1.0f) / 2.0f);
            int col = i - row * (row + 1) / 2;

            float lx = (col - row / 2.0f) * spacing;
            float lz = startDist + row * spacing;

            float c = cosf(yaw), s = sinf(yaw);
            x = lx * c + lz * s;
            z = -lx * s + lz * c;

            float t = (float)i / 25.0f;
            y = -(t * t);
        };

        const float rotation_speed = 0.5;
        static float rotation_modifier = 0.0;
        rotation_modifier += rotation_speed * delta_time;

        for (int i = 0; i < directories; i++)
        {
            float x, y, z;
            trianglePos(i, 0.0f + rotation_modifier, x, y, z);
            directory_renders[i] = new Directory(x, y, z, 0, 0.0f + rotation_modifier, 0);
        }

        for (int i = 0; i < files; i++)
        {
            float x, y, z;
            trianglePos(i, -2.1f + rotation_modifier, x, y, z);
            file_renders[i] = new File(x, y, z, 0, -2.1f + rotation_modifier, 0);
        }

        for (int i = 0; i < binaries; i++)
        {
            float x, y, z;
            trianglePos(i, 2.1f + rotation_modifier, x, y, z);
            binary_renders[i] = new Binary(x, y, z, 0, 2.1f + rotation_modifier, 0);
        }

        if (Input::get_key_down(GLFW_KEY_LEFT_SHIFT))
        {
            static float cooldown = 0.0;
            cooldown -= delta_time;

            if (cooldown <= 0)
            {
                file_selected++;
                cooldown = 0.05;
            }

            if (file_selected > (binaries + files + directories))
            {
                file_selected = 0;
            }
        }
        
        if (Input::get_key_down(GLFW_KEY_LEFT_CONTROL))
        {
            static float cooldown = 0.0;
            cooldown -= delta_time;

            if (cooldown <= 0)
            {
                file_selected--;
                cooldown = 0.05;
            }

            if (file_selected < 0)
            {
                file_selected = binaries + files + directories;
            }
        }

        static bool previous_selection = false;
        static bool lock_selection = false;
        bool current_selection = Input::get_key_down(GLFW_KEY_ENTER);
        if (current_selection && !previous_selection)
        {
            selecting = true;

            lock_selection = !lock_selection;
        }
        previous_selection = current_selection;

        static bool previous_return = false;
        static bool lock_return = false;
        bool current_return = Input::get_key_down(GLFW_KEY_ESCAPE);
        if (current_return && !previous_return)
        {
            for (int i = current_directory.length() - 1; i >= 0; i--)
            {
                if (current_directory.c_str()[i] == '/')
                {
                    current_directory.pop_back();

                    if (current_directory.empty())
                    {
                        gltTerminate();
                        glfwTerminate();
                        return 0;
                    }

                    break;
                }
                else
                {
                    current_directory.pop_back();
                }
            }

            lock_return = !lock_return;
        }
        previous_return = current_return;

        center->get_transform()->yaw += 0.1;
        center->draw(modelLoc, objectTypeLoc);

        for (int i = 0; i < directories; i++)
        {
            if (i == file_selected)
            {
                directory_renders[i]->get_transform()->y += selected_elevation;

                gltSetText(file_text, directory_names[i].c_str());

                if (selecting)
                {
                    directory_renders[i]->get_transform()->y += selected_elevation;
                    current_directory.append("/" + directory_names[i]);
                    file_selected = -1;
                    selecting = false;
                }
            }

            directory_renders[i]->draw(modelLoc, objectTypeLoc);
        }

        for (int i = 0; i < files; i++)
        {
            if (i == (file_selected - directories))
            {
                file_renders[i]->get_transform()->y += selected_elevation;

                gltSetText(file_text, file_names[i].c_str());

                if (selecting)
                {
                    std::string command = "cd " + current_directory + " && nvim " + file_names[i];
                    std::system(command.c_str());
                    selecting = false;
                }
            }

            file_renders[i]->draw(modelLoc, objectTypeLoc);
        }

        for (int i = 0; i < binaries; i++)
        {
            if (i == (file_selected - directories - files))
            {
                binary_renders[i]->get_transform()->y += selected_elevation;

                gltSetText(file_text, binary_names[i].c_str());

                if (selecting)
                {
                    std::string command = "cd " + current_directory + " && ./" + binary_names[i];
                    std::system(command.c_str());
                    selecting = false;
                }
            }

            binary_renders[i]->draw(modelLoc, objectTypeLoc);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    gltTerminate();
    glfwTerminate();
    return 0;
}
