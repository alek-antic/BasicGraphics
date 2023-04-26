//
//  PointLight.cpp
//  graphics
//
//  Created by Alek Antic on 12/7/19.
//  Copyright © 2019 Alek Antic. All rights reserved.
//

#include "PointLight.h"


PointLight::PointLight(glm::vec3 pos, glm::vec3 color) {
    this->pos = pos;
    this->color = color;
}

void PointLight::update(GLuint shader) {
    GLuint posLoc = glGetUniformLocation(shader, "lightPos");
    GLuint colorLoc = glGetUniformLocation(shader, "lightColor");
    
    glUniform3fv(posLoc, 1, glm::value_ptr(this->pos));
    glUniform3fv(colorLoc, 1, glm::value_ptr(this->color));
}
