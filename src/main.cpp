#include "Window.h"
#include "DataLoader.h"
#include "Renderer.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

int main() {
    Window appWindow(1280, 720, "Stratum Debug");

    //Initialize
    if (!appWindow.init()) {
        return -1;
    }

    // Load the Point Cloud
    std::vector<Point> pointsCloud;
    DataLoader::loadTerrain("C:/Users/15878/Desktop/Projects/stratum/assets/converted/terrain.txt", pointsCloud);

    Shader terrainShader("C:/Users/15878/Desktop/Projects/stratum/src/shaders/terrain.vert", 
        "C:/Users/15878/Desktop/Projects/stratum/src/shaders/terrain.frag");

    Renderer renderer;
    renderer.init(pointsCloud);


    //The Render Loop
    while (appWindow.isOpen()) {
        appWindow.clear();
        terrainShader.use();

        // TEST CAMERA MATRIX 
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(0, 100, 500), glm::vec3(0,0,0), glm::vec3(0,1,0));
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1280.0f/720.0f, 0.1f, 2000.0f);
        glm::mat4 mvp = proj * view * model;

        terrainShader.setMat4("u_MVP", mvp);

        renderer.draw();
        appWindow.update();
    }

    return 0;
}