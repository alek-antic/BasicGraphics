#include "Terrain.h"

#include <limits>

inline static float random(int range) {
    return (rand() % (range * 2)) - range;
}


Terrain::Terrain(int seed) {
    program = LoadShaders("shaders/cel.vert", "shaders/cel.frag");
    
    srand(seed);
    
    model = glm::mat4(1.0f);
    
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            heights[i][j] = 0;
        }
    }
    
    heights[0][0] = random(DIM);
    heights[0][DIM-1] = random(DIM);
    heights[DIM-1][0] = random(DIM);
    heights[DIM-1][DIM-1] = random(DIM);
    
    diamondSquare(heights, DIM);
    
    std::vector<glm::vec3> points;
    std::vector<glm::ivec3> indices;
    std::vector<glm::vec3> normals;
    
    avg = 0.0f;
    peak = FLT_MIN;
    base = FLT_MAX;
    
    for (int x = 0; x < DIM; x++) {
        for (int z = 0; z < DIM; z++) {
            glm::vec3 p = glm::vec3((float) (x - DIM/2), heights[x][z], (float) (z - DIM/2));
            points.push_back(p);
            
            if (heights[x][z] > peak) peak = heights[x][z];
            if (heights[x][z] < base) base = heights[x][z];
            
            avg += heights[x][z];
        }
    }
    
    avg /= (DIM * DIM);
    
    // compute normal for every point
    for (int x = 0; x < DIM; x++) {
        for (int z = 0; z < DIM; z++) {
            glm::vec3 norm = glm::vec3(0.0f,0.0f,0.0f);
            
            if (x > 0 && z > 0) {
                glm::vec3 diag = glm::vec3((float) (x - 1 - DIM/2), heights[x-1][z-1], (float) (z - 1 - DIM/2))
                                    - glm::vec3((float) (x - DIM/2), heights[x][z], (float) (z - DIM/2));
                glm::vec3 vert = glm::vec3((float) (x - 1 - DIM/2), heights[x-1][z], (float) (z - DIM/2))
                                    - glm::vec3((float) (x - DIM/2), heights[x][z], (float) (z - DIM/2));
                glm::vec3 hor = glm::vec3((float) (x - DIM/2), heights[x][z-1], (float) (z - 1 - DIM/2))
                                    - glm::vec3((float) (x - DIM/2), heights[x][z], (float) (z - DIM/2));
                
                norm += glm::cross(vert, diag) + glm::cross(diag, hor);
            }
            
            if (x < DIM - 1 && z > 0) {
                glm::vec3 diag = glm::vec3((float) (x + 1 - DIM/2), heights[x+1][z-1], (float) (z - 1 - DIM/2))
                                    - glm::vec3((float) (x - DIM/2), heights[x][z], (float) (z - DIM/2));
                glm::vec3 vert = glm::vec3((float) (x + 1 - DIM/2), heights[x+1][z], (float) (z - DIM/2))
                                    - glm::vec3((float) (x - DIM/2), heights[x][z], (float) (z - DIM/2));
                glm::vec3 hor = glm::vec3((float) (x - DIM/2), heights[x][z-1], (float) (z - 1 - DIM/2))
                                    - glm::vec3((float) (x - DIM/2), heights[x][z], (float) (z - DIM/2));
                
                norm += glm::cross(hor, diag) + glm::cross(diag, vert);
            }
            
            if (x < DIM - 1 && z < DIM - 1) {
                glm::vec3 diag = glm::vec3((float) (x + 1 - DIM/2), heights[x+1][z+1], (float) (z + 1 - DIM/2))
                                    - glm::vec3((float) (x - DIM/2), heights[x][z], (float) (z - DIM/2));
                glm::vec3 vert = glm::vec3((float) (x + 1 - DIM/2), heights[x+1][z], (float) (z - DIM/2))
                                    - glm::vec3((float) (x - DIM/2), heights[x][z], (float) (z - DIM/2));
                glm::vec3 hor = glm::vec3((float) (x - DIM/2), heights[x][z+1], (float) (z + 1 - DIM/2))
                                    - glm::vec3((float) (x - DIM/2), heights[x][z], (float) (z - DIM/2));
                
                norm += glm::cross(vert, diag) + glm::cross(diag, hor);
            }
            
            if (x > 0 && z < DIM - 1) {
                glm::vec3 diag = glm::vec3((float) (x - 1 - DIM/2), heights[x-1][z+1], (float) (z + 1 - DIM/2))
                                    - glm::vec3((float) (x - DIM/2), heights[x][z], (float) (z - DIM/2));
                glm::vec3 vert = glm::vec3((float) (x - 1 - DIM/2), heights[x-1][z], (float) (z - DIM/2))
                                    - glm::vec3((float) (x - DIM/2), heights[x][z], (float) (z - DIM/2));
                glm::vec3 hor = glm::vec3((float) (x - DIM/2), heights[x][z+1], (float) (z + 1 - DIM/2))
                                    - glm::vec3((float) (x - DIM/2), heights[x][z], (float) (z - DIM/2));
                
                norm += glm::cross(hor, diag) + glm::cross(diag, vert);
            }
            
            
            norm = glm::normalize(norm);
            normals.push_back(norm);
        }
    }
        
    for (int x = 0; x < DIM - 1; x++) {
        for (int z = 0; z < DIM - 1; z++) {
            glm::ivec3 f1 = glm::ivec3((x+1) * DIM + z+1, x * DIM + z, x * DIM + z+1);
            glm::ivec3 f2 = glm::ivec3((x+1) * DIM + z+1, (x+1) * DIM + z, x * DIM + z);
            indices.push_back(f1);
            indices.push_back(f2);
        }
    }
    
    this->model = glm::translate(glm::vec3(0.0f, -base, 0.0f));
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(3, vbos);
    
    glBindVertexArray(vao);
    
    // Vertex VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW);
    
    // Point VAO at Vertex VBO
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    // Vertex EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(), indices.data(), GL_STATIC_DRAW);
    
    // Normal VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Unbind from VBO AND VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void Terrain::diamondSquare(float (&arr)[DIM][DIM], int size) {
    
    int half = size / 2;
    if (half < 1) return;
    
    for (int x = half; x < DIM; x += size) {
        for (int z = half; z < DIM; z += size) {
            squareStep(arr, x % DIM, z % DIM, half);
        }
    }
    
    int col = 0;
    for (int x = 0; x < DIM; x+= half) {
        col++;
        if (col % 2 == 1) {
            for (int z = half; z < DIM; z += size) {
                diamondStep(arr, x % DIM, z % DIM, half);
            }
        } else {
            for (int z = 0; z < DIM; z += size) {
                diamondStep(arr, x % DIM, z % DIM, half);
            }
        }
    }
    
    diamondSquare(arr, size / 2);
}


