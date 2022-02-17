#ifndef _ROOM_H_
#define _ROOM_H_
// Include C++ headers
#include <iostream>
#include <string>

// Include GLEW
#include <GL/glew.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <stdio.h>
#include "camera.h"
#include "shader.h"
#include "texture.h"
#include "celestialbody.h"
#include "camera.h"

class Room {
public:
    Room::Room(std::vector<std::string> faces, GLuint shaderProgram); // Constructor
    void Room::createContext();
    void Room::render(GLuint modelMatrixLocation, GLuint diffuseColorSampler, GLuint specularColorSampler, GLuint useTextureLocation, Camera* cameras);
    Drawable* walls[6];
    std::vector<glm::vec3> wallVertices[6];
    std::vector<glm::vec3> wallNormals[6];
    GLuint tds[6];
    GLuint tss[6];
    glm::mat4 scaling;
    glm::mat4 modelMatrix;
    GLuint roomUniformLocation;
};


#endif