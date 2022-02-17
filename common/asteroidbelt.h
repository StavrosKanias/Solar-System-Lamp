#ifndef _ASTEROIDBELT_H_
#define _ASTEROIDBELT_H_

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

class AsteroidBelt {
public:
    AsteroidBelt::AsteroidBelt(char* asteroidObj, char* aTexture, int amount, double nearPlanetOrbitradius, double farPlanetOrbitRadius, double width, std::vector<std::string> shader_files); // Constructor
    void AsteroidBelt::createContext();
    void AsteroidBelt::render(GLfloat* asteroidProjection, GLfloat* asteroidView, GLuint originalShader);
    glm::mat4* modelMatrices;
    unsigned int amount;
    float radius;
    float offset;
    Drawable* rock;
    GLuint shaderProgram;
    GLuint asteroidProjectionLocation;
    GLuint asteroidViewLocation;
    GLuint asteroidTexture;
    GLuint diffuseColorSampler;
};
#endif