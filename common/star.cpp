#include "star.h"

using namespace glm;
using namespace std;

Star::Star(char* sObj, char* sName, char* sdTexture, char* ssTexture, float sMass, float sRadius, float sdpy, CelestialBody* orbitBody, float oRadius, float yOff, float lightPower, GLFWwindow* window, std::vector<std::string> explosionShader_files) :
	CelestialBody(sObj, sName, sdTexture, ssTexture, sMass, sRadius, sdpy, orbitBody, oRadius, yOff) { // Star constructor calling CelestialBody's constructor
	w = window;
	lp = lightPower;
	l = new Light(window,
		vec4{ 0.5, 0.5, 0.5, 0.5 },
		vec4{ 2.0, 2.0, 2.0, 2.0 },
		vec4{ 0.1, 0.1, 0.1, 0.1 },
		vec3{ 0, 0, 0 },
		lp
	);
	explosionTime = 1.0;
	explosionSpeed = 0.001;

	explosionShader = loadShaders(explosionShader_files.at(0).c_str(), explosionShader_files.at(1).c_str(), explosionShader_files.at(2).c_str());
	explosionDiffuseColorSampler = glGetUniformLocation(explosionShader, "texture_diffuse");
	explosionModelMatrixLocation = glGetUniformLocation(explosionShader, "model");
	explosionProjectionMatrixLocation = glGetUniformLocation(explosionShader, "projection");
	explosionViewMatrixLocation = glGetUniformLocation(explosionShader, "view");
	explosionTimeLocation = glGetUniformLocation(explosionShader, "time");

}

void Star::renderStar(GLuint modelMatrixLocation, GLuint diffuseColorSampler, GLuint specularColorSampler, GLuint useTextureLocation, float time)
{
	CelestialBody::render(modelMatrixLocation, diffuseColorSampler, specularColorSampler, useTextureLocation, time);
}

void Star::explode(GLfloat* view, GLfloat* projection, GLuint originalShader) {
	
	mat4 explosionModelMatrix = size;
	glUseProgram(explosionShader);
	glUniformMatrix4fv(explosionModelMatrixLocation, 1, GL_FALSE, &explosionModelMatrix[0][0]);
	glUniformMatrix4fv(explosionViewMatrixLocation, 1, GL_FALSE, view);
	glUniformMatrix4fv(explosionProjectionMatrixLocation, 1, GL_FALSE, projection);
	glUniform1f(explosionTimeLocation, explosionTime);
	// Setting up texture to display on shader program          //  --- Texture Pipeline ---
	glActiveTexture(GL_TEXTURE0);								// Activate texture position
	glBindTexture(GL_TEXTURE_2D, td);			// Assign texture to position 
	glUniform1i(explosionDiffuseColorSampler, 0);						// Assign sampler to that position

	b->bind();
	b->draw();
	explosionTime += explosionSpeed;
	explosionSpeed += (explosionSpeed/10.0);
	glUseProgram(originalShader);

}