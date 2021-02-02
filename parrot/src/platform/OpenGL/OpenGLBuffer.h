#pragma once

#include "parrot/renderer/Buffer.h"

namespace parrot {

    class OpenGLVertexBuffer : public VertexBuffer {
    public:
        OpenGLVertexBuffer(uint32_t size);
        OpenGLVertexBuffer(float* vertices, uint32_t size);
        virtual ~OpenGLVertexBuffer();

        virtual void bind()   const override;
        virtual void unbind() const override;
        virtual void setData(const void* data, uint32_t size) override;

        virtual void                setLayout(const BufferLayout& layout) override {
            m_layout = layout;
        }
        virtual const BufferLayout& getLayout() const override {
            return m_layout;
        }

    private:
        uint32_t     m_renderer_id = 0;
        BufferLayout m_layout;
    };

    class OpenGLIndexBuffer : public IndexBuffer {
    public:
        OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
        virtual ~OpenGLIndexBuffer();

        virtual void     bind    () const override;
        virtual void     unbind  () const override;
        virtual uint32_t getCount() const override { return m_count; }
    private:
        uint32_t m_renderer_id = 0;
        uint32_t m_count = 0;
    };
}