#include "Renderer.h"
#include <iostream>

Renderer::Renderer(): m_VAO(0), m_VBO(0), m_VertexCount(0) {}

Renderer::~Renderer() {
    if (m_VAO != 0) glDeleteVertexArrays(1, &m_VAO);
    if (m_VBO != 0) glDeleteBuffers(1, &m_VBO);
}

void Renderer::init(const std::vector<Point>& points) {
    m_VertexCount = points.size();

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    // GL_STATIC_DRAW so we write once and draw multiple times using same data
    glBufferData(GL_ARRAY_BUFFER,
        m_VertexCount * sizeof(Point),
        points.data(),
        GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point), 0);

    glEnableVertexAttribArray(0);

    // Unbinding (first buffer then the array)
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);
}

void Renderer::draw() {
    if (m_VertexCount == 0) return;
    
    glBindVertexArray(m_VAO);

    glDrawArrays(GL_POINTS, 0, (GLsizei)m_VertexCount);

    glBindVertexArray(0);
}
