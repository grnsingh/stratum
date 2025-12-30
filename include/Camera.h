#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera{
    public:
    Camera(glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f));

    glm::mat4 GetViewMatrix() const;

    void ProcessMouseMovements(float x, float y);
    void ProcessMouseScroll(float x);
    void UpdateRadius(float radius);

    glm::vec3 GetPosition() const;

    private:
    glm::vec3 m_Target; 
    float m_Radius;
    float m_Yaw;   // (Horizontal)
    float m_Pitch; // (Vertical)

    glm::vec3 CalculatePosition() const;

};