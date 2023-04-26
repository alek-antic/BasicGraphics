#version 330 core

/*
 CEL SHADER
 */

in vec3 fragPos;
in vec3 fragNorm;

uniform float peak;
uniform float base;
uniform float avg;

uniform vec3 viewPos;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform int useToon;

out vec4 fragColor;

void main() {
    vec3 result;
    vec3 viewDir = -normalize(viewPos - fragPos);
    
    float edge = max(0.0f, dot(normalize(fragNorm), normalize(viewDir)));
    if (edge < 0.2) {
        result = vec3(0.0f,0.0f,0.0f);
    } else {
        float y = fragPos.y / peak;
        
        vec3 matAmbient;
        vec3 matDiffuse;
        float phongConst;
        
        if (y > 0.90) {
            // snowy peaks
            matAmbient = vec3(0.25, 0.25, 0.25);
            matDiffuse = vec3(0.9, 0.9, 0.9);
            phongConst = 0.078125;
        } else if (y > 0.75) {
            // rocky area
            matAmbient = vec3(0.10, 0.10, 0.10);
            matDiffuse = vec3(0.5, 0.5, 0.5);
            phongConst = 0.078125;
        } else if (y > 0.6) {
            // forest emergent
            matAmbient = vec3(0.05, 0.15, 0.05);
            matDiffuse = vec3(0.04, 0.4, 0.04);
            phongConst = 0.078125;
        } else if (y > 0.55) {
            // forest canopy
            matAmbient = vec3(0.15, 0.15, 0.10);
            matDiffuse = vec3(0.04, 0.58, 0.04);
            phongConst = 0.078125;
        } else if (y > 0.5) {
            // forest understory
            matAmbient = vec3(0.15, 0.15, 0.10);
            matDiffuse = vec3(0.0, 0.7, 0.04);
            phongConst = 0.078125;
        } else if (y > 0.375) {
            // dirt lands
            matAmbient = vec3(0.15, 0.10, 0.01);
            matDiffuse = vec3(0.4, 0.2, 0.0);
            phongConst = 0.078125;
        } else if (y > 0.25) {
            // badlands
            matAmbient = vec3(0.20, 0.1, 0.01);
            matDiffuse = vec3(0.6, 0.2, 0.0);
            phongConst = 0.4;
        } else {
            // desert
            matAmbient = vec3(0.20, 0.20, 0.01);
            matDiffuse = vec3(0.87, 0.87, 0.0);
            phongConst = 0.21794872;
        }
        
        vec3 norm = normalize(fragNorm);
        vec3 lightDir = normalize(lightPos - fragPos);
        vec3 reflectDir = reflect(lightDir, norm);
        
        float diffuseStrength = 1.0f;
        float diff = max(dot(norm, lightDir), 0.0f);
        vec3 diffuse = diff * lightColor;
        
        result = matAmbient + diffuse * matDiffuse;
        
        
        float shading = dot(norm, lightDir);
        float celShading = 1.0f;
    
        if (useToon != 0) {
            if (shading > 0.95)
                celShading = 1.0f;
            else if (shading > 0.5)
                celShading = 0.75f;
            else if (shading > 0.25)
                celShading = 0.5f;
            else
                celShading = 0.35f;
        }
        
        result = celShading * result;
    }
    fragColor = vec4(result, 1.0f);
}
