#include "Window.h"
#include "DataLoader.h"
#include "Renderer.h"
#include "Shader.h"
#include "Camera.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <future> 
#include <thread>
#include <chrono>
#include <algorithm> 
#include <iostream>

Camera camera(glm::vec3(0.0f, 0.0f, 0.0f)); 

float g_LastX = 0.0f;
float g_LastY = 0.0f;
bool  g_FirstMouse = true;
bool  g_IsDragging = false;
bool  g_MouseEnabled = true;

// CALLBACKS
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (!ImGui::GetIO().WantCaptureMouse) {
        camera.ProcessMouseScroll((float)yoffset);
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

    if (ImGui::GetIO().WantCaptureMouse) {
        g_IsDragging = false;
        return;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (!g_IsDragging) {
            g_LastX = (float)xpos;
            g_LastY = (float)ypos;
            g_IsDragging = true;
        }

        float xoffset = (float)xpos - g_LastX;
        float yoffset = g_LastY - (float)ypos; 
        g_LastX = (float)xpos;
        g_LastY = (float)ypos;

        camera.ProcessMouseMovements(xoffset, yoffset);
    } else {
        g_IsDragging = false;
    }
}

int main() {

    Window appWindow(1280, 720, "Stratum");
    if (!appWindow.init()) return -1;

    GLFWwindow* nativeWin = appWindow.getNativeWindow();
    glfwSetFramebufferSizeCallback(nativeWin, framebuffer_size_callback);
    glfwSetCursorPosCallback(nativeWin, mouse_callback);
    glfwSetScrollCallback(nativeWin, scroll_callback);

    // IMGUI INIT 
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark(); // Cool Dark Theme
    ImGui_ImplGlfw_InitForOpenGL(nativeWin, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    Shader terrainShader("C:/Users/15878/Desktop/Projects/stratum/src/shaders/terrain.vert", 
                         "C:/Users/15878/Desktop/Projects/stratum/src/shaders/terrain.frag");
    Renderer renderer;

    std::cout << "[MAIN] Launching worker thread..." << std::endl;
    std::future<std::unique_ptr<std::vector<Point>>> loadTask = std::async(
        std::launch::async, 
        DataLoader::loadTerrain, 
        "C:/Users/15878/Desktop/Projects/stratum/assets/converted/terrain.txt"
    );

    // App State Variables
    bool isDataLoaded = false;
    bool isRadiusSet  = false; 
    
    // UI Variables
    float minHeightVal = 0.0f;
    float maxHeightVal = 1000.0f;
    float pointSize    = 1.0f;
    float bgColor[4]   = { 0.1f, 0.1f, 0.1f, 1.0f };

    // RENDER LOOP
    while (appWindow.isOpen()) {
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (!isDataLoaded) {

            if (loadTask.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                auto points = loadTask.get(); 
                if (points && !points->empty()) {
                    // Pre-process Centering
                    float minX = 1e9, maxX = -1e9, minY = 1e9, maxY = -1e9;
                    float minZ = 1e9, maxZ = -1e9;

                    for (const auto& p : *points) {
                        if (p.x < minX) minX = p.x; if (p.x > maxX) maxX = p.x;
                        if (p.y < minY) minY = p.y; if (p.y > maxY) maxY = p.y;
                        if (p.z < minZ) minZ = p.z; if (p.z > maxZ) maxZ = p.z;
                    }

                    float centerX = (minX + maxX) / 2.0f;
                    float centerY = (minY + maxY) / 2.0f;
                    float centerZ = (minZ + maxZ) / 2.0f;

                    for (auto& p : *points) {
                        p.x -= centerX; p.y -= centerY; p.z -= centerZ;
                    }

                    minHeightVal = minZ - centerZ;
                    maxHeightVal = maxZ - centerZ;

                    float widthX = maxX - minX;
                    float widthY = maxY - minY;
                    float maxDim = std::max(widthX, widthY);
                    
                    if(!isRadiusSet) {
                         camera.UpdateRadius(maxDim * 1.5f);
                         isRadiusSet = true;
                    }

                    renderer.init(*points);
                    isDataLoaded = true;
                }
            }
            
            ImGui::SetNextWindowPos(ImVec2(50, 50));
            ImGui::Begin("Loading...", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::Text("Loading Terrain Data...");
            ImGui::End();
        } 
        else {
            // ACTIVE STATE 
            
            ImGui::Begin("Stratum Controls"); 
            ImGui::Text("Application Average: %.1f FPS", ImGui::GetIO().Framerate);
            ImGui::Separator();
            
            ImGui::Text("Visualization Settings");
            ImGui::ColorEdit3("Background", bgColor);
            ImGui::SliderFloat("Point Size", &pointSize, 1.0f, 10.0f);
            
            ImGui::Separator();
            ImGui::Text("Height Map Gradient");
            ImGui::SliderFloat("Sea Level (Min)", &minHeightVal, -5000.0f, 5000.0f);
            ImGui::SliderFloat("Snow Level (Max)", &maxHeightVal, -5000.0f, 5000.0f);

            ImGui::End();

            terrainShader.use();
            glPointSize(pointSize);

            int width, height;
            glfwGetFramebufferSize(nativeWin, &width, &height);
            float aspectRatio = (width > 0 && height > 0) ? (float)width / (float)height : 1.0f;

            glm::mat4 view  = camera.GetViewMatrix();
            glm::mat4 proj  = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 10000.0f);
            glm::mat4 model = glm::mat4(1.0f);
            glm::mat4 mvp   = proj * view * model;

            terrainShader.setMat4("u_MVP", mvp);
            terrainShader.setFloat("u_MinHeight", minHeightVal); 
            terrainShader.setFloat("u_MaxHeight", maxHeightVal);

            renderer.draw(camera.GetPosition());
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        appWindow.update();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}