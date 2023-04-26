#ifndef Transform_h
#define Transform_h

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "Node.h"
#include <list>


class Transform : public Node {
private:
    glm::mat4 M;
    std::list<Node*> children;
    bool animator;
    float maxAngle;
    float curAngle;
    float delta;
    glm::vec3 axis;
    
    bool useDraw;
    
public:
    Transform(glm::mat4 M);
    Transform(float delta, glm::vec3 axis, float maxAngle);
    ~Transform();
    
    void addChild(Node * n);
    
    void draw(glm::mat4 C);
    void update(glm::mat4 proj, glm::mat4 view, glm::vec3 camPos);
    void setDraw(bool useDraw);
    
    void changeTransform(glm::mat4 M);
};


#endif /* Transform_h */
