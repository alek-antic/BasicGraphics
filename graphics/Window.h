#ifndef _WINDOW_H_
#define _WINDOW_H_

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <memory>

#include "shader.h"
#include "Terrain.h"
#include "PointLight.h"
#include "Camera.h"


class Window
{
public:
	static int width;
	static int height;
	static const char* windowTitle;
    
    static bool keys[1024];
    
	static glm::mat4 projection;
	
    static glm::vec2 lastPoint;
    
    static Camera * camera;
    static Terrain * terrain;
    static PointLight * light;
    
    static GLuint shader;
   
    static double fov;

	static bool initializeObjects();
	static void cleanUp();
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);
	static void idleCallback(float delta);
	static void displayCallback(GLFWwindow*);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursorCallback(GLFWwindow * window, double xpos, double ypos);
};

#endif
