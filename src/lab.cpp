// Include C++ headers
#include <iostream>
#include <string>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Shader loading utilities and other
#include <common/shader.h>
#include <common/util.h>
#include <common/camera.h>
#include <common/model.h>
#include <common/texture.h>
#include <common/light.h>
#include <common/skybox.h>
#include <common/celestialbody.h>
#include <common/galaxy.h> 
#include <common/star.h> 

#include <math.h>
 
using namespace std;
using namespace glm;

// Function prototypes
void initialize();
void createContext();
void mainLoop();
void free();

#define W_WIDTH 1920
#define W_HEIGHT 1080
#define TITLE "Shadow Mapping"

#define SHADOW_WIDTH 1024
#define SHADOW_HEIGHT 1024

// Global Variables
GLFWwindow* window;
Camera* camera;
Light* light;
Galaxy* galaxy;
Star* sun;
CelestialBody* solarSystem[12];
GLuint shaderProgram, depthProgram, miniMapProgram;
GLuint modelDiffuseTexture, modelSpecularTexture;
GLuint depthFrameBuffer, depthTexture;
float s = 1.0;

// locations for shaderProgram
GLuint viewMatrixLocation;
GLuint projectionMatrixLocation;
GLuint modelMatrixLocation;
GLuint KaLocation, KdLocation, KsLocation, NsLocation;
GLuint LaLocation, LdLocation, LsLocation;
GLuint lightPositionLocation;
GLuint lightPowerLocation;
GLuint diffuseColorSampler; 
GLuint specularColorSampler;
GLuint useTextureLocation;
GLuint depthMapSampler;
GLuint lightVPLocation;
GLuint lightDirectionLocation;
GLuint lightFarPlaneLocation;
GLuint lightNearPlaneLocation;

// locations for depthProgram
GLuint shadowViewProjectionLocation; 
GLuint shadowModelLocation;

// locations for miniMapProgram
GLuint quadTextureSamplerLocation;

// Creating a function to upload (make uniform) the light parameters to the shader program
void uploadLight(const Light& light) {
	glUniform4f(LaLocation, light.La.r, light.La.g, light.La.b, light.La.a);
	glUniform4f(LdLocation, light.Ld.r, light.Ld.g, light.Ld.b, light.Ld.a);
	glUniform4f(LsLocation, light.Ls.r, light.Ls.g, light.Ls.b, light.Ls.a);
	glUniform3f(lightPositionLocation, light.lightPosition_worldspace.x,
		light.lightPosition_worldspace.y, light.lightPosition_worldspace.z);
	glUniform1f(lightPowerLocation, light.power);
}


