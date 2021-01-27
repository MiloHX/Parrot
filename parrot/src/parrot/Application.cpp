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

        // Vertex Array
        glGenVertexArrays(1, &m_vertex_array);
        glBindVertexArray(m_vertex_array);

        // Vertex Buffer
        glGenBuffers(1, &m_vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);

        float vertices[3 * 3] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.0f,  0.5f, 0.0f
        };

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        // Index Buffer
        glGenBuffers(1, &m_index_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);

        unsigned int indices[3] = {
            0, 1, 2
        };

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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
            glClearColor(0.3f, 0.3f, 0.3f, 1);
            glClear(GL_COLOR_BUFFER_BIT);
            
            glBindVertexArray(m_vertex_array);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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
