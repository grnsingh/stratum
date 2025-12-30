#include "Camera.h"
#include <algorithm> 

Camera::Camera(glm::vec3 target) 
    : m_Target(target), m_Radius(500.0f), m_Yaw(0.0f), m_Pitch(0.5f) {}

glm::mat4 Camera::GetViewMatrix() const {
    glm::vec3 position = CalculatePosition();
    return glm::lookAt(position, m_Target, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::ProcessMouseMovements(float x, float y) {
    float sensitivity = 0.001f;

    // Update Angles
    m_Yaw   -= x * sensitivity;
    m_Pitch += y * sensitivity;
}

void Camera::ProcessMouseScroll(float yoffset) {
    float zoomSpeed = m_Radius * 0.1f;
    m_Radius -= yoffset * zoomSpeed;
    
    if (m_Radius < 1.0f) m_Radius = 1.0f;
    if (m_Radius > 10000.0f) m_Radius = 10000.0f;
}

void Camera::UpdateRadius(float newRadius) {
    m_Radius = newRadius;
}

glm::vec3 Camera::GetPosition() const {
    return CalculatePosition();
}

glm::vec3 Camera::CalculatePosition() const {
    // The Spherical to Cartesian 
    float x = m_Radius * cos(m_Pitch) * sin(m_Yaw);
    float y = m_Radius * sin(m_Pitch);
    float z = m_Radius * cos(m_Pitch) * cos(m_Yaw);

    return m_Target + glm::vec3(x, y, z);
}