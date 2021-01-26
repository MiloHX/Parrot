#include "prpch.h"
#include "Application.h"

#include <glad/glad.h>

namespace parrot {

    #define BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)

    // Init Singleton
    Application* Application::s_Instance = nullptr;

    Application::Application() {
        PR_INT_ASSERT(!s_Instance, "Application already exist!")
        s_Instance = this;
        m_window = std::unique_ptr<Window>(Window::create());
        m_window->setEventCallBack(BIND_EVENT_FUNC(onEvent));
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

            m_window->onUpdate();
        };
    }

    void Application::onEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(onWindowClose));

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
