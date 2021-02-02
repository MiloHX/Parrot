#include "prpch.h"
#include "parrot/renderer/Renderer2D.h"
#include "parrot/renderer/RenderCommand.h"
#include "parrot/renderer/VertexArray.h"
#include "parrot/renderer/Shader.h"

namespace parrot {

    struct QuadVertex {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 tex_coord;
        float     tex_index;
        glm::vec2 tex_scale;
    };

    struct Renderer2Data {
        const uint32_t max_quads_per_draw = 16384;
        const uint32_t max_quad_vertices_per_draw = max_quads_per_draw * 4;
        const uint32_t max_quad_indices_per_draw  = max_quads_per_draw * 6;

        static const uint32_t s_max_texture_slots = 32;

        Ref<VertexArray>  vertex_array;
        Ref<VertexBuffer> quad_vertex_buffer;
        Ref<Shader>       quad_texture_shader;
        Ref<Texture2D>    white_texture;

        uint32_t    quad_index_count      = 0;
        QuadVertex* quad_vertices         = nullptr;
        QuadVertex* quad_vertices_pointer = nullptr;

        std::array<Ref<Texture2D>, s_max_texture_slots> texture_slots;

        uint32_t texture_slot_index = 1;

        glm::vec4 quad_vertex_positions[4];
    };

    static Renderer2Data s_data;

    void Renderer2D::init() {
        // Vertex Array
        s_data.vertex_array = VertexArray::create();

        s_data.quad_vertex_buffer = VertexBuffer::create(s_data.max_quad_vertices_per_draw * sizeof(QuadVertex));
        s_data.quad_vertex_buffer->setLayout({
            { ShaderDataType::Float3, "a_position"  },
            { ShaderDataType::Float4, "a_color"     },
            { ShaderDataType::Float2, "a_tex_coord" },
            { ShaderDataType::Float , "a_tex_index" },
            { ShaderDataType::Float2, "a_tex_scale" }
        });
        s_data.vertex_array->addVertexBuffer(s_data.quad_vertex_buffer);

        s_data.quad_vertices = new QuadVertex[s_data.max_quad_vertices_per_draw];

        uint32_t* quad_indices = new uint32_t[s_data.max_quad_indices_per_draw];
        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_data.max_quad_indices_per_draw; i += 6) {
            quad_indices[i + 0] = offset + 0;
            quad_indices[i + 1] = offset + 1;
            quad_indices[i + 2] = offset + 2;
            quad_indices[i + 3] = offset + 2;
            quad_indices[i + 4] = offset + 3;
            quad_indices[i + 5] = offset + 0;

            offset += 4;
        }

        Ref<IndexBuffer> index_buffer;
        index_buffer = IndexBuffer::create(quad_indices, s_data.max_quad_indices_per_draw);
        s_data.vertex_array->setIndexBuffer(index_buffer);
        delete[] quad_indices;

        s_data.white_texture = Texture2D::create(1, 1);
        uint32_t white_texture_data = 0xffffffff;
        s_data.white_texture->setData(&white_texture_data, sizeof(uint32_t));
        s_data.texture_slots[0] = s_data.white_texture;

        int32_t texture_samplers[s_data.s_max_texture_slots];
        for (int i = 0; i < s_data.s_max_texture_slots; ++i) {
            texture_samplers[i] = i;
        }

        s_data.quad_texture_shader = Shader::create("asset/shader/texture_shader.glsl"   );
        s_data.quad_texture_shader->bind();
        s_data.quad_texture_shader->setIntArray("u_textures", texture_samplers, s_data.s_max_texture_slots);

        s_data.quad_vertex_positions[0] = glm::vec4{ -0.5f, -0.5f, 0.0f, 1.0f };
        s_data.quad_vertex_positions[1] = glm::vec4{  0.5f, -0.5f, 0.0f, 1.0f };
        s_data.quad_vertex_positions[2] = glm::vec4{  0.5f,  0.5f, 0.0f, 1.0f };
        s_data.quad_vertex_positions[3] = glm::vec4{ -0.5f,  0.5f, 0.0f, 1.0f };
    }

    void Renderer2D::shutdown() {
    }

    void Renderer2D::beginScene(const OrthographicCamera camera) {
        s_data.quad_texture_shader->bind();
        s_data.quad_texture_shader->setMat4("u_view_projection", camera.getViewPorjectionMatrix());

        s_data.quad_index_count = 0;
        s_data.quad_vertices_pointer = s_data.quad_vertices;
        s_data.texture_slot_index = 1;
    }

    void Renderer2D::endScene() {
        uint32_t data_size = (uint8_t*)s_data.quad_vertices_pointer - (uint8_t*)s_data.quad_vertices;
        s_data.quad_vertex_buffer->setData(s_data.quad_vertices, data_size);
        flush();
    }

    void Renderer2D::flush(){
        for (int i = 0; i < s_data.texture_slot_index; ++i) {
            s_data.texture_slots[i]->bind(i);
        }
            
        RenderCommand::drawIndexed(s_data.vertex_array, s_data.quad_index_count);
    }

    void Renderer2D::drawQuad(
        const glm::vec3&      position, 
        const glm::vec2&      size, 
        float                 rotation, 
        const Ref<Texture2D>& texture, 
        const glm::vec4&      color, 
        const glm::vec2&      texture_scale
    ) {
        float texture_index = 0.0f;

        if (texture) {
            for (int i = 1; i < s_data.texture_slot_index; i++) {
                if (*s_data.texture_slots[i].get() == *texture.get()) {
                    texture_index = static_cast<float>(i);
                    break;
                }
            }
            if (texture_index == 0.0f) {
                texture_index = (float)s_data.texture_slot_index;
                s_data.texture_slots[texture_index] = texture;
                ++s_data.texture_slot_index;
            }
        } 

        glm::mat4 transform;
        if (rotation) {
            if (size.x != 1.0f || size.y != 1.0f) {
                transform = glm::translate(glm::mat4(1.0f), position)
                                    * glm::rotate   (glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f))
                                    * glm::scale    (glm::mat4(1.0f), glm::vec3{ size.x, size.y, 1.0f });
            } else {
                transform = glm::translate(glm::mat4(1.0f), position)
                          * glm::scale    (glm::mat4(1.0f), glm::vec3{ size.x, size.y, 1.0f });
            }
        } else {
            if (size.x != 1.0f || size.y != 1.0f) {
                transform = glm::translate(glm::mat4(1.0f), position)
                          * glm::scale    (glm::mat4(1.0f), glm::vec3{ size.x, size.y, 1.0f });
            } else {
                transform = glm::translate(glm::mat4(1.0f), position);
            }
        } 

        s_data.quad_vertices_pointer->position  = glm::vec3(transform * s_data.quad_vertex_positions[0]);
        s_data.quad_vertices_pointer->color     = color;
        s_data.quad_vertices_pointer->tex_coord = glm::vec2{ 0.0f, 0.0f };
        s_data.quad_vertices_pointer->tex_index = texture_index;
        s_data.quad_vertices_pointer->tex_scale = texture_scale;
        s_data.quad_vertices_pointer++;

        s_data.quad_vertices_pointer->position  = glm::vec3(transform * s_data.quad_vertex_positions[1]);
        s_data.quad_vertices_pointer->color     = color;
        s_data.quad_vertices_pointer->tex_coord = glm::vec2{ 1.0f, 0.0f };
        s_data.quad_vertices_pointer->tex_index = texture_index;
        s_data.quad_vertices_pointer->tex_scale = texture_scale;
        s_data.quad_vertices_pointer++;

        s_data.quad_vertices_pointer->position  = glm::vec3(transform * s_data.quad_vertex_positions[2]);
        s_data.quad_vertices_pointer->color     = color;
        s_data.quad_vertices_pointer->tex_coord = glm::vec2{ 1.0f, 1.0f };
        s_data.quad_vertices_pointer->tex_index = texture_index;
        s_data.quad_vertices_pointer->tex_scale = texture_scale;
        s_data.quad_vertices_pointer++;

        s_data.quad_vertices_pointer->position  = glm::vec3(transform * s_data.quad_vertex_positions[3]);
        s_data.quad_vertices_pointer->color     = color;
        s_data.quad_vertices_pointer->tex_coord = glm::vec2{ 0.0f, 1.0f };
        s_data.quad_vertices_pointer->tex_index = texture_index;
        s_data.quad_vertices_pointer->tex_scale = texture_scale;
        s_data.quad_vertices_pointer++;

        s_data.quad_index_count +=6;

    }
}