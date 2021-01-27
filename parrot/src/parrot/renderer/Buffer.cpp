#include "prpch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "platform/OpenGL/OpenGLBuffer.h"

namespace parrot {

    VertexBuffer* VertexBuffer::create(float* vertices, uint32_t size) {
        switch (Renderer::getAPI()) {
            case RendererAPI::None:
                PR_INT_ASSERT(false, "RendererAPI::None is not supported");
                return nullptr;
            case RendererAPI::OpenGL:
                return new OpenGLVertexBuffer(vertices, size);
        }

        PR_INT_ASSERT(false, "Unknown RendererAPI when creating vertex buffer");
        return nullptr;
    }

    IndexBuffer* IndexBuffer::create(uint32_t* indices, uint32_t count) {
        switch (Renderer::getAPI()) {
            case RendererAPI::None:
                PR_INT_ASSERT(false, "RendererAPI::None is not supported");
                return nullptr;
            case RendererAPI::OpenGL:
                return new OpenGLIndexBuffer(indices, count);
        }

        PR_INT_ASSERT(false, "Unknown RendererAPI when creating index buffer");
        return nullptr;
    }

}
