#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <glm/glm.hpp>

namespace Config {

    const std::string dataDir ="/Users/fang/Desktop/4611/project5/data";
    const std::string codeDir ="/Users/fang/Documents/codingStuff/xcode-projects/4611-project5/4611-project5";

    // Shaders
    const std::string phongVert = codeDir + "/phong.vert";
    const std::string phongFrag = codeDir + "/phong.frag";
    const std::string silhouetteVert = codeDir + "/silhouette.vert";
    const std::string silhouetteFrag = codeDir + "/silhouette.frag";

    // Mesh and ramps
    const std::string mesh[8] = {dataDir + "/cow.obj", dataDir + "/bunny.obj",
                                dataDir + "/teapot.obj", dataDir + "/hippo.obj",
                                dataDir + "/maxplanck.obj", dataDir + "/sphere.obj",
                                dataDir + "/bunnyLowres.obj", dataDir + "/chamferedCube.obj"};
    const std::string diffuseRamp = dataDir + "/toonDiffuse.bmp";
    const std::string specularRamp = dataDir + "/toonSpecular.bmp";

    // Shader parameters.
    // TODO: Pass them into your shaders using uniform variables.

    // Lighting
    glm::vec4 Ia(0.3, 0.3, 0.3, 1);
    glm::vec4 Id(0.7, 0.7, 0.7, 1);
    glm::vec4 Is(1.0, 1.0, 1.0, 1);
    // Material parameters
    glm::vec4 ka(1.0, 0.4, 0.4, 1);
    glm::vec4 kd(1.0, 0.4, 0.4, 1);
    glm::vec4 ks(0.6, 0.6, 0.6, 1);
    float s = 50;

    // Outline parameters
    float thickness = 0.01;

}

#endif
