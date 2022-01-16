#ifndef _GALAXY_H_
#define _GALAXY_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <stdio.h>
#include "camera.h"
#include "shader.h"
#include "texture.h"
#include "celestialbody.h"
#include "skybox.h"

class Galaxy{
public:
    Galaxy::Galaxy(char* name, std::vector<std::string> faces, std::vector<std::string> shader_files); // Constructor
    char name[20];
    std::vector<std::string> f;
    CSkybox* s;
    CelestialBody* c;
};

#endif