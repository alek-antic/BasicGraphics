#ifndef Bezier_hpp
#define Bezier_hpp

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Geometry.h"
#include "Node.h"
#include <stdio.h>
#include <vector>

enum CTRL {P0, P1, P2, P3, NOP};

class Bezier : public Geometry {
private:
    glm::vec3 p0, p1, p2, p3;
    glm::vec3 a, b, c, d;
    CTRL currentPoint;
    bool pointsChanged;
    
    void calculateCoeff();
    void updatePoints();
public:
    Bezier(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
    ~Bezier();
    
    glm::vec3 getPoint(float t);
    void draw(glm::mat4 C);
    void update(glm::mat4 proj, glm::mat4 view, glm::vec3 camPos);
    
    void translatePt(glm::vec3 delta);
    void setControlPoint(CTRL p);
};

#endif /* Bezier_hpp */
