//
//  Camera.cpp
//  graphics
//
//  Created by Alek Antic on 12/5/19.
//  Copyright © 2019 Alek Antic. All rights reserved.
//

#include "Camera.h"
#include <iostream>


Camera::Camera(glm::vec3 global_up) {
    pos = glm::vec3(0.0f, 1025.0f, 0.0f);
    up = world_up = global_up;
    yaw = YAW;
    pitch = PITCH;
    
    this->updateVecs();
}

glm::mat4 Camera::getView() {
    return glm::lookAt(this->pos, this->pos + this->dir, this->up);
}

void Camera::keyEvent(Movement dir, float timePressed) {
    float dist = timePressed * speed;
    switch (dir) {
        case FORWARD:
            this->pos += this->dir * dist;
            break;
        case BACKWARD:
            this->pos -= this->dir * dist;
            break;
        case LEFT:
            this->pos -= this->right * dist;
            break;
        case RIGHT:
            this->pos += this->right * dist;
            break;
    }
}

void Camera::mouseEvent(float xOffset, float yOffset) {
    xOffset *= sensitivity;
    yOffset *= sensitivity;
    
    this->yaw   += xOffset;
    this->pitch -= yOffset;
    
    if (this->pitch > 89.0f)
        this->pitch = 89.0f;
    if (this->pitch < -89.0f)
        this->pitch = -89.0f;
    
    this->updateVecs();
}


void Camera::updateVecs() {
    glm::vec3 dir;
    dir.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    dir.y = sin(glm::radians(this->pitch));
    dir.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->dir = glm::normalize(dir);
    this->right = glm::normalize(glm::cross(this->dir, this->world_up));
    this->up = glm::normalize(glm::cross(this->right, this->dir));
}


void Camera::update(GLuint shader) {
    GLuint viewLoc = glGetUniformLocation(shader, "view");
    GLuint viewPosLoc = glGetUniformLocation(shader, "viewPos");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(this->getView()));
    glUniform3fv(viewPosLoc, 1, glm::value_ptr(this->pos));
}
