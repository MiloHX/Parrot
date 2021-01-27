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

        // Window Creation & Adding imgui layer
        m_window = std::unique_ptr<Window>(Window::create());
        m_window->setEventCallBack(PR_BIND_EVENT_FUNC(Application::onEvent));
        m_imgui_layer = new ImGuiLayer();
        pushOverlay(m_imgui_layer);

        // Triangle Vertex Array
        m_tr_vertex_array.reset(VertexArray::create());

        // Triangle Vertex Data
        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
             0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f
        };

        // Triangle Vertex Buffer
        std::shared_ptr<VertexBuffer> tr_vertex_buffer;
        tr_vertex_buffer.reset(VertexBuffer::create(vertices, sizeof(vertices)));
        tr_vertex_buffer->setLayout({
            { ShaderDataType::Float3, "a_position" },
            { ShaderDataType::Float4, "a_color"    }
        });
        m_tr_vertex_array->addVertexBuffer(tr_vertex_buffer);

        // Triangle Index Data
        uint32_t indices[3] = {
            0, 1, 2
        };

        // Triangle Index Buffer
        std::shared_ptr<IndexBuffer> tr_index_buffer;
        tr_index_buffer.reset(IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_tr_vertex_array->setIndexBuffer(tr_index_buffer);

        // Triangle Shaders
        std::string vertex_source = R"(
            #version 330 core
            layout(location = 0) in vec3 a_position;
            layout(location = 1) in vec4 a_color;

            out vec3 v_position;
            out vec4 v_color;

            void main() {
                v_position  = a_position;
                v_color     = a_color;
                gl_Position = vec4(a_position, 1.0);
            }
        )";

        std::string fragment_source = R"(
            #version 330 core
            layout(location = 0) out vec4 color;
            in vec3 v_position;
            in vec4 v_color;
            void main() {
                color = vec4(v_position * 0.5 + 0.5, 1.0);
                color = v_color;
            }
        )";           

        m_tr_shader.reset(new Shader(vertex_source, fragment_source));

        // Square Vertex Array
        m_sq_vertex_array.reset(VertexArray::create());

        // Square Vertex Data
        float sq_vertices[3 * 4] = {
            -0.75f, -0.75f, 0.0f,
             0.75f, -0.75f, 0.0f,
             0.75f,  0.75f, 0.0f,
            -0.75f,  0.75f, 0.0f
        };

        // Square Vertex Buffer
        std::shared_ptr<VertexBuffer> sq_vertex_buffer;
        sq_vertex_buffer.reset(VertexBuffer::create(sq_vertices, sizeof(sq_vertices)));
        sq_vertex_buffer->setLayout({
            { ShaderDataType::Float3, "a_position" },
        });
        m_sq_vertex_array->addVertexBuffer(sq_vertex_buffer);

        // Square Index Data
        uint32_t sq_indices[6] = {
            0, 1, 2, 2, 3, 0
        };

        // Triangle Index Buffer
        std::shared_ptr<IndexBuffer> sq_index_buffer;
        sq_index_buffer.reset(IndexBuffer::create(sq_indices, sizeof(sq_indices) / sizeof(uint32_t)));
        m_sq_vertex_array->setIndexBuffer(sq_index_buffer);

        // Square Shaders
        std::string sq_vertex_source = R"(
            #version 330 core
            layout(location = 0) in vec3 a_position;

            out vec3 v_position;

            void main() {
                v_position  = a_position;
                gl_Position = vec4(a_position, 1.0);
            }
        )";

        std::string sq_fragment_source = R"(
            #version 330 core
            layout(location = 0) out vec4 color;
            in vec3 v_position;
            void main() {
                color = vec4(0.3, 0.3, 1.0, 1.0);
            }
        )";
        m_sq_shader.reset(new Shader(sq_vertex_source, sq_fragment_source));
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
            
            m_sq_shader->bind();
            m_sq_vertex_array->bind();
            glDrawElements(GL_TRIANGLES, m_sq_vertex_array->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);

            m_tr_shader->bind();
            m_tr_vertex_array->bind();
            glDrawElements(GL_TRIANGLES, m_tr_vertex_array->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);

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
