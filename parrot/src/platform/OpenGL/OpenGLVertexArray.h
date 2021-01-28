#pragma once

#include "parrot/renderer/VertexArray.h"

namespace parrot {

    class OpenGLVertexArray : public VertexArray {
    public:
        OpenGLVertexArray();
        virtual ~OpenGLVertexArray();

        virtual void bind     () const override;
        virtual void unbind   () const override;

        virtual void addVertexBuffer(const Ref<VertexBuffer>& vertex_buffer) override;
        virtual void setIndexBuffer (const Ref<IndexBuffer >& index_buffer ) override;

        virtual const std::vector<Ref<VertexBuffer>>& getVertexBufferList() const override { return m_vertex_buffer_list; }
        virtual const Ref<IndexBuffer>&               getIndexBuffer     () const override { return m_index_buffer      ; }

    private:
        uint32_t m_renderer_id;
        std::vector<Ref<VertexBuffer>> m_vertex_buffer_list;
        Ref<IndexBuffer>               m_index_buffer;
    };

}