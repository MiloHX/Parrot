#include "prpch.h"
#include "platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace parrot {

    OpenGLContext::OpenGLContext(GLFWwindow* window_hanlde) : m_window_handle(window_hanlde) {
        PR_CORE_ASSERT(window_hanlde, "Window Hanlde is Null")
    }

    void OpenGLContext::init() {
        glfwMakeContextCurrent(m_window_handle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        PR_CORE_ASSERT(status, "Glad Initialization Failed")

        PR_CORE_INFO("OpenGL Renderer: {0} - {1}", glGetString(GL_VENDOR), glGetString(GL_RENDERER));
        PR_CORE_INFO("OpenGL Version : {0}", glGetString(GL_VERSION));
    }

    void OpenGLContext::swapBuffers() {
        glfwSwapBuffers(m_window_handle);
    }

}