#include "prpch.h"
#include "platform/window/WindowsWindow.h"
#include "parrot/event/ApplicationEvent.h"
#include "parrot/event/KeyEvent.h"
#include "parrot/event/MouseEvent.h"

#include "platform/OpenGL/OpenGLContext.h"

namespace parrot {

    static bool s_is_GLFW_initialized = false;

    static void GLFWErrorCallback(int error, const char* description) {
        PR_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    Window* Window::create(const WindowProps& props) {
        return new WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const WindowProps& props) {
        init(props);
    }

    WindowsWindow::~WindowsWindow() {
        shutdown();
    }

    void WindowsWindow::init(const WindowProps& props) {
        m_data.title  = props.title;
        m_data.width  = props.width;
        m_data.height = props.height;

        PR_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width, props.height);

        if (!s_is_GLFW_initialized) {
            int success = glfwInit();
            PR_CORE_ASSERT(success, "Cound not initialize GLFW! ");
            glfwSetErrorCallback(GLFWErrorCallback);
            s_is_GLFW_initialized = true;
        }

        m_window = glfwCreateWindow((int)props.width, (int)props.height, m_data.title.c_str(), nullptr, nullptr);
        
        m_context = new OpenGLContext(m_window);
        m_context->init();

        glfwSetWindowUserPointer(m_window, &m_data);
        setVSync(true);

        glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.width  = width;
            data.height = height;
            WindowResizedEvent event(width, height);
            data.event_callback(event);
        });

        glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.event_callback(event);
        });

        glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scan_code, int action, int mods) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            switch (action) {
                case GLFW_PRESS: {
                    KeyPressedEvent event(key, 0);
                    data.event_callback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    KeyReleasedEvent event(key);
                    data.event_callback(event);
                    break;
                }
                case GLFW_REPEAT: {
                    KeyPressedEvent event(key, 1);
                    data.event_callback(event);
                    break;
                }
                default:
                    break;
            }
        });

        glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int key_code) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            KeyTypedEvent event(key_code);
            data.event_callback(event);
        });

        glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            switch (action) {
                case GLFW_PRESS: {
                    MouseButtonPressedEvent event(button);
                    data.event_callback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    MouseButtonReleasedEvent event(button);
                    data.event_callback(event);
                    break;
                }
                default:
                    break;
            }
        });

        glfwSetScrollCallback(m_window, [](GLFWwindow* window, double x_offset, double y_offset) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            MouseScrolledEvent event((float)x_offset, (float)y_offset);
            data.event_callback(event);
        });

        glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double x_pos, double y_pos) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            MouseMovedEvent event((float)x_pos, (float)y_pos);
            data.event_callback(event);
        });
    }

    void WindowsWindow::shutdown() {
        glfwDestroyWindow(m_window);
    }

    void WindowsWindow::onUpdate() {
        glfwPollEvents();
        m_context->swapBuffers();
    }

    void WindowsWindow::setVSync(bool enabled) {
        if (enabled) {
            glfwSwapInterval(1);
        } else {
            glfwSwapInterval(0);
        }

        m_data.vSync = enabled;
    }

    bool WindowsWindow::isVSync() const {
        return m_data.vSync;
    }
}