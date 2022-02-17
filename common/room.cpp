#include "room.h"
using namespace glm;
using namespace std;



Room::Room(std::vector<std::string> faces, GLuint shaderProgram) { // Constructor
    for (int i = 0; i < 6; i++) {
        tds[i] = loadSOIL(faces[i].c_str());
        tss[i] = loadSOIL(faces[i].c_str());
    }
    createContext();
    roomUniformLocation = glGetUniformLocation(shaderProgram, "room");
    glUniform1i(roomUniformLocation, 0);
}
void Room::createContext() {
    //front->left->right->back->up->down
    // front->back->right->left->up->down
    //clockwise vertices

    wallVertices[0] = {
        vec3(-1.0f,  -1.0f, -1.0f),
        vec3(-1.0f, 1.0f, -1.0f),
        vec3(1.0f, 1.0f, -1.0f),
        vec3(1.0f, 1.0f, -1.0f),
        vec3(1.0f,  -1.0f, -1.0f),
        vec3(-1.0f,  -1.0f, -1.0f)
    };

    wallNormals[0] = {
        vec3(0.0f, 0.0f, 1.0f),
        vec3(0.0f, 0.0f, 1.0f),
        vec3(0.0f, 0.0f, 1.0f),
        vec3(0.0f, 0.0f, 1.0f),
        vec3(0.0f, 0.0f, 1.0f),
        vec3(0.0f, 0.0f, 1.0f)
    };

    wallVertices[1] = {
        vec3(1.0f, -1.0f,  1.0f),
        vec3(1.0f,  1.0f,  1.0f),
        vec3(-1.0f,  1.0f,  1.0f),
        vec3(-1.0f,  1.0f,  1.0f),
        vec3(-1.0f, -1.0f,  1.0f),
        vec3(1.0f, -1.0f,  1.0f)
    };

    wallNormals[1] = {
        vec3(0.0f, 0.0f, -1.0f),
        vec3(0.0f, 0.0f, -1.0f),
        vec3(0.0f, 0.0f, -1.0f),
        vec3(0.0f, 0.0f, -1.0f),
        vec3(0.0f, 0.0f, -1.0f),
        vec3(0.0f, 0.0f, -1.0f)
    };

    wallVertices[2] = {
         vec3(1.0f, -1.0f, -1.0f),
         vec3(1.0f, 1.0f,  -1.0f),
         vec3(1.0f,  1.0f,  1.0f),
         vec3(1.0f,  1.0f,  1.0f),
         vec3(1.0f,  -1.0f, 1.0f),
         vec3(1.0f, -1.0f, -1.0f)
    };

    wallNormals[2] = {
        vec3(-1.0f, 0.0f, 0.0f),
        vec3(-1.0f, 0.0f, 0.0f),
        vec3(-1.0f, 0.0f, 0.0f),
        vec3(-1.0f, 0.0f, 0.0f),
        vec3(-1.0f, 0.0f, 0.0f),
        vec3(-1.0f, 0.0f, 0.0f)
    };

    wallVertices[3] = {
        vec3(-1.0f, -1.0f,  1.0f),
        vec3(-1.0f, 1.0f, 1.0f),
        vec3(-1.0f,  1.0f, -1.0f),
        vec3(-1.0f,  1.0f, -1.0f),
        vec3(-1.0f,  -1.0f,  -1.0f),
        vec3(-1.0f, -1.0f,  1.0f)
    };

    wallNormals[3] = {
        vec3(1.0f, 0.0f, 0.0f),
        vec3(1.0f, 0.0f, 0.0f),
        vec3(1.0f, 0.0f, 0.0f),
        vec3(1.0f, 0.0f, 0.0f),
        vec3(1.0f, 0.0f, 0.0f),
        vec3(1.0f, 0.0f, 0.0f)
    };

    wallVertices[4] = {
        vec3(-1.0f,  1.0f, -1.0f),
        vec3(1.0f,  1.0f, -1.0f),
        vec3(1.0f,  1.0f,  1.0f),
        vec3(1.0f,  1.0f,  1.0f),
        vec3(-1.0f,  1.0f,  1.0f),
        vec3(-1.0f,  1.0f, -1.0f)
    };

    wallNormals[4] = {
        vec3(0.0f, -1.0f, 0.0f),
        vec3(0.0f, -1.0f, 0.0f),
        vec3(0.0f, -1.0f, 0.0f),
        vec3(0.0f, -1.0f, 0.0f),
        vec3(0.0f, -1.0f, 0.0f),
        vec3(0.0f, -1.0f, 0.0f)
    };

    wallVertices[5] = {
       vec3(-1.0f, -1.0f, -1.0f),
       vec3(-1.0f, -1.0f,  1.0f),
       vec3(1.0f, -1.0f, 1.0f),
       vec3(1.0f, -1.0f, 1.0f),
       vec3(1.0f, -1.0f,  -1.0f),
       vec3(-1.0f, -1.0f,  -1.0f)
    };

    wallNormals[5] = {
        vec3(0.0f, 1.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f)
    };

    vector<vec2> UVs = {
        vec2(0.0f, 0.0f),
        vec2(0.0f, 1.0f),
        vec2(1.0f, 1.0f),
        vec2(1.0f, 1.0f),
        vec2(1.0f, 0.0f),
        vec2(0.0f, 0.0f),
    };

    for (int i = 0; i < 6; i++) {
        walls[i] = new Drawable(wallVertices[i], UVs, wallNormals[i]);
    }

}

void Room::render(GLuint modelMatrixLocation, GLuint diffuseColorSampler, GLuint specularColorSampler, GLuint useTextureLocation, Camera* camera) {

    if (camera->position.x > 6900.0) {
        camera->position.x = 6900.0;
    }
    if (camera->position.y > 6900.0) {
        camera->position.y = 6900.0;
    }
    if (camera->position.z > 6900.0) {
        camera->position.z = 6900.0;
    }

    if (camera->position.x < -6900.0) {
        camera->position.x = -6900.0;
    }
    if (camera->position.y < -6900.0) {
        camera->position.y = -6900.0;
    }
    if (camera->position.z < -6900.0) {
        camera->position.z = -6900.0;
    }

    for (int i = 0; i < 6; i++) {
        scaling = scale(mat4(), vec3(7000.0, 7000.0, 7000.0));
        modelMatrix = scaling;
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
        // Setting up texture to display on shader program          //  --- Texture Pipeline ---
        glActiveTexture(GL_TEXTURE1);								// Activate texture position
        glBindTexture(GL_TEXTURE_2D, tds[i]);			// Assign texture to position 
        glUniform1i(diffuseColorSampler, 1);						// Assign sampler to that position

        glActiveTexture(GL_TEXTURE2);								//
        glBindTexture(GL_TEXTURE_2D, tss[i]);			// Same process for specular texture
        glUniform1i(specularColorSampler, 2);						//

        // Inside the fragment shader, there is an if statement whether to use  
        // the material of an object or sample a texture
        glUniform1i(useTextureLocation, 1);
        glUniform1i(roomUniformLocation, 1);
        // Draw model1
        walls[i]->bind();
        walls[i]->draw();
        glUniform1i(roomUniformLocation, 0);
    }
}
