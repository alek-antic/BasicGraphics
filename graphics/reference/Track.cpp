//
//  Track.cpp
//  graphics
//
//  Created by Alek Antic on 11/14/19.
//  Copyright © 2019 Alek Antic. All rights reserved.
//

#include "Track.h"

glm::mat4 transform = glm::mat4(1.0f);

Track::Track() {
    currentControlPt = 0;
    currentPt = 0;

    // Create curves here
    curves[0] = new Bezier(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-2.0f, 1.0f, 2.0f), glm::vec3(5.0f, 3.0f, -1.0f), glm::vec3(1.0f, 0.0f, 5.0f));
    curves[1] = new Bezier(glm::vec3(1.0f, 0.0f, 5.0f), glm::vec3(-3.0f, 3.0f, 5.0f), glm::vec3(-5.0f, 1.0f, 2.0f), glm::vec3(3.0f, 2.0f, 3.0f));
    curves[2] = new Bezier(glm::vec3(3.0f, 2.0f, 3.0f), glm::vec3(2.0f, -2.0f, 3.0f), glm::vec3(4.0f, -4.0f, 2.0f), glm::vec3(6.0f, -4.0f, -3.0f));
    curves[3] = new Bezier(glm::vec3(6.0f, -4.0f, -3.0f), glm::vec3(-1.0f, 6.0f, 3.0f), glm::vec3(-5.0f, 0.0f, 3.0f), glm::vec3(-7.0f, -2.0f, 0.0f));
    curves[4] = new Bezier(glm::vec3(-7.0f, -2.0f, 0.0f), glm::vec3(-3.0f, 1.0f, -1.0f), glm::vec3(-4.0f, -4.0f, 4.0f), glm::vec3(-3.0f, 4.0f, 3.0f));
    curves[5] = new Bezier(glm::vec3(-3.0f, 4.0f, 3.0f), glm::vec3(1.0f, 2.0f, 3.0f), glm::vec3(3.0f, 2.0f, 1.0f), glm::vec3(-4.0f, 2.0f, 6.0f));
    curves[6] = new Bezier(glm::vec3(-4.0f, 2.0f, 6.0f), glm::vec3(2.0f, 3.0f, 4.0f), glm::vec3(-4.0f, -3.0f, -2.0f), glm::vec3(-2.0f, 1.0f, 3.0f));
    curves[7] = new Bezier(glm::vec3(-2.0f, 1.0f, 3.0f), glm::vec3(4.0f, 1.0f, 3.0f), glm::vec3(-1.0f, -3.0f, 5.0f),  glm::vec3(0.0f, 0.0f, 0.0f));
}

Track::~Track() {
    for (Bezier * b : curves) {
        delete b;
    }
    
    glDeleteProgram(program);
}

void Track::draw(glm::mat4 C) {
    transform = C;
    for (int i = 0; i < 8; i++) {
        curves[i]->draw(C);
    }
}

glm::vec3 Track::advancePoint() {
    glm::vec3 point = curves[currentPt / 200]->getPoint(((float) (currentPt % 200)) / 200.0f);
    point = glm::vec3(transform * glm::vec4(point, 1.0f));
    currentPt++;
    currentPt = currentPt % 1600;
    return point;
}

void Track::nextCtrlPoint() {
    currentControlPt = ++currentControlPt % 32;
}

void Track::prevCtrlPoint() {
    currentControlPt = --currentControlPt % 32;
}

void Track::translatePt(glm::vec3 delta) {
    curves[currentControlPt / 8]->translatePt(delta);
}

void Track::update(glm::mat4 proj, glm::mat4 view, glm::vec3 camPos) {
    for (Bezier * b : curves) {
        b->update(proj, view, camPos);
    }
    
    for (int i = 0; i < 8; i++) {
        if (i == currentControlPt / 8) {
            int p = currentControlPt % 4;
            CTRL pt;
            if (p == 0)
                pt = P0;
            else if (p == 1)
                pt = P1;
            else if (p == 2)
                pt = P2;
            else
                pt = P3;
            curves[i]->setControlPoint(pt);
        } else {
            curves[i]->setControlPoint(NOP);
        }
    }
}


