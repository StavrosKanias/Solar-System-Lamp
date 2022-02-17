#ifndef _CSYKBOX_H_
#define _CSYKBOX_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <stdio.h>
#include "camera.h"
#include "shader.h"
#include "texture.h"

const bool SKYBOX_DEBUG = true;

class CSkybox
{
public:
    CSkybox::CSkybox(std::vector<std::string> f, std::vector<std::string> shader_files); // Constructor
    void CSkybox::render(GLfloat *view, GLfloat *projection, GLuint originalShader);
    void createContext();
    void CSkybox::applyTexture(std::vector<std::string> faces);
    unsigned int cubemapTexture;
    GLuint skyboxVAO;
    GLuint skyboxVBO;
    GLuint textureID;
    GLuint skyboxShader;
    GLuint skyboxProjection;
    GLuint skyboxView;
    GLuint shaderProgram;
    GLuint skyboxSampler;
};

#endif