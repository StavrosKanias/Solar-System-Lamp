#include "star.h"

using namespace glm;
using namespace std;

Star::Star(char* sObj, char* sName, char* sdTexture, char* ssTexture, float sMass, float sRadius, float sdpy, CelestialBody* orbitBody, float oRadius, float lightPower, GLFWwindow* window) :
	CelestialBody(sObj, sName, sdTexture, ssTexture, sMass, sRadius, sdpy, orbitBody, oRadius) { // Star constructor calling CelestialBody's constructor
	w = window;
	lp = lightPower;
	l = new Light(window,
		vec4{ 0.5, 0.5, 0.5, 0.5 },
		vec4{ 2.0, 2.0, 2.0, 2.0 },
		vec4{ 0.1, 0.1, 0.1, 0.1 },
		vec3{ 0, 0, 0 },
		lp
	);
}

void Star::renderStar(GLuint modelMatrixLocation, GLuint diffuseColorSampler, GLuint specularColorSampler, GLuint useTextureLocation, float time)
{
	CelestialBody::render(modelMatrixLocation, diffuseColorSampler, specularColorSampler, useTextureLocation, time);
	l->update(modelMatrix);
}