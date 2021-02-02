#include "prpch.h"
#include "parrot/renderer/Buffer.h"
#include "parrot/renderer/Renderer.h"
#include "platform/OpenGL/OpenGLBuffer.h"

namespace parrot {

    Ref<VertexBuffer> VertexBuffer::create(uint32_t size) {
        switch (Renderer::getAPI()) {
            case RendererAPI::API::None:
                PR_CORE_ASSERT(false, "RendererAPI::None is not supported");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return createRef<OpenGLVertexBuffer>(size);
        }

        PR_CORE_ASSERT(false, "Unknown RendererAPI when creating vertex buffer with size");
        return nullptr;
    }

    Ref<VertexBuffer> VertexBuffer::create(float* vertices, uint32_t size) {
        switch (Renderer::getAPI()) {
            case RendererAPI::API::None:
                PR_CORE_ASSERT(false, "RendererAPI::None is not supported");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return createRef<OpenGLVertexBuffer>(vertices, size);
        }

        PR_CORE_ASSERT(false, "Unknown RendererAPI when creating vertex buffer with vertices and size");
        return nullptr;
    }

    Ref<IndexBuffer> IndexBuffer::create(uint32_t* indices, uint32_t count) {
        switch (Renderer::getAPI()) {
            case RendererAPI::API::None:
                PR_CORE_ASSERT(false, "RendererAPI::None is not supported");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return createRef<OpenGLIndexBuffer>(indices, count);
        }

        PR_CORE_ASSERT(false, "Unknown RendererAPI when creating index buffer");
        return nullptr;
    }

}
