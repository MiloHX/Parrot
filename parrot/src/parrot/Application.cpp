#include "prpch.h"
#include "Application.h"
#include "Input.h"

#include <glad/glad.h>

namespace parrot {

    // Init Singleton
    Application* Application::s_instance = nullptr;

    // Temporary
    static GLenum shaderDataTypeToOpenGLBaseType(ShaderDataType type) {
        switch (type) {
            case parrot::ShaderDataType::None  : return GL_FLOAT;
            case parrot::ShaderDataType::Float : return GL_FLOAT;
            case parrot::ShaderDataType::Float2: return GL_FLOAT;
            case parrot::ShaderDataType::Float3: return GL_FLOAT;
            case parrot::ShaderDataType::Float4: return GL_FLOAT;
            case parrot::ShaderDataType::Mat3  : return GL_FLOAT;
            case parrot::ShaderDataType::Mat4  : return GL_FLOAT;
            case parrot::ShaderDataType::Int   : return GL_INT;
            case parrot::ShaderDataType::Int2  : return GL_INT;
            case parrot::ShaderDataType::Int3  : return GL_INT;
            case parrot::ShaderDataType::Int4  : return GL_INT;
            case parrot::ShaderDataType::Bool  : return GL_BOOL;
        }
    }

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
        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
             0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f
        };
        m_vertex_buffer.reset(VertexBuffer::create(vertices, sizeof(vertices)));

        BufferLayout layout = {
            { ShaderDataType::Float3, "a_position" },
            { ShaderDataType::Float4, "a_color"    }
        };

        m_vertex_buffer->setLayout(layout);

        const auto& vertex_layout = m_vertex_buffer->getLayout();
        uint32_t index = 0;
        for (const auto& element : vertex_layout) {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(
                index, 
                element.getComponentCount(), 
                shaderDataTypeToOpenGLBaseType(element.type), 
                element.normalized ? GL_TRUE : GL_FALSE, 
                vertex_layout.getStride(), 
                (const void*)(element.offset)
            );
            ++index;
        }

        // Index Buffer
        uint32_t indices[3] = {
            0, 1, 2
        };
        m_index_buffer.reset(IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));

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

        m_shader.reset(new Shader(vertex_source, fragment_source));

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
            
            m_shader->bind();
            glBindVertexArray(m_vertex_array);
            glDrawElements(GL_TRIANGLES, m_index_buffer->getCount(), GL_UNSIGNED_INT, nullptr);

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
