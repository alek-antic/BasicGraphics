#ifndef Terrain_hpp
#define Terrain_hpp

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

#include "Object.h"
#include "shader.h"

const int DIM = 1025;

class Terrain : public Object
{
private:
    float heights[DIM][DIM];
    
    float peak, base, avg;
    
    GLuint program;
    GLuint vao;
    GLuint vbos[3];
    
    void diamondSquare(float (&arr)[DIM][DIM], int size);
    void diamondStep(float (&arr)[DIM][DIM], int xPos, int zPos, int reach);
    void squareStep(float (&arr)[DIM][DIM], int xPos, int zPos, int reach);
    
public:
    Terrain(int seed);
    ~Terrain();
    
    void draw();
    void update();
    
    GLuint getShader();
};

#endif /* Terrain_hpp */
