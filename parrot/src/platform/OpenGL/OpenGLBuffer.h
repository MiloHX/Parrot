#pragma once

#include "parrot/renderer/Buffer.h"

namespace parrot {

    class OpenGLVertexBuffer : public VertexBuffer {
    public:
        OpenGLVertexBuffer(float* vertices, uint32_t size);
        virtual ~OpenGLVertexBuffer();

        virtual void bind()   const;
        virtual void unbind() const;
    private:
        uint32_t m_renderer_id = 0;
    };

    class OpenGLIndexBuffer : public IndexBuffer {
    public:
        OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
        virtual ~OpenGLIndexBuffer();

        virtual void     bind    () const;
        virtual void     unbind  () const;
        virtual uint32_t getCount() const { return m_count; }
    private:
        uint32_t m_renderer_id = 0;
        uint32_t m_count = 0;
    };
}