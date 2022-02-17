#include "light.h"
using namespace glm;
float near_plane = 0.1f;
float far_plane = 1000000.0f;
std::vector<glm::mat4> shadowTransforms;

Light::Light(GLFWwindow* window, 
             glm::vec4 init_La,
             glm::vec4 init_Ld,
             glm::vec4 init_Ls,
             glm::vec3 init_position,
             float init_power) : window(window) {
    La = init_La;
    Ld = init_Ld;
    Ls = init_Ls;
    power = init_power;
    //lightPosition_worldspace = M * vec4(vec3(0.0,0.0,0.0),1);
    lightPosition_worldspace = init_position;

    // setting near and far plane affects the detail of the shadow
    nearPlane = -10000.0;
    farPlane = 10000.0;

    direction = normalize(targetPosition - lightPosition_worldspace);

    lightSpeed = 0.1f;
    targetPosition = glm::vec3(0.0, 0.0, 0.0);


    projectionMatrix = ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
    orthoProj = true;
}

void Light::update(glm::mat4 sModelMatrix, float near_plane, float far_plane, int SHADOW_WIDTH, int SHADOW_HEIGHT) {
 
    lightPosition_worldspace = vec3(glm::column(sModelMatrix,3));
    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPosition_worldspace, lightPosition_worldspace + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPosition_worldspace, lightPosition_worldspace + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPosition_worldspace, lightPosition_worldspace + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPosition_worldspace, lightPosition_worldspace + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPosition_worldspace, lightPosition_worldspace + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPosition_worldspace, lightPosition_worldspace + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
}

void Light::uploadLight(GLuint LaLocation, GLuint LdLocation, GLuint LsLocation, GLuint lightPositionLocation, GLuint lightPowerLocation, GLuint farPlaneLocation) {
    
    glUniform4f(LaLocation, La.r, La.g, La.b, La.a);
    glUniform4f(LdLocation, Ld.r, Ld.g, Ld.b, Ld.a);
    glUniform4f(LsLocation, Ls.r, Ls.g, Ls.b, Ls.a);
    glUniform3f(lightPositionLocation, lightPosition_worldspace.x, lightPosition_worldspace.y, lightPosition_worldspace.z);
    glUniform1f(lightPowerLocation, power);
    glUniform1f(farPlaneLocation, far_plane);

}

void Light::uploadDepth(GLuint shadowMatrices[6], GLuint shadowLightPositionLocation, GLuint depthFarPlaneLocation) {

    for (int i = 0; i < 6; i++)
        glUniformMatrix4fv(shadowMatrices[i], 1, GL_FALSE, &shadowTransforms[i][0][0]);
    glUniform3f(shadowLightPositionLocation, lightPosition_worldspace.x, lightPosition_worldspace.y, lightPosition_worldspace.z);
    glUniform1f(depthFarPlaneLocation, far_plane);

}
