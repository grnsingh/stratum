#pragma once

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>

class Shader {
    public:
        GLuint ID;

        Shader(const char* vertexPath, const char* fragmentPath);

        void use();

        // Utility functions
        void setBool(const std::string &name, bool value) const;
        void setFloat(const std::string &name, float value) const;
        void setInt(const std::string &name, int value) const;
        void setMat4(const std::string &name, const glm::mat4 &mat) const;
    
    private:
        void checkForErrors(unsigned int shader, std::string type);
};
