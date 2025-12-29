#pragma once

#include <vector>
#include <glad/glad.h>
#include "DataLoader.h"

class Renderer {
private:
    GLuint m_VAO;
    GLuint m_VBO;
    unsigned long long m_VertexCount;

public:
    Renderer();
    ~Renderer(); 

    void init(const std::vector<Point>& points);

    void draw();
};