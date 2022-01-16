#include <glfw3.h>
#include <iostream>
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include "light.h"

using namespace glm;

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
    nearPlane = 0.0;
    farPlane = 5000.0;

    direction = normalize(targetPosition - lightPosition_worldspace);

    lightSpeed = 0.1f;
    targetPosition = glm::vec3(0.0, 0.0, 0.0);


    projectionMatrix = ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
    orthoProj = true;
}



void Light::update(glm::mat4 sModelMatrix) {
 
    lightPosition_worldspace = vec3(glm::column(sModelMatrix,3));

}


mat4 Light::lightVP() {
    return projectionMatrix * viewMatrix;
}