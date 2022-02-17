#include "skybox.h"

using namespace glm;
using namespace std;

CSkybox::CSkybox(std::vector<std::string> faces, std::vector<std::string> shader_files) // Constructor
{

    shaderProgram = loadShaders(shader_files.at(0).c_str(), shader_files.at(1).c_str());

    skyboxProjection = glGetUniformLocation(shaderProgram, "projection");
    skyboxView = glGetUniformLocation(shaderProgram, "view");
    skyboxSampler = glGetUniformLocation(shaderProgram, "skybox");
    createContext();
    applyTexture(faces);

}

void CSkybox::applyTexture(std::vector<std::string> faces) {
    textureID = loadCubemap(faces);
}


void CSkybox::createContext() {

    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &skyboxVAO);
    glBindVertexArray(skyboxVAO);

    glGenBuffers(1, &skyboxVBO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);

}

void CSkybox::render(GLfloat *view, GLfloat *projection, GLuint originalShader)
{  
    
    glDepthFunc(GL_LEQUAL);
    glUseProgram(shaderProgram);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    glUniform1i(skyboxSampler, 0);

    glBindVertexArray(skyboxVAO);

    glUniformMatrix4fv(skyboxProjection, 1, GL_FALSE, projection);
    glUniformMatrix4fv(skyboxView, 1, GL_FALSE, view);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDepthFunc(GL_LESS);
    glUseProgram(originalShader);
}