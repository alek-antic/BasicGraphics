#include "Geometry.h"

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdio>

Geometry * Geometry::init(std::string filename, GLenum drawingMode) {
    Geometry * g = new Geometry();
    
    g->program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

    std::ifstream objFile(filename);
    
    if (objFile.is_open()) {
        std::string line;
        std::vector<glm::vec3> input_vert, input_norm;
        std::vector<unsigned int> tempVertInd, tempNormInd;
        
        while (std::getline(objFile, line)) {
            std::stringstream ss;
            ss << line;
            
            std::string label;
            ss >> label;
            
            if (label == "v") {
                glm::vec3 point;
                ss >> point.x >> point.y >> point.z;
                
               input_vert.push_back(point);
            } else if (label == "vn") {
                glm::vec3 normal;
                ss >> normal.x >> normal.y >> normal.z;
                
                input_norm.push_back(normal);
            } else if (label == "f") {
                glm::ivec3 vertInd, texInd, normInd;
                
                /*
                 * TODO: Add to this to use textures instead of solid color
                 */
                int res = sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
                         &vertInd.x
                       , &texInd.x
                       , &normInd.x
                       , &vertInd.y
                       , &texInd.y
                       , &normInd.y
                       , &vertInd.z
                       , &texInd.z
                       , &normInd.z
                       );
                
                if (res != 9) {
                    int res = sscanf(line.c_str(), "f %d//%d %d//%d %d//%d",
                      &vertInd.x
                    , &normInd.x
                    , &vertInd.y
                    , &normInd.y
                    , &vertInd.z
                    , &normInd.z
                    );
                    if (res != 6) {
                        std::cerr << "Error parsing obj" << std::endl;
                    }
                }
                
                vertInd.x = vertInd.x - 1;
                vertInd.y = vertInd.y - 1;
                vertInd.z = vertInd.z - 1;
                normInd.x = normInd.x - 1;
                normInd.y = normInd.y - 1;
                normInd.z = normInd.z - 1;
                                
                tempVertInd.push_back(vertInd.x);
                tempVertInd.push_back(vertInd.y);
                tempVertInd.push_back(vertInd.z);
                tempNormInd.push_back(normInd.x);
                tempNormInd.push_back(normInd.y);
                tempNormInd.push_back(normInd.z);
            }
        }
        
        for (unsigned int i = 0; i < tempVertInd.size(); i++) {
            
            g->points.push_back(input_vert[tempVertInd[i]]);
            g->norms.push_back(input_norm[tempNormInd[i]]);
            g->pointInds.push_back(i);
        }
        
    } else {
        std::cerr << "Can't open the file " << filename << std::endl;
        return NULL;
    }
    
    objFile.close();
    
    // Generate a vertex array (VAO) and a vertex buffer objects (VBO).
    glGenVertexArrays(1, &(g->VAO));
    glGenBuffers(1, &(g->vertexVBO));
    glGenBuffers(1, &(g->normalVBO));
    glGenBuffers(1, &(g->vertexEBO));
    glGenBuffers(1, &(g->normalEBO));

    // Bind to the VAO.
    glBindVertexArray(g->VAO);

    // Bind to the first VBO. We will use it to store the points.
    glBindBuffer(GL_ARRAY_BUFFER, g->vertexVBO);
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * g->points.size(),
        g->points.data(), GL_STATIC_DRAW);
    // Enable vertex attribute 0 as positions.
    // We will be able to access points through it.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    // Load normals for lighting
    glBindBuffer(GL_ARRAY_BUFFER, g->normalVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * g->norms.size(),
                 g->norms.data(), GL_STATIC_DRAW);
    // Enable vertex attribute 1 as normals.
    // Give shader access to this data
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);

    // Bind EBO. We will use it to store the vertex indicies.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g->vertexEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * g->pointInds.size(), g->pointInds.data(), GL_STATIC_DRAW);
    
    // Unbind from the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
    
    g->drawingMode = drawingMode;
    return g;
}

Geometry::~Geometry() {
    glDeleteBuffers(1, &vertexVBO);
    glDeleteBuffers(1, &vertexEBO);
    glDeleteBuffers(1, &normalVBO);
    glDeleteBuffers(1, &normalEBO);
    glDeleteVertexArrays(1, &VAO);
}

void Geometry::draw(glm::mat4 C) {
    glUseProgram(program);
    
    GLint modelLoc = glGetUniformLocation(program, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(C));
    
    GLint colorLoc = glGetUniformLocation(program, "color");
    glUniform3fv(colorLoc, 1, glm::value_ptr(this->color));
    
    // Bind to the VAO.
    glBindVertexArray(VAO);
    glDrawElements(drawingMode, 3 * pointInds.size(), GL_UNSIGNED_INT, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
}

void Geometry::update(glm::mat4 proj, glm::mat4 view, glm::vec3 camPos) {
    glUseProgram(program);
    GLint projLoc = glGetUniformLocation(program, "projection");
    GLint viewLoc = glGetUniformLocation(program, "view");
    GLint eyeLoc = glGetUniformLocation(program, "eye");
    
    
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniform3fv(eyeLoc, 1, glm::value_ptr(camPos));
}

void Geometry::setColor(glm::vec3 color) {
    this->color = color;
}