void createContext() {

	// Create and complile our GLSL program from the shader
	shaderProgram = loadShaders("ShadowMapping.vertexshader", "ShadowMapping.fragmentshader");
	
	// Task 3.1 
	// Create and load the shader program for the depth buffer construction
	// You need to load and use the Depth.vertexshader, Depth.fragmentshader
	// NOTE: These files are not visible in the visual studio, as they are not a part of
	//       the main project. To add a new file to our project, we need to add it to the
	//       CMAKELists.txt and build the project again. 
	// NOTE: We alse need to create a global variable to store new shader program
	depthProgram = loadShaders("Depth.vertexshader", "Depth.fragmentshader");
	

	// Task 2.1
	// Use the SimpleTexture.vertexshader, "SimpleTexture.fragmentshader"
	miniMapProgram = loadShaders("SimpleTexture.vertexshader", "SimpleTexture.fragmentshader");
	

	// NOTE: Don't forget to delete the shader programs on the free() function


	// Get pointers to uniforms
	// --- shaderProgram ---
	projectionMatrixLocation = glGetUniformLocation(shaderProgram, "P");
	viewMatrixLocation = glGetUniformLocation(shaderProgram, "V");
	modelMatrixLocation = glGetUniformLocation(shaderProgram, "M");
	// for phong lighting
	KaLocation = glGetUniformLocation(shaderProgram, "mtl.Ka");
	KdLocation = glGetUniformLocation(shaderProgram, "mtl.Kd");
	KsLocation = glGetUniformLocation(shaderProgram, "mtl.Ks");
	NsLocation = glGetUniformLocation(shaderProgram, "mtl.Ns");
	LaLocation = glGetUniformLocation(shaderProgram, "light.La");
	LdLocation = glGetUniformLocation(shaderProgram, "light.Ld");
	LsLocation = glGetUniformLocation(shaderProgram, "light.Ls");
	lightPositionLocation = glGetUniformLocation(shaderProgram, "light.lightPosition_worldspace");
	lightPowerLocation = glGetUniformLocation(shaderProgram, "light.power");
	diffuseColorSampler = glGetUniformLocation(shaderProgram, "diffuseColorSampler");
	specularColorSampler = glGetUniformLocation(shaderProgram, "specularColorSampler");
	
	// Task 1.4
	useTextureLocation = glGetUniformLocation(shaderProgram, "useTexture"); 

	// locations for shadow rendering
	depthMapSampler = glGetUniformLocation(shaderProgram, "shadowMapSampler");
	lightVPLocation = glGetUniformLocation(shaderProgram, "lightVP");


	// --- depthProgram ---
	shadowViewProjectionLocation = glGetUniformLocation(depthProgram, "VP");
	shadowModelLocation = glGetUniformLocation(depthProgram, "M");

	// --- miniMapProgram ---
	quadTextureSamplerLocation = glGetUniformLocation(miniMapProgram, "textureSampler");

	// ---------------------------------------------------------------------------- //
	// -  Task 3.2 Create a depth framebuffer and a texture to store the depthmap - //
	// ---------------------------------------------------------------------------- //
	
	// Tell opengl to generate a framebuffer
	glGenFramebuffers(1, &depthFrameBuffer);
	// Binding the framebuffer, all changes bellow will affect the binded framebuffer
	// **Don't forget to bind the default framebuffer at the end of initialization
	glBindFramebuffer(GL_FRAMEBUFFER, depthFrameBuffer);

	// We need a texture to store the depth image
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	// Telling opengl the required information about the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0,
		GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);							// Task 4.5 Texture wrapping methods
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);							// GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER

	// Task 4.5 Don't shadow area out of light's viewport
	
	// Step 1 : (Don't forget to comment out the respective lines above
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	// Set color to set out of border 
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// Next go to fragment shader and add an iff statement, so if the distance in the z-buffer is equal to 1, 
	// meaning that the fragment is out of the texture border (or further than the far clip plane) 
	// then the shadow value is 0.
	//*/

	 //Task 3.2 Continue
	// Attaching the texture to the framebuffer, so that it will monitor the depth component
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

	// Since the depth buffer is only for the generation of the depth texture, 
	// there is no need to have a color output
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);


	// Finally, we have to always check that our frame buffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		glfwTerminate();
		throw runtime_error("Frame buffer not initialized correctly");
	}

	// Binding the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	

	std::vector<std::string> faces = {
		"../textures/space_right.tga",
		"../textures/space_left.tga",
		"../textures/space_up.tga",
		"../textures/space_down.tga",
		"../textures/space_front.tga",
		"../textures/space_back.tga"
	};

	std::vector<std::string> skyboxShaders = {
		"skybox.vertexshader",
		"skybox.fragmentshader"
	};

	//Create skybox 
	float sunmass = 1.9 * pow(10, 8);
	float mercurymass = 3.3 * pow(10, 1), merradius = 0.24 , meroradius = 58.0, merdpy = 0.151;
	float venusmass = 4.8 * pow(10, 2), vradius = 0.6, voradius = 108.0, vdpy = 0.093;
	float earthmass = 5.9 * pow(10, 2), eradius = 0.64, eoradius = 150.0, edpy = 36.5;
	float moonmass = 7.3 * pow(10, 0), mradius = 0.1 , moradius = 5.0, modpy = 1.0;
	float marsmass = 6.4 * pow(10, 1), marradius = 0.33, maroradius = 228.0, mardpy = 66.0;
	float jupitermass = 1.8 * pow(10, 5), jradius = 69.9, joradius = 778.0, jdpy = 1039.9;
	float saturnmass = 5.6 * pow(10, 4), satradius = 58.23, soradius = 1430.0, satdpy = 2347.4;
	float uranusmass = 8.6 * pow(10, 3), uradius = 25.36, uoradius = 2870.0, udpy = 4091.6;
	float neptunemass = 10.2 * pow(10, 3), nradius = 24.6, noradius = 4500.0, ndpy = 9028.5;
	

	galaxy = new Galaxy("Milky Way", faces, skyboxShaders);
	sun = new Star("../objs/star.obj", "sun", "../textures/sun.jpg", "../textures/sun.jpg", sunmass, 28.0, 1.0, galaxy->c, 1000, 15.0, window);
	solarSystem[0] = new CelestialBody("../objs/planet.obj", "mercury", "../textures/mercurymap.jpg", "../textures/mercurymap.jpg", mercurymass, merradius, merdpy, sun, meroradius);
	solarSystem[1] = new CelestialBody("../objs/planet.obj", "venus", "../textures/venusmap.jpg", "../textures/venusmap.jpg", venusmass, vradius,vdpy, sun, voradius);
	solarSystem[2] = new CelestialBody("../objs/planet.obj", "earth", "../textures/earthmap.jpg", "../textures/earthmap.jpg", earthmass, eradius, edpy, sun, eoradius);
	solarSystem[3] = new CelestialBody("../objs/satellite.obj", "moon", "../textures/moonmap.jpg", "../textures/moonmap.jpg", moonmass, mradius, modpy, solarSystem[2], moradius);
	solarSystem[4] = new CelestialBody("../objs/planet.obj", "mars", "../textures/marsmap.jpg", "../textures/marsmap.jpg", marsmass, marradius, mardpy, sun, maroradius);
	solarSystem[5] = new CelestialBody("../objs/satellite.obj", "phobos", "../textures/phobosbump.jpg", "../textures/phobosbump.jpg", moonmass , mradius / 1.5, modpy, solarSystem[4], moradius - 0.5);
	solarSystem[6] = new CelestialBody("../objs/satellite.obj", "deimos", "../textures/deimosbump.jpg", "../textures/deimosbump.jpg", moonmass , mradius / 2.0, modpy, solarSystem[4], moradius + 0.5);
	solarSystem[7] = new CelestialBody("../objs/planet.obj", "jupiter", "../textures/jupitermap.jpg", "../textures/jupitermap.jpg", jupitermass, jradius, jdpy, sun, joradius);
	solarSystem[8] = new CelestialBody("../objs/planet.obj", "saturn", "../textures/saturnmap.jpg", "../textures/saturnmap.jpg", saturnmass, satradius,satdpy, sun, soradius);
	solarSystem[9] = new CelestialBody("../objs/saturn_ring.obj", "saturn's ring", "../textures/satrunallrings.png", "../textures/saturnring.png", saturnmass, satradius +30.0, 0, sun, soradius);
	solarSystem[10] = new CelestialBody("../objs/planet.obj", "uranus", "../textures/uranusmap.jpg", "../textures/uranusmap.jpg", uranusmass, uradius, udpy, sun, uoradius);
	solarSystem[11] = new CelestialBody("../objs/planet.obj", "neptune", "../textures/neptunemap.jpg", "../textures/neptunemap.jpg", neptunemass, nradius, ndpy, sun, noradius);

}

