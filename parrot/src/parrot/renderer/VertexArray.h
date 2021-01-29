#pragma once

#include "parrot/renderer/Buffer.h"

#include <memory>

namespace parrot {

    class VertexArray {
    public:
        virtual ~VertexArray() {};

        virtual void bind     () const = 0;
        virtual void unbind   () const = 0;
        virtual void addVertexBuffer(const Ref<VertexBuffer>& vertex_buffer) = 0;
        virtual void setIndexBuffer (const Ref<IndexBuffer >& index_buffer ) = 0;

        virtual const std::vector<Ref<VertexBuffer>>& getVertexBufferList() const = 0;
        virtual const Ref<IndexBuffer>&               getIndexBuffer     () const = 0;

        static Ref<VertexArray> create();
    };

}