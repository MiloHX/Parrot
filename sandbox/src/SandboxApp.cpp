#include <Parrot.h>

#include "platform/OpenGL/OpenGLShader.h"
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class ExmapleLayer : public parrot::Layer {
public:
    ExmapleLayer() : 
        Layer("Example"), 
        m_camera_controller(1920.0f / 1080.0f, true)  {

        // Triangle Vertex Array
        m_tr_vertex_array.reset(parrot::VertexArray::create());

        // Triangle Vertex Data
        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
             0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f
        };

        // Triangle Vertex Buffer
        parrot::Ref<parrot::VertexBuffer> tr_vertex_buffer;
        tr_vertex_buffer.reset(parrot::VertexBuffer::create(vertices, sizeof(vertices)));
        tr_vertex_buffer->setLayout({
            { parrot::ShaderDataType::Float3, "a_position" },
            { parrot::ShaderDataType::Float4, "a_color"    }
        });
        m_tr_vertex_array->addVertexBuffer(tr_vertex_buffer);

        // Triangle Index Data
        uint32_t indices[3] = {
            0, 1, 2
        };

        // Triangle Index Buffer
        parrot::Ref<parrot::IndexBuffer> tr_index_buffer;
        tr_index_buffer.reset(parrot::IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_tr_vertex_array->setIndexBuffer(tr_index_buffer);

        // Triangle Shaders
        std::string vertex_source = R"(
            #version 330 core
            layout(location = 0) in vec3 a_position;
            layout(location = 1) in vec4 a_color;

            uniform mat4 u_view_projection;
            uniform mat4 u_transform;
        
            out vec3 v_position;
            out vec4 v_color;

            void main() {
                v_position  = a_position;
                v_color     = a_color;
                gl_Position = u_view_projection * u_transform * vec4(a_position, 1.0);
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

        m_tr_shader = parrot::Shader::create("tr_shader", vertex_source, fragment_source);

        // Square Vertex Array
        m_sq_vertex_array.reset(parrot::VertexArray::create());

        // Square Vertex Data
        float sq_vertices[5 * 4] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
        };

        // Square Vertex Buffer
        parrot::Ref<parrot::VertexBuffer> sq_vertex_buffer;
        sq_vertex_buffer.reset(parrot::VertexBuffer::create(sq_vertices, sizeof(sq_vertices)));
        sq_vertex_buffer->setLayout({
            { parrot::ShaderDataType::Float3, "a_position"   },
            { parrot::ShaderDataType::Float2, "a_text_coord" },
        });
        m_sq_vertex_array->addVertexBuffer(sq_vertex_buffer);

        // Square Index Data
        uint32_t sq_indices[6] = {
            0, 1, 2, 2, 3, 0
        };

        // Triangle Index Buffer
        parrot::Ref<parrot::IndexBuffer> sq_index_buffer;
        sq_index_buffer.reset(parrot::IndexBuffer::create(sq_indices, sizeof(sq_indices) / sizeof(uint32_t)));
        m_sq_vertex_array->setIndexBuffer(sq_index_buffer);

        // Square Shaders
        std::string sq_vertex_source = R"(
            #version 330 core
            layout(location = 0) in vec3 a_position;
    
            uniform mat4 u_view_projection;
            uniform mat4 u_transform;

            out vec3 v_position;

            void main() {
                v_position  = a_position;
                gl_Position = u_view_projection * u_transform * vec4(a_position, 1.0);
            }
        )";

        std::string sq_fragment_source = R"(
            #version 330 core
            layout(location = 0) out vec4 color;

            uniform vec3 u_color;

            in vec3 v_position;
            void main() {
                color = vec4(u_color, 1.0);
            }
        )";
        m_sq_shader = parrot::Shader::create("sq_shader", sq_vertex_source, sq_fragment_source);

        // Texture Shaders
        //m_tx_shader = parrot::Shader::create("asset/shader/texture_shader.glsl");
        auto tx_shader =  m_shader_library.load("asset/shader/texture_shader.glsl");

        m_texture      = parrot::Texture2D::create("asset/texture/Checkerboard.png");
        m_logo_texture = parrot::Texture2D::create("asset/texture/icon64.png"      );

        std::dynamic_pointer_cast<parrot::OpenGLShader>(tx_shader)->bind();
        std::dynamic_pointer_cast<parrot::OpenGLShader>(tx_shader)->uploadUniformInt("u_texture", 0);
    }

    void onUpdate(parrot::TimeStep time_step) override {

        // Update
        m_camera_controller.onUpdate(time_step);

        // Render
        parrot::RenderCommand::setClearColor(glm::vec4{ 0.3f, 0.3f, 0.3f, 1.0f });
        parrot::RenderCommand::clear();

        parrot::Renderer::beginScene(m_camera_controller.getCamera());

        glm::mat4 sq_scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

        glm::vec4 red_color (1.0f, 0.3f, 0.3f, 1.0f);
        glm::vec4 blue_color(0.3f, 0.3f, 1.0f, 1.0f);

        std::dynamic_pointer_cast<parrot::OpenGLShader>(m_sq_shader)->bind();
        std::dynamic_pointer_cast<parrot::OpenGLShader>(m_sq_shader)->uploadUniformFloat3("u_color", m_sq_color);

        for (int i = 0; i < 20; ++i) {
            for (int j = 0; j < 20; ++j) {
                glm::vec3 pos(j * 0.11f - 1.1f, i * 0.11f - 1.1f, 0.0f);
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * sq_scale;
                parrot::Renderer::submit(m_sq_shader, m_sq_vertex_array, transform);
            }
        }

        auto tx_shader = m_shader_library.get("texture_shader");

        m_texture->bind();
        glm::mat4 tx_scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.5f));
        parrot::Renderer::submit(tx_shader, m_sq_vertex_array, glm::mat4(1.0f) * tx_scale);
        m_logo_texture->bind();
        parrot::Renderer::submit(tx_shader, m_sq_vertex_array, glm::mat4(1.0f) * tx_scale);


        //parrot::Renderer::submit(m_tr_shader, m_tr_vertex_array);
        parrot::Renderer::endScene();

    }

    void onImGuiRender() override {
        ImGui::Begin("UNIFORMS");
        ImGui::ColorEdit3("Square Color", glm::value_ptr(m_sq_color));
        ImGui::End();
    }

    void onEvent(parrot::Event& event) override {
        m_camera_controller.onEvent(event);
    }

private:
    parrot::ShaderLibrary                m_shader_library;
    parrot::Ref<parrot::VertexArray>     m_tr_vertex_array;
    parrot::Ref<parrot::VertexArray>     m_sq_vertex_array;
    parrot::Ref<parrot::Shader     >     m_tr_shader;
    parrot::Ref<parrot::Shader     >     m_sq_shader;
    // parrot::Ref<parrot::Shader     >     m_tx_shader;
    parrot::Ref<parrot::Texture2D  >     m_texture;
    parrot::Ref<parrot::Texture2D  >     m_logo_texture;
    parrot::OrthographicCameraController m_camera_controller;

    glm::vec3                            m_sq_color = glm::vec3{ 0.2f, 0.3f, 0.8f };
};

class Sandbox : public parrot::Application {
public:
    Sandbox() {
        pushLayer(new ExmapleLayer());
    }

    ~Sandbox() {
    }
};

// Return Application to be run on Parrot Engine
parrot::Application* parrot::createApplication() {
    return new Sandbox();
}