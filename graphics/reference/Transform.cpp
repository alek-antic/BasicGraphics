#include "Transform.h"
#include <stdio.h>


Transform::Transform(glm::mat4 M) {
    this->M = M;
    this->children = std::list<Node *>();
    this->animator = false;
    this->useDraw = true;
}

Transform::Transform(float delta, glm::vec3 axis, float maxAngle) {
    this->animator = true;
    this->maxAngle = maxAngle;
    this->curAngle = 0.0;
    this->delta = delta;
    this->axis = axis;
    this->M = glm::mat4(1.0f);
    this->children = std::list<Node *>();
    this->useDraw = true;
}

void Transform::draw(glm::mat4 C) {
   if (useDraw) {
        glm::mat4 M_new = C * M;
        for (Node * n : this->children) {
            if (n) {
                n->draw(M_new);
            }
        }
   }
}

void Transform::update(glm::mat4 proj, glm::mat4 view, glm::vec3 camPos) {
    if (this->animator) {
        curAngle += delta;
        this->M = glm::rotate(glm::radians(curAngle), axis);
        
        if (glm::abs(curAngle) >= glm::abs(maxAngle)) {
            maxAngle = -maxAngle;
            delta = -delta;
        }
    }
    
    for (Node * n : this->children) {
        if (n) {
            n->update(proj, view, camPos);
        }
    }
}

void Transform::addChild(Node * n) {
    this->children.push_back(n);
}

void Transform::setDraw(bool useDraw) {
    this->useDraw = useDraw;
}

void Transform::changeTransform(glm::mat4 M) {
    this->M = M;
}
