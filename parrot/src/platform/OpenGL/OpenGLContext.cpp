#include "prpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace parrot {

    OpenGLContext::OpenGLContext(GLFWwindow* window_hanlde) : m_window_handle(window_hanlde) {
        PR_INT_ASSERT(window_hanlde, "Window Hanlde is Null")
    }

    void OpenGLContext::init() {
        glfwMakeContextCurrent(m_window_handle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        PR_INT_ASSERT(status, "Glad Initialization Failed")
    }

    void OpenGLContext::swapBuffers() {
        glfwSwapBuffers(m_window_handle);
    }

}