void free() {
	// Delete Shader Programs
	glDeleteProgram(shaderProgram);
	glDeleteProgram(depthProgram);
	glDeleteProgram(miniMapProgram);

	glfwTerminate();
}


void depth_pass(mat4 viewMatrix, mat4 projectionMatrix) {

	// Task 4.3
	


	// Setting viewport to shadow map size
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

	// Binding the depth framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthFrameBuffer);

	// Cleaning the framebuffer depth information (stored from the last render)
	glClear(GL_DEPTH_BUFFER_BIT);

	// Selecting the new shader program that will output the depth component
	glUseProgram(depthProgram);

	// sending the view and projection matrix to the shader
	mat4 view_projection = projectionMatrix * viewMatrix;
	glUniformMatrix4fv(shadowViewProjectionLocation, 1, GL_FALSE, &view_projection[0][0]);

	// binding the default framebuffer again
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	
}

void lighting_pass(mat4 viewMatrix, mat4 projectionMatrix) {

	// Step 1: Binding a frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, W_WIDTH, W_HEIGHT);

	// Step 2: Clearing color and depth info
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Step 3: Selecting shader program
	glUseProgram(shaderProgram);

	// Making view and projection matrices uniform to the shader program
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
	// uploading the light parameters to the shader program
	uploadLight(*sun->l);

	// Task 4.1 Display shadows on the 
	//*/
	// Sending the shadow texture to the shaderProgram
	
	// Setting up texture to display on shader program          //  --- Texture Pipeline ---
	glActiveTexture(GL_TEXTURE0);								// Activate texture position
	glBindTexture(GL_TEXTURE_2D, depthTexture);			// Assign texture to position 
	glUniform1i(depthMapSampler, 0);						// Assign sampler to that position

	// Sending the light View-Projection matrix to the shader program

	mat4 lightVP = sun->l->projectionMatrix * sun->l->viewMatrix;
	glUniformMatrix4fv(lightVPLocation, 1, GL_FALSE, &lightVP[0][0]);
	//*/




	// ----------------------------------------------------------------- //
	// --------------------- Drawing scene objects --------------------- //	
	// ----------------------------------------------------------------- //
	double time =  s * glfwGetTime(); // get the clock
	sun->renderStar(modelMatrixLocation, diffuseColorSampler, specularColorSampler, useTextureLocation, time);
	for (int i = 0; i < 12; i++) {
		solarSystem[i]->render(modelMatrixLocation, diffuseColorSampler, specularColorSampler, useTextureLocation, time);
	}

}

