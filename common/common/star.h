#ifndef _STAR_H_
#define _STAR_H_

#include "celestialbody.h"
#include "light.h"

class Star : public CelestialBody{
public:
    Star::Star(char* sObj, char* sName, char* sdTexture, char* ssTexture, float sMass, float sRadius, float sdpy, CelestialBody* orbitBody, float oRadius, float lightPower, GLFWwindow* window); // Constructor
    void Star::renderStar(GLuint modelMatrixLocation, GLuint diffuseColorSampler, GLuint specularColorSampler, GLuint useTextureLocation, float time);
    GLFWwindow* w;
    Light* l;
    float lp;
};

#endif