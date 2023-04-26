//
//  Camera.hpp
//  graphics
//
//  Created by Alek Antic on 12/5/19.
//  Copyright © 2019 Alek Antic. All rights reserved.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include "Terrain.h"

enum Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW = 90.0f;
const float PITCH = 0.0f;
const float speed = 75.0f;
const float sensitivity = 0.25f;

class Camera {
protected:
    glm::vec3 pos, dir, up, right;
    glm::vec3 world_up;
    glm::mat4 cam;
    float yaw, pitch;
    
    
private:
    
    void updateVecs();
    
public:
    Camera(glm::vec3 global_up);
    
    glm::mat4 getView();
    
    void keyEvent(Movement dir, float timePressed);
    void mouseEvent(float xOffset, float yOffset);
    
    void update(GLuint shader);
    
};

#endif /* Camera_hpp */
