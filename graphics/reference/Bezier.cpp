//
//  Bezier.cpp
//  graphics
//
//  Created by Alek Antic on 11/14/19.
//  Copyright © 2019 Alek Antic. All rights reserved.
//

#include "Bezier.h"
#include "Transform.h"

const float NUM_POINTS = 200.0;

Bezier::Bezier(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
    program = LoadShaders("shaders/bezier.vert", "shaders/bezier.frag");
    currentPoint = NOP;

    this->p0 = p0;
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    
    calculateCoeff();
    updatePoints();
    pointsChanged = false;
    setColor(glm::vec3(1.0f));
}

Bezier::~Bezier() {
    glDeleteBuffers(1, &vertexVBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(program);
}

glm::vec3 Bezier::getPoint(float t) {
    return (a * t * t * t) + (b * t * t) + (c * t) + d;
}


void Bezier::draw(glm::mat4 C) {
    glUseProgram(program);
    
    glLineWidth(10.0f);
    
    GLint modelLoc = glGetUniformLocation(program, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(C));
    
    GLint colorLoc = glGetUniformLocation(program, "color");
    glUniform3fv(colorLoc, 1, glm::value_ptr(this->color));
    
    // Bind to the VAO.
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_STRIP, 0, points.size());
    // Unbind from the VAO.
    glBindVertexArray(0);
}

void Bezier::setControlPoint(CTRL p) {
    currentPoint = p;
}

void Bezier::translatePt(glm::vec3 delta) {
    switch (currentPoint) {
        case P0:
            this->p0 += delta;
            break;
        case P1:
            this->p1 += delta;
            break;
        case P2:
            this->p2 += delta;
            break;
        case P3:
            this->p3 += delta;
            break;
        default:
            break;
    }
    
    pointsChanged = true;
}

void Bezier::calculateCoeff() {
    this->a = -p0 + 3.0f * p1 - 3.0f * p2 + p3;
    this->b = 3.0f * p0 - 6.0f * p1 + 3.0f * p2;
    this->c = -3.0f * p0 + 3.0f * p1;
    this->d = p0;
}

void Bezier::updatePoints() {
    // Generate all of the points
    points = std::vector<glm::vec3>();
    for (float i = 0.0; i <= 1.0; i += 0.005) {
        glm::vec3 pt = getPoint(i);
        points.push_back(pt);
    }
    
    // Put all the points into the VBO, link it to VAO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &vertexVBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Bezier::update(glm::mat4 proj, glm::mat4 view, glm::vec3 camPos) {
    if (pointsChanged) {
        calculateCoeff();
        updatePoints();
        pointsChanged = false;
    }
    
    glUseProgram(program);
    GLint projLoc = glGetUniformLocation(program, "projection");
    GLint viewLoc = glGetUniformLocation(program, "view");
    GLint eyeLoc = glGetUniformLocation(program, "eye");
    
    
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniform3fv(eyeLoc, 1, glm::value_ptr(camPos));
}
