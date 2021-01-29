#include "Sandbox2D.h"
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "platform/OpenGL/OpenGLShader.h" // to be removed

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"),  m_camera_controller(1920.0f / 1080.0f, true) {
}

void Sandbox2D::onAttach() {
    // Triangle Vertex Array
    m_vertex_array = parrot::VertexArray::create();

    float vertices[3 * 4] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    parrot::Ref<parrot::VertexBuffer> vertex_buffer;
    vertex_buffer = parrot::VertexBuffer::create(vertices, sizeof(vertices));
    vertex_buffer->setLayout({
        { parrot::ShaderDataType::Float3, "a_position" }
    });
    m_vertex_array->addVertexBuffer(vertex_buffer);

    uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };

    parrot::Ref<parrot::IndexBuffer> index_buffer;
    index_buffer = parrot::IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t));
    m_vertex_array->setIndexBuffer(index_buffer);

    m_flat_color_shader = parrot::Shader::create("asset/shader/flat_color_shader.glsl");
}

void Sandbox2D::onDetach() {
}

void Sandbox2D::onUpdate(parrot::TimeStep time_step) {
    m_camera_controller.onUpdate(time_step);

    // Render
    parrot::RenderCommand::setClearColor(glm::vec4{ 0.3f, 0.3f, 0.3f, 1.0f });
    parrot::RenderCommand::clear();

    parrot::Renderer::beginScene(m_camera_controller.getCamera());

    std::dynamic_pointer_cast<parrot::OpenGLShader>(m_flat_color_shader)->bind();
    std::dynamic_pointer_cast<parrot::OpenGLShader>(m_flat_color_shader)->uploadUniformFloat4("u_color", m_color);

    parrot::Renderer::submit(m_flat_color_shader, m_vertex_array, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

    parrot::Renderer::endScene();
}

void Sandbox2D::onImGuiRender() {
    ImGui::Begin("UNIFORMS");
    ImGui::ColorEdit3("Square Color", glm::value_ptr(m_color));
    ImGui::End();
}

void Sandbox2D::onEvent(parrot::Event& event) {
    m_camera_controller.onEvent(event);
}
