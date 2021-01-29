#include "prpch.h"
#include "platform/OpenGL/OpenGLVertexArray.h"

#include <glad/glad.h>

namespace parrot {

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

        return GL_FLOAT;
    }

    OpenGLVertexArray::OpenGLVertexArray() {
        glCreateVertexArrays(1, &m_renderer_id);
    }

    OpenGLVertexArray::~OpenGLVertexArray() {
        glDeleteVertexArrays(1, &m_renderer_id);
    }

    void OpenGLVertexArray::bind() const {
        glBindVertexArray(m_renderer_id);
    }

    void OpenGLVertexArray::unbind() const {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::addVertexBuffer(const Ref<VertexBuffer>& vertex_buffer) {
        PR_CORE_ASSERT(vertex_buffer->getLayout().getElementList().size(), "Vertex buffer has no layout")

        glBindVertexArray(m_renderer_id);
        vertex_buffer->bind();

        const auto& vertex_layout = vertex_buffer->getLayout();
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
        m_vertex_buffer_list.push_back(vertex_buffer);
    }

    void OpenGLVertexArray::setIndexBuffer(const Ref<IndexBuffer>& index_buffer) {
        glBindVertexArray(m_renderer_id);
        index_buffer->bind();

        m_index_buffer = index_buffer;
    }
}