void Terrain::squareStep(float (&arr)[DIM][DIM], int xPos, int zPos, int reach) {
    int count = 0;
    float avg = 0.0f;
    if (xPos - reach >= 0 && zPos - reach >= 0) {
        avg += arr[xPos-reach][zPos-reach];
        count++;
    }
    
    if (xPos - reach >= 0 && zPos + reach < DIM) {
        avg += arr[xPos-reach][zPos + reach];
        count++;
    }

    if (xPos + reach < DIM && zPos - reach >= 0) {
        avg += arr[xPos+reach][zPos-reach];
        count++;
    }
    
    if (xPos + reach < DIM && zPos + reach < DIM) {
        avg += arr[xPos-reach][zPos+reach];
        count++;
    }
    
    avg += random(reach * 1.5);
    avg /= count;
    arr[xPos][zPos] = avg;
    
}

void Terrain::diamondStep(float (&arr)[DIM][DIM], int xPos, int zPos, int reach) {
    int count = 0;
    float avg = 0.0f;
    
    if (xPos - reach >= 0) {
        avg += arr[xPos-reach][zPos];
        count++;
    }
    
    if (xPos + reach < DIM) {
        avg += arr[xPos+reach][zPos];
        count++;
    }
    
    if (zPos - reach >= 0) {
        avg += arr[xPos][zPos-reach];
        count++;
    }
    
    if (zPos + reach < DIM) {
        avg += arr[xPos][zPos+reach];
        count++;
    }
    
    avg += random(reach * 2);
    avg /= count;
    arr[xPos][zPos] = avg;
    
}

Terrain::~Terrain() {
    glDeleteBuffers(2, vbos);
    glDeleteVertexArrays(1, &vao);
}

void Terrain::draw() {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glUseProgram(program);
    // Bind to the VAO
    glBindVertexArray(vao);
    
    // Set shader uniforms
    GLuint modelLoc = glGetUniformLocation(program, "model");
    GLuint avgLoc   = glGetUniformLocation(program, "avg");
    GLuint peakLoc  = glGetUniformLocation(program, "peak");
    GLuint baseLoc  = glGetUniformLocation(program, "base");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(this->model));
    glUniform1f(avgLoc, avg);
    glUniform1f(peakLoc, peak - base);
    glUniform1f(baseLoc, base);
    
    // Draw triangles
    glDrawElements(GL_TRIANGLES, 2 * DIM * DIM * 3, GL_UNSIGNED_INT, 0);
    
    // Unbind from VAO
    glBindVertexArray(0);
    
    glDisable(GL_CULL_FACE);
}

void Terrain::update(){}

GLuint Terrain::getShader() {
    return program;
}
