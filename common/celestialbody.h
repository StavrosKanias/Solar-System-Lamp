#ifndef _CELESTIALBODY_H_
#define _CELESTIALBODY_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <string>
#include <iostream>
#include <cstring>
#include <vector>
#include <stdio.h>
#include "camera.h"
#include "shader.h"
#include "model.h"
#include "texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define G 0.0000000000667

class CelestialBody {
public:
    CelestialBody::CelestialBody(char* bObj, char* bName, char* bdTexture, char* bsTexture, float bMass, float bRadius, float dayLengh, CelestialBody* orbitBody, float oRadius, float yOff); // Constructor
    void CelestialBody::render(GLuint modelMatrixLocation, GLuint diffuseColorSampler, GLuint specularColorSampler, GLuint useTextureLocation, double time);
    char name[20];
    CelestialBody* ob;
    Drawable* b;
    GLuint td,ts;
    float r;
    float m;
    float orbr;;
    float angv;
    double orbv;
    float dpy;
    glm::mat4 axisRotation;
    glm::mat4 orbitRotation;
    glm::mat4 orbitRadius;
    glm::mat4 size;
    glm::mat4 modelMatrix;
    bool dest;
    glm::mat4 elevate;
    float yOffset;
    float originalOrbR;
    float dr;
    float dt;
};

#endif