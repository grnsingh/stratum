#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "DataLoader.h"

struct Chunk {
    GLuint VAO, VBO;
    int vertexCount;
    glm::vec3 center;
    
    float minY, maxY; 
};

class Renderer {
    public:
        Renderer();
        ~Renderer();

        void init(std::vector<Point>& points); 
        void draw(const glm::vec3& cameraPos);

    private:
        std::vector<Chunk> m_Chunks;
};