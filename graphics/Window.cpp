#include "Window.h"
#include <algorithm>

int Window::width;
int Window::height;

const char* Window::windowTitle = "Toon Terrain, seed: %d";

bool Window::keys[1024];

glm::mat4 Window::projection; // Projection matrix.
const float nearPlaneDist = 1.0;
const float farPlaneDist = 2500.0;

glm::vec2 Window::lastPoint;

Camera * Window::camera;
Terrain * Window::terrain;
PointLight * Window::light;

GLuint Window::shader;

int seed = 0;
int useToon = 1;
GLuint toonLoc;

double Window::fov = 45.0;

bool Window::initializeObjects()
{
    camera = new Camera(glm::vec3(0.0f, 1.0f, 0.0f));
    light = new PointLight(glm::vec3(0.0f, 500.0f, 0.0f), glm::vec3(0.9f, 0.9f, 0.9f));
    seed = time(0);
    
    terrain = new Terrain(1575888131);
    shader = terrain->getShader();
    
    toonLoc = glGetUniformLocation(shader, "useToon");
    
    return true;
}

void Window::cleanUp()
{
    delete camera;
    delete terrain;
}

GLFWwindow* Window::createWindow(int width, int height)
{
    // Initialize GLFW.
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return NULL;
    }
    
    // 4x antialiasing.
    glfwWindowHint(GLFW_SAMPLES, 4);
    
    
#ifdef __APPLE__ 
    // Apple implements its own version of OpenGL and requires special treatments
    // to make it uses modern OpenGL.
    
    // Ensure that minimum OpenGL version is 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Enable forward compatibility and allow a modern OpenGL context
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    // Create the GLFW window.
    GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);
    
    // Check if the window could not be created.
    if (!window)
    {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return NULL;
    }
    
    // Make the context of the window.
    glfwMakeContextCurrent(window);
    
    
    
#ifndef __APPLE__
    // On Windows and Linux, we need GLEW to provide modern OpenGL functionality.
    
    // Initialize GLEW.
    if (glewInit())
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return NULL;
    }
#endif
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    
    // Set swap interval to 1.
    glfwSwapInterval(0);
    
    // Call the resize callback to make sure things get drawn immediately.
    Window::resizeCallback(window, width, height);
    
    return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
    // In case your Mac has a retina display.
    glfwGetFramebufferSize(window, &width, &height);
#endif
    Window::width = width;
    Window::height = height;
    // Set the viewport size.
    glViewport(0, 0, width, height);
    
    // Set the projection matrix.
    Window::projection = glm::perspective(glm::radians(fov),
                                          double(width) / (double)height, (double)nearPlaneDist, (double)farPlaneDist);
}

void Window::idleCallback(float delta)
{
    // update objects
    if (keys[GLFW_KEY_W]){
        Window::camera->keyEvent(FORWARD, delta);
    }

    if (keys[GLFW_KEY_A]){
        Window::camera->keyEvent(LEFT, delta);
    }
    
    if (keys[GLFW_KEY_S]){
        Window::camera->keyEvent(BACKWARD, delta);
    }
    
    if (keys[GLFW_KEY_D]){
        Window::camera->keyEvent(RIGHT, delta);
    }
    
    camera->update(shader);
    light->update(shader);
}

void Window::displayCallback(GLFWwindow* window)
{
    char title[128];
    sprintf(title, windowTitle, seed);
    glfwSetWindowTitle(window, title);
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    GLuint projLoc;
    
    projLoc = glGetUniformLocation(shader, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    terrain->draw();
    
    // Gets events, including input such as keyboard and mouse or window resizing.
    glfwPollEvents();
    // Swap buffers.
    glfwSwapBuffers(window);    
}



void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Check for a key press.
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
            case GLFW_KEY_ESCAPE:
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
                
            case GLFW_KEY_R:
                delete terrain;
                seed = time(0);
                terrain = new Terrain(seed);
            case GLFW_KEY_T:
                useToon = (useToon + 1) % 2;
                glUniform1i(toonLoc, useToon);
            default:
                Window::keys[key] = true;
                break;
        }
    }
    else if (action == GLFW_RELEASE) {
        Window::keys[key] = false;
    }
}



void Window::cursorCallback(GLFWwindow * window, double xpos, double ypos) {
    Window::camera->mouseEvent(xpos - Window::lastPoint.x, ypos - Window::lastPoint.y);
    Window::lastPoint = glm::vec2(xpos, ypos);
}
