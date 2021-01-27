#pragma once

#include "parrot/renderer/Buffer.h"

#include <memory>

namespace parrot {

    class VertexArray {
    public:
        virtual ~VertexArray() {};

        virtual void bind     () const = 0;
        virtual void unbind   () const = 0;
        virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertex_buffer) = 0;
        virtual void setIndexBuffer (const std::shared_ptr<IndexBuffer >& index_buffer ) = 0;

        virtual const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBufferList() const = 0;
        virtual const std::shared_ptr<IndexBuffer>&               getIndexBuffer     () const = 0;

        static VertexArray* create();
    };

}