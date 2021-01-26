#pragma once

#include "parrot/renderer/GraphicsContext.h"

struct GLFWwindow;

namespace parrot {

    class OpenGLContext : public GraphicsContext {
    public:
        OpenGLContext(GLFWwindow* window_hanlde);

        virtual void init       () override;
        virtual void swapBuffers() override;
    private:
        GLFWwindow* m_window_handle;
    };
}