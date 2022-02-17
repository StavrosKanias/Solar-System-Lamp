#include "galaxy.h"

using namespace glm;
using namespace std;

Galaxy::Galaxy(char* gName, std::vector<std::string> faces, std::vector<std::string> shader_files)
{    
    strcpy(name, gName);
    for (int i = 0; i < 6; i++) {
        f.push_back(faces[i]);
    }
    s = new CSkybox(f, shader_files);
    c = new CelestialBody("../objs/planet.obj", gName, "../textures/center.jpg", "../textures/center.jpg", 0.01, 0.001, 1.0, NULL, 0.0, 0.1);
}