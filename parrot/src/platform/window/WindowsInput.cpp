#include "prpch.h"
#include "parrot/core/Input.h"
#include "parrot/core/Application.h"

#include <GLFW/glfw3.h>

namespace parrot {

    bool Input::isKeyPressed(KeyCode key_code) {
        auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        auto state  = glfwGetKey(window, (int)key_code);

        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::isMouseButtonPressed(MouseButton button) {
        auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        auto state  = glfwGetMouseButton(window, (int)button);

        return state == GLFW_PRESS;
    }

    std::pair<float, float> Input::getMousePosition() {
        auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        return { (float)xpos, (float)ypos };
    }
}