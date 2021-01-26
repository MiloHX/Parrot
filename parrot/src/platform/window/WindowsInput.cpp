#include "prpch.h"
#include "WindowsInput.h"
#include "parrot/Input.h"
#include "parrot/Application.h"

#include <GLFW/glfw3.h>

namespace parrot {

    Input* Input::s_instance = new WindowsInput();

    bool WindowsInput::isKeyPressedImpl(int key_code) {
        auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        auto state  = glfwGetKey(window, key_code);

        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool WindowsInput::isMouseButtonPressedImpl(int button) {
        auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        auto state  = glfwGetMouseButton(window, button);

        return state == GLFW_PRESS;
    }

    std::pair<float, float> WindowsInput::getMousePositionImpl() {
        auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        return { (float)xpos, (float)ypos };
    }
}