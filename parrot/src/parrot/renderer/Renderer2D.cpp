#include "prpch.h"
#include "parrot/renderer/Renderer2D.h"
#include "parrot/renderer/RenderCommand.h"
#include "parrot/renderer/VertexArray.h"
#include "parrot/renderer/Shader.h"

namespace parrot {

    struct Renderer2Data {
        Ref<VertexArray> vertex_array;
        Ref<Shader>      texture_shader;
        Ref<Texture2D>   white_texture;
    };

    static Renderer2Data* s_data;

    void Renderer2D::init() {
        s_data = new Renderer2Data();
        // Triangle Vertex Array
        s_data->vertex_array = VertexArray::create();

        float vertices[5 * 4] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
        };

        Ref<parrot::VertexBuffer> vertex_buffer;
        vertex_buffer = VertexBuffer::create(vertices, sizeof(vertices));
        vertex_buffer->setLayout({
            { ShaderDataType::Float3, "a_position"  },
            { ShaderDataType::Float2, "a_tex_coord" }
        });
        s_data->vertex_array->addVertexBuffer(vertex_buffer);

        uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };

        Ref<IndexBuffer> index_buffer;
        index_buffer = IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t));
        s_data->vertex_array->setIndexBuffer(index_buffer);

        s_data->white_texture = Texture2D::create(1, 1);
        uint32_t white_texture_data = 0xffffffff;
        s_data->white_texture->setData(&white_texture_data, sizeof(uint32_t));

        s_data->texture_shader = Shader::create("asset/shader/texture_shader.glsl"   );
        s_data->texture_shader->bind();
        s_data->texture_shader->setInt("u_texture", 0);
    }

    void Renderer2D::shutdown() {
        delete s_data;
    }

    void Renderer2D::beginScene(const OrthographicCamera camera) {
        s_data->texture_shader->bind();
        s_data->texture_shader->setMat4("u_view_projection", camera.getViewPorjectionMatrix());
    }

    void Renderer2D::endScene() {
    }

    void Renderer2D::drawQuad(
        const glm::vec3&      position, 
        const glm::vec2&      size, 
        float                 rotation, 
        const Ref<Texture2D>& texture, 
        const glm::vec4&      color, 
        const glm::vec2&      texture_scale
    ) {
        s_data->texture_shader->setFloat4("u_color", color);
        if (texture) {
            texture->bind();
        } else {
            s_data->white_texture->bind();
        }
        glm::mat4 transform;
        if (rotation == 0) {
            transform =
                glm::translate(glm::mat4(1.0f), position) *
                glm::scale(glm::mat4(1.0f), glm::vec3{ size.x, size.y, 1.0f });
        } else {
            transform =
                glm::translate(glm::mat4(1.0f), position) *
                glm::rotate(glm::mat4(1.0f), rotation, glm::vec3{ 0.0f, 0.0f, 1.0f }) *
                glm::scale(glm::mat4(1.0f), glm::vec3{ size.x, size.y, 1.0f });
        }
        s_data->texture_shader->setMat4("u_transform", transform);
        s_data->texture_shader->setFloat2("u_texture_scale", texture_scale);

        s_data->vertex_array->bind();
        RenderCommand::drawIndexed(s_data->vertex_array);
    }
}