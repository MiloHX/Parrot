#include "prpch.h"
#include "parrot/renderer/Renderer2D.h"
#include "parrot/renderer/RenderCommand.h"
#include "parrot/renderer/VertexArray.h"
#include "parrot/renderer/Shader.h"

namespace parrot {

    struct Renderer2Data {
        Ref<VertexArray>     vertex_array;
        Ref<Shader>          flat_color_shader;
    };

    static Renderer2Data* s_renderer_2D_data;

    void Renderer2D::init() {
        s_renderer_2D_data = new Renderer2Data();
        // Triangle Vertex Array
        s_renderer_2D_data->vertex_array = VertexArray::create();

        float vertices[3 * 4] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.5f,  0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f
        };

        Ref<parrot::VertexBuffer> vertex_buffer;
        vertex_buffer = VertexBuffer::create(vertices, sizeof(vertices));
        vertex_buffer->setLayout({
            { ShaderDataType::Float3, "a_position" }
                                 });
        s_renderer_2D_data->vertex_array->addVertexBuffer(vertex_buffer);

        uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };

        Ref<IndexBuffer> index_buffer;
        index_buffer = IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t));
        s_renderer_2D_data->vertex_array->setIndexBuffer(index_buffer);

        s_renderer_2D_data->flat_color_shader = Shader::create("asset/shader/flat_color_shader.glsl");
    }

    void Renderer2D::shutdown() {
        delete s_renderer_2D_data;
    }

    void Renderer2D::beginScene(const OrthographicCamera camera) {
        s_renderer_2D_data->flat_color_shader->bind();
        s_renderer_2D_data->flat_color_shader->setMat4("u_view_projection", camera.getViewPorjectionMatrix());
    }

    void Renderer2D::endScene() {
    }

    void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
        s_renderer_2D_data->flat_color_shader->bind();
        s_renderer_2D_data->flat_color_shader->setFloat4("u_color", color);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3{ size.x, size.y, 1.0f });
        s_renderer_2D_data->flat_color_shader->setMat4("u_transform", transform);


        s_renderer_2D_data->vertex_array->bind();
        RenderCommand::drawIndexed(s_renderer_2D_data->vertex_array);
    }
}