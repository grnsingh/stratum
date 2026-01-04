#include "Renderer.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <glm/glm.hpp>

Renderer::Renderer() {}

Renderer::~Renderer() {
    for (auto& chunk : m_Chunks) {
        if (chunk.VAO != 0) glDeleteVertexArrays(1, &chunk.VAO);
        if (chunk.VBO != 0) glDeleteBuffers(1, &chunk.VBO);
    }
}

void Renderer::init(std::vector<Point>& points) {
    if (points.empty()) return;

    // Find Bounds of the whole world to determine grid size
    float minX = 1e9, maxX = -1e9;
    float minZ = 1e9, maxZ = -1e9;

    for (const auto& p : points) {
        if (p.x < minX) minX = p.x; if (p.x > maxX) maxX = p.x;
        if (p.z < minZ) minZ = p.z; if (p.z > maxZ) maxZ = p.z;
    }

    // This splits the mountain into a grid so we can cull parts that are far away
    int gridSize = 4; 
    float stepX = (maxX - minX) / gridSize;
    float stepZ = (maxZ - minZ) / gridSize;

    std::vector<std::vector<Point>> chunkBuckets(gridSize * gridSize);

    for (const auto& p : points) {
        // Calculate which grid cell this point belongs to
        int xIndex = (int)((p.x - minX) / stepX);
        int zIndex = (int)((p.z - minZ) / stepZ);

        // Clamp indices (handle edge case where p.x == maxX)
        if (xIndex >= gridSize) xIndex = gridSize - 1;
        if (zIndex >= gridSize) zIndex = gridSize - 1;

        int bucketIndex = zIndex * gridSize + xIndex;
        chunkBuckets[bucketIndex].push_back(p);
    }

    
    auto rng = std::default_random_engine{};

    for (int i = 0; i < chunkBuckets.size(); i++) {
        std::vector<Point>& bucket = chunkBuckets[i];
        if (bucket.empty()) continue;

        // By randomizing the order, "Draw first 10%" becomes "Draw 10% density"
        // If we didn't shuffle, 10% could just be a slice of the corner
        std::shuffle(bucket.begin(), bucket.end(), rng);

        Chunk newChunk;
        newChunk.vertexCount = bucket.size();

        float avgX = 0, avgY = 0, avgZ = 0;
        for (const auto& p : bucket) {
            avgX += p.x; avgY += p.y; avgZ += p.z;
        }
        newChunk.center = glm::vec3(avgX / bucket.size(), avgY / bucket.size(), avgZ / bucket.size());

        // OpenGL Upload
        glGenVertexArrays(1, &newChunk.VAO);
        glGenBuffers(1, &newChunk.VBO);

        glBindVertexArray(newChunk.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, newChunk.VBO);

        glBufferData(GL_ARRAY_BUFFER,
            bucket.size() * sizeof(Point),
            bucket.data(),
            GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)0);
        glEnableVertexAttribArray(0);

        // Unbinding (first buffer then the array)
        glBindBuffer(GL_ARRAY_BUFFER, 0); 
        glBindVertexArray(0);

        m_Chunks.push_back(newChunk);
    }

    std::cout << "[RENDERER] Created " << m_Chunks.size() << " LOD Chunks." << std::endl;
}

void Renderer::draw(const glm::vec3& cameraPos) {
    if (m_Chunks.empty()) return;

    for (const auto& chunk : m_Chunks) {
        glBindVertexArray(chunk.VAO);

        float dist = glm::distance(cameraPos, chunk.center);

        int pointsToDraw = 0;

        if (dist < 2000.0f) {
            pointsToDraw = chunk.vertexCount;
        } 
        else if (dist < 5000.0f) {
            // Since points are shuffled, this looks like a uniform reduction
            pointsToDraw = chunk.vertexCount / 4;
        } 
        else if (dist < 10000.0f) {
            // Far -> 5% Details
            pointsToDraw = chunk.vertexCount / 20;
        } 
        else {
            pointsToDraw = 0;
        }

        if (pointsToDraw > 0) {
            glDrawArrays(GL_POINTS, 0, (GLsizei)pointsToDraw);
        }
    }

    glBindVertexArray(0);
}