// Task 2.3: visualize the depth_map on a sub-window at the top of the screen
void renderDepthMap() {
	// using the correct shaders to visualize the depth texture on the quad
	glUseProgram(miniMapProgram);

	//enabling the texture - follow the aforementioned pipeline
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glUniform1i(quadTextureSamplerLocation, 0);

	// Drawing the quad
	//quad->bind();
	//quad->draw();
}



void mainLoop() {


	do {

		mat4 light_proj = sun->l->projectionMatrix;
		mat4 light_view = sun->l->viewMatrix;


		// Task 3.5
		// Create the depth buffer
		depth_pass(light_view, light_proj);

		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
			camera->speed = 300;
		}
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
			camera->speed = 30;
		}

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			s += 0.1;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			if (s > 0.0) {
				printf("%f",s);
				s -= 0.1;
			}
		}

		// Getting camera information
		camera->update();
		mat4 projectionMatrix = camera->projectionMatrix;
		mat4 viewMatrix = camera->viewMatrix;
		mat4 viewSkyboxMatrix = mat4(mat3(viewMatrix));


		lighting_pass(viewMatrix, projectionMatrix);

		galaxy->s->render(&viewSkyboxMatrix[0][0], &projectionMatrix[0][0], shaderProgram);

		// Task 2.3:
		renderDepthMap();


		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

}


void initialize() {
	// Initialize GLFW
	if (!glfwInit()) {
		throw runtime_error("Failed to initialize GLFW\n");
	}


	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// Open a window and create its OpenGL context
	//window = glfwCreateWindow(W_WIDTH, W_HEIGHT, TITLE, glfwGetPrimaryMonitor(), NULL);
	window = glfwCreateWindow(W_WIDTH, W_HEIGHT, TITLE, NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		throw runtime_error(string(string("Failed to open GLFW window.") +
			" If you have an Intel GPU, they are not 3.3 compatible." +
			"Try the 2.1 version.\n"));
	}
	glfwMakeContextCurrent(window);

	// Start GLEW extension handler
	glewExperimental = GL_TRUE;

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		throw runtime_error("Failed to initialize GLEW\n");
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Hide the mouse and enable unlimited movement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, W_WIDTH / 2, W_HEIGHT / 2);

	// Gray background color
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	// glEnable(GL_CULL_FACE);

	// enable texturing and bind the depth texture
	glEnable(GL_TEXTURE_2D);

	// Log
	logGLParameters();

	// Create camera
	camera = new Camera(window);
}

int main(void) {
	try {
		initialize();
		createContext();
		mainLoop();
		free();
	}
	catch (exception& ex) {
		cout << ex.what() << endl;
		char c = getchar();
		free();
		return -1;
	}

	return 0;
}