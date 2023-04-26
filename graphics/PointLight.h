//
//  PointLight.hpp
//  graphics
//
//  Created by Alek Antic on 12/7/19.
//  Copyright © 2019 Alek Antic. All rights reserved.
//

#ifndef PointLight_hpp
#define PointLight_hpp

#include <stdio.h>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class PointLight {
private:
    glm::vec3 pos;
    glm::vec3 color;
        
public:
    PointLight(glm::vec3 pos, glm::vec3 color);
    
    void update(GLuint shader);
};

#endif /* PointLight_hpp */
