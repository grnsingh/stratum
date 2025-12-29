#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

class Window {
    private:
    GLFWwindow* m_Window;
    int m_Width;
    int m_Height;
    std::string m_Title;

    public:
        Window(int width, int height, const std::string& title);
        ~Window();

        bool init();

        bool isOpen();
        void update();
        void clear();

        int getWidth() const {return m_Width; };
        int getHeight() const {return m_Height; };
        GLFWwindow* getNativeWindow() const { return m_Window; }
};