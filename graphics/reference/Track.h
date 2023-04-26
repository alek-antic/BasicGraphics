#ifndef Track_hpp
#define Track_hpp

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Geometry.h"
#include "Bezier.h"

#include <stdio.h>

class Track : public Geometry {
private:
    Bezier * curves[8];
    int currentControlPt;
    int currentPt;
    
public:
    Track();
    ~Track();
    
    void draw(glm::mat4 C);
    
    void nextCtrlPoint();
    void prevCtrlPoint();
    
    void translatePt(glm::vec3 delta);
    
    void update(glm::mat4 proj, glm::mat4 view, glm::vec3 camPos);
    
    glm::vec3 advancePoint();
};


#endif /* Track_hpp */
