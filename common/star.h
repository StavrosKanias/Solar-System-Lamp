#ifndef _STAR_H_
#define _STAR_H_

#include "celestialbody.h"
#include "light.h"

class Star : public CelestialBody{
public:
    Star::Star(char* sObj, char* sName, char* sdTexture, char* ssTexture, float sMass, float sRadius, float sdpy, CelestialBody* orbitBody, float oRadius, float yOff, float lightPower, GLFWwindow* window, std::vector<std::string> explosionShader_files); // Constructor
    void Star::renderStar(GLuint modelMatrixLocation, GLuint diffuseColorSampler, GLuint specularColorSampler, GLuint useTextureLocation, float time);
    void Star::explode(GLfloat* view, GLfloat* projection, GLuint originalShader);
    GLFWwindow* w;
    Light* l;
    float lp;
    GLuint explosionShader;
    GLuint explosionViewMatrixLocation;
    GLuint explosionProjectionMatrixLocation;
    GLuint explosionModelMatrixLocation;
    GLuint explosionDiffuseColorSampler;
    GLuint explosionTimeLocation;
    float explosionTime;
    float explosionSpeed;
};

#endif