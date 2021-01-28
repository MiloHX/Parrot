#include "prpch.h"
#include "Application.h"
#include "Input.h"
#include "parrot/renderer/Renderer.h"

#include "GLFW/glfw3.h"

namespace parrot {

    // Init Singleton
    Application* Application::s_instance = nullptr;

    Application::Application() {
        PR_CORE_ASSERT(!s_instance, "Application already exist!")
        s_instance = this;

        // Window Creation & Adding imgui layer
        m_window = Ref<Window>(Window::create());
        m_window->setEventCallBack(PR_BIND_EVENT_FUNC(Application::onEvent));
        m_imgui_layer = new ImGuiLayer();
        pushOverlay(m_imgui_layer);
    }

    Application::~Application() {
    }

    void Application::pushLayer(Layer *layer) {
        m_layer_stack.pushLayer(layer);
        layer->onAttach();
    }

    void Application::pushOverlay(Layer *overlay) {
        m_layer_stack.pushOverlay(overlay);
        overlay->onAttach();
    }

    void Application::run() {

        while (m_running) {
            float time = static_cast<float>(glfwGetTime());  // Should be platform specific
            m_time_step       = time - m_last_frame_time;
            m_last_frame_time = time;

            for (Layer* layer : m_layer_stack) {
                layer->onUpdate(m_time_step);
            }

            m_imgui_layer->begin();
                for (Layer* layer : m_layer_stack) {
                    layer->onImGuiRender();
                }
            m_imgui_layer->end();

            m_window->onUpdate();
        };
    }

    void Application::onEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<WindowCloseEvent>(PR_BIND_EVENT_FUNC(Application::onWindowClose));

        for (auto it = m_layer_stack.end(); it != m_layer_stack.begin();) {
            (*--it)->onEvent(e);
            if (e.isHandled()) {
                break;
            }
        }
    }

    bool Application::onWindowClose(Event& e) {
        m_running = false;
        return true;
    }

}
