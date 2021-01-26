#include "prpch.h"
#include "Application.h"
#include "Input.h"

#include <glad/glad.h>

namespace parrot {

    // Init Singleton
    Application* Application::s_instance = nullptr;

    Application::Application() {
        PR_INT_ASSERT(!s_instance, "Application already exist!")
        s_instance = this;
        m_window = std::unique_ptr<Window>(Window::create());
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
            glClearColor(1, 1, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT);
            
            for (Layer* layer : m_layer_stack) {
                layer->onUpdate();
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
