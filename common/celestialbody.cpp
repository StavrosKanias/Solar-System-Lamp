#include "celestialbody.h"

using namespace glm;
using namespace std;

CelestialBody::CelestialBody(char* bObj, char* bName, char* bdTexture, char* bsTexture, float bMass, float bRadius, float daysPerYear, CelestialBody* orbitBody, float oRadius) // Constructor
{
	strcpy(name, bName);
	b = new Drawable(bObj);
	td = loadSOIL(bdTexture);
	ts = loadSOIL(bsTexture);
	r = bRadius;
	m = bMass;
	ob = orbitBody;
	orbr = oRadius;
	orbv = 0.0;
	dpy = daysPerYear;
	dest = 0;
	e = 0.0;
	angv = 0.0;
	
}

void CelestialBody::render(GLuint modelMatrixLocation, GLuint diffuseColorSampler, GLuint specularColorSampler, GLuint useTextureLocation, double time)
{
	if (ob != NULL) {
		orbv = sqrt((G * ob->m * pow(10,3)) / orbr);
		angv = (orbv / orbr);
		// creating a model matrix
		orbitRotation = rotate(mat4(), float(angv * time), vec3(0.0, 1.0, 0.0));
		axisRotation = rotate(mat4(), float(dpy * angv * time), vec3(0.0, 1.0, 0.0));
		size = scale(mat4(), vec3(r, r, r));
		orbitRadius = translate(mat4(), vec3(orbr, 0.0, 0.0));
		modelMatrix = (ob->modelMatrix / ob->size) * (orbitRotation * orbitRadius * axisRotation * size) ;
	}

	else {
		orbitRotation = rotate(mat4(), float(0), vec3(0.0, 1.0, 0.0));
		axisRotation = rotate(mat4(), float(0), vec3(0.0, 1.0, 0.0));
		size = scale(mat4(), vec3(r, r, r));
		orbitRadius = translate(mat4(), vec3(0.0, 0.0, 0.0));
		modelMatrix = size;
	}

	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

	// Setting up texture to display on shader program          //  --- Texture Pipeline ---
	glActiveTexture(GL_TEXTURE0);								// Activate texture position
	glBindTexture(GL_TEXTURE_2D, td);			// Assign texture to position 
	glUniform1i(diffuseColorSampler, 1);						// Assign sampler to that position

	glActiveTexture(GL_TEXTURE1);								//
	glBindTexture(GL_TEXTURE_2D, ts);			// Same process for specular texture
	glUniform1i(specularColorSampler, 2);						//

	// Inside the fragment shader, there is an if statement whether to use  
	// the material of an object or sample a texture
	glUniform1i(useTextureLocation, 1);

	// Draw model1
	b->bind();
	b->draw();
	
}