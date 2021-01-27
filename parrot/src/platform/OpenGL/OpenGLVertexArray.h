#pragma once

#include "parrot/renderer/VertexArray.h"

namespace parrot {

    class OpenGLVertexArray : public VertexArray {
    public:
        OpenGLVertexArray();
        virtual ~OpenGLVertexArray();

        virtual void bind     () const override;
        virtual void unbind   () const override;

        virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertex_buffer) override;
        virtual void setIndexBuffer (const std::shared_ptr<IndexBuffer >& index_buffer ) override;

        virtual const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBufferList() const override { return m_vertex_buffer_list; }
        virtual const std::shared_ptr<IndexBuffer>&               getIndexBuffer     () const override { return m_index_buffer      ; }

    private:
        uint32_t m_renderer_id;
        std::vector<std::shared_ptr<VertexBuffer>> m_vertex_buffer_list;
        std::shared_ptr<IndexBuffer>               m_index_buffer;
    };

}