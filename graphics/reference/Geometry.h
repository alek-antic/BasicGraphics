#include "Node.h"

#ifndef Geometry_h
#define Geometry_h

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

#include "shader.h"

class Geometry : public Node {
protected:
    std::vector<glm::vec3> points;
    std::vector<glm::vec3> norms;
    std::vector<unsigned int> pointInds;
    GLuint VAO, vertexVBO, normalVBO, vertexEBO, normalEBO;
    
    GLuint program;
    glm::vec3 color;
    
    GLenum drawingMode;
    
public:
    static Geometry * init(std::string filename, GLenum drawingMode);
    
    ~Geometry();
    
    void draw(glm::mat4 C);
    void update(glm::mat4 proj, glm::mat4 view, glm::vec3 camPos);
    void setColor(glm::vec3 color);
};

#endif /* Geometry_h */
