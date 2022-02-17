#include "asteroidbelt.h"

using namespace glm;

AsteroidBelt::AsteroidBelt(char* asteroidObj, char* aTexture, int am, double nearPlanetOrbitradius, double farPlanetOrbitRadius, double width, std::vector<std::string> shader_files) {
    shaderProgram = loadShaders(shader_files.at(0).c_str(), shader_files.at(1).c_str());
    rock = new Drawable(asteroidObj);
    amount = am;
    offset = width;
    radius = nearPlanetOrbitradius + (farPlanetOrbitRadius - nearPlanetOrbitradius) / 4.0;
    asteroidTexture = loadSOIL(aTexture);
    asteroidProjectionLocation = glGetUniformLocation(shaderProgram, "projection");
    asteroidViewLocation = glGetUniformLocation(shaderProgram, "view");
    diffuseColorSampler = glGetUniformLocation(shaderProgram, "texture_diffuse");
    modelMatrices = new glm::mat4[amount];
}
void AsteroidBelt::createContext() {
    // generate a large list of semi-random model transformation matrices
    // ------------------------------------------------------------------
    srand(static_cast<unsigned int>(glfwGetTime())); // initialize random seed
    for (unsigned int i = 0; i < amount; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        // 1. translation: displace along circle with 'radius' in range [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        // 2. scale: Scale between 0.05 and 0.25f
        float scale = static_cast<float>((rand() % 20) / 100.0 + 0.05);
        model = glm::scale(model, glm::vec3(scale));

        // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
        float rotAngle = static_cast<float>((rand() % 360));
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. now add to list of matrices
        modelMatrices[i] = model;
    }

    // configure instanced array
    // -------------------------
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

    // set transformation matrices as an instance vertex attribute (with divisor 1)
    rock->bind();
    // set attribute pointers for matrix (4 times vec4)
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    glBindVertexArray(0);
    
}
void AsteroidBelt::render(GLfloat* asteroidProjection, GLfloat* asteroidView, GLuint originalShader) {

    glUseProgram(shaderProgram);
    glUniformMatrix4fv(asteroidProjectionLocation, 1, GL_FALSE, asteroidProjection);
    glUniformMatrix4fv(asteroidViewLocation, 1, GL_FALSE, asteroidView);

    // draw meteorites       
                                                       //  --- Texture Pipeline ---
    glActiveTexture(GL_TEXTURE0);					  // Activate texture position
    glBindTexture(GL_TEXTURE_2D, asteroidTexture);   // Assign texture to position  
    glUniform1i(diffuseColorSampler, 0);          	// Assign sampler to that position
    rock->bind();
    glDrawElementsInstanced(GL_TRIANGLES, rock->indices.size(), GL_UNSIGNED_INT, 0, amount);
    glBindVertexArray(0);
    glUseProgram(originalShader);
}

