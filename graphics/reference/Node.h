#ifndef Node_h
#define Node_h

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>

class Node {
public:
    virtual void draw(glm::mat4 C) = 0;
    virtual void update(glm::mat4 proj, glm::mat4 view, glm::vec3 camPos) = 0;
};
#endif /* Node_h */
