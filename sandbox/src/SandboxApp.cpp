#include <Parrot.h>

#include <imgui.h>

class ExmapleLayer : public parrot::Layer {
public:
    ExmapleLayer() : Layer("Example"), m_camera(-1.6f, 1.6f, -0.9, 0.9f), m_camera_position(0.0f), m_camera_rotation(0.0f) {

        // Triangle Vertex Array
        m_tr_vertex_array.reset(parrot::VertexArray::create());

        // Triangle Vertex Data
        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
             0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f
        };

        // Triangle Vertex Buffer
        std::shared_ptr<parrot::VertexBuffer> tr_vertex_buffer;
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
        std::shared_ptr<parrot::IndexBuffer> tr_index_buffer;
        tr_index_buffer.reset(parrot::IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_tr_vertex_array->setIndexBuffer(tr_index_buffer);

        // Triangle Shaders
        std::string vertex_source = R"(
            #version 330 core
            layout(location = 0) in vec3 a_position;
            layout(location = 1) in vec4 a_color;

            uniform mat4 u_view_projection;
        
            out vec3 v_position;
            out vec4 v_color;

            void main() {
                v_position  = a_position;
                v_color     = a_color;
                gl_Position = u_view_projection * vec4(a_position, 1.0);
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

        m_tr_shader.reset(new parrot::Shader(vertex_source, fragment_source));

        // Square Vertex Array
        m_sq_vertex_array.reset(parrot::VertexArray::create());

        // Square Vertex Data
        float sq_vertices[3 * 4] = {
            -0.75f, -0.75f, 0.0f,
             0.75f, -0.75f, 0.0f,
             0.75f,  0.75f, 0.0f,
            -0.75f,  0.75f, 0.0f
        };

        // Square Vertex Buffer
        std::shared_ptr<parrot::VertexBuffer> sq_vertex_buffer;
        sq_vertex_buffer.reset(parrot::VertexBuffer::create(sq_vertices, sizeof(sq_vertices)));
        sq_vertex_buffer->setLayout({
            { parrot::ShaderDataType::Float3, "a_position" },
        });
        m_sq_vertex_array->addVertexBuffer(sq_vertex_buffer);

        // Square Index Data
        uint32_t sq_indices[6] = {
            0, 1, 2, 2, 3, 0
        };

        // Triangle Index Buffer
        std::shared_ptr<parrot::IndexBuffer> sq_index_buffer;
        sq_index_buffer.reset(parrot::IndexBuffer::create(sq_indices, sizeof(sq_indices) / sizeof(uint32_t)));
        m_sq_vertex_array->setIndexBuffer(sq_index_buffer);

        // Square Shaders
        std::string sq_vertex_source = R"(
            #version 330 core
            layout(location = 0) in vec3 a_position;
    
            uniform mat4 u_view_projection;

            out vec3 v_position;

            void main() {
                v_position  = a_position;
                gl_Position = u_view_projection * vec4(a_position, 1.0);
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
        m_sq_shader.reset(new parrot::Shader(sq_vertex_source, sq_fragment_source));
    }

    void onUpdate(parrot::TimeStep time_step) override {

        if (parrot::Input::isKeyPressed(PR_KEY_A)) {
            m_camera_position.x -= m_camera_move_speed * time_step;
        }else if (parrot::Input::isKeyPressed(PR_KEY_D)) {
            m_camera_position.x += m_camera_move_speed * time_step;
        }

        if (parrot::Input::isKeyPressed(PR_KEY_W)) {
            m_camera_position.y += m_camera_move_speed * time_step;
        } else if (parrot::Input::isKeyPressed(PR_KEY_S)) {
            m_camera_position.y -= m_camera_move_speed * time_step;
        }

        if (parrot::Input::isKeyPressed(PR_KEY_LEFT)) {
            m_camera_rotation += m_camera_rotation_speed * time_step;
        } else if (parrot::Input::isKeyPressed(PR_KEY_RIGHT)) {
            m_camera_rotation -= m_camera_rotation_speed * time_step;
        }

        parrot::RenderCommand::setClearColor(glm::vec4{ 0.3f, 0.3f, 0.3f, 1.0f });
        parrot::RenderCommand::clear();

        m_camera.setPosition (m_camera_position);
        m_camera.setZRotation(m_camera_rotation);

        parrot::Renderer::beginScene(m_camera);
        parrot::Renderer::submit(m_sq_shader, m_sq_vertex_array);
        parrot::Renderer::submit(m_tr_shader, m_tr_vertex_array);
        parrot::Renderer::endScene();

    }

    void onImGuiRender() override {
    }

    void onEvent(parrot::Event& event) override {
        parrot::EventDispatcher dispatcher(event);
        dispatcher.dispatch<parrot::KeyPressedEvent>(PR_BIND_EVENT_FUNC(ExmapleLayer::onKeyPressedEvent));
    }

    bool onKeyPressedEvent(parrot::KeyPressedEvent& event) {
        return false;
    }

private:

    std::shared_ptr<parrot::VertexArray> m_tr_vertex_array;
    std::shared_ptr<parrot::VertexArray> m_sq_vertex_array;
    std::shared_ptr<parrot::Shader     > m_tr_shader;
    std::shared_ptr<parrot::Shader     > m_sq_shader;
    parrot::OrthographicCamera           m_camera;
    glm::vec3                            m_camera_position;
    float                                m_camera_move_speed = 1.0f;
    float                                m_camera_rotation;
    float                                m_camera_rotation_speed = 20.0f;
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