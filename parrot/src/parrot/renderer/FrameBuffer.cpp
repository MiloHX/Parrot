#include "prpch.h"
#include "FrameBuffer.h"
#include "parrot/renderer/Renderer.h"
#include "platform/OpenGL/OpenGLFrameBuffer.h"

namespace parrot {
    Ref<FrameBuffer> FrameBuffer::create(const FrameBufferProps& frame_buffer_props) {
        switch (Renderer::getAPI()) {
            case RendererAPI::API::None:
                PR_CORE_ASSERT(false, "RendererAPI::None is not supported");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return createRef<OpenGLFrameBuffer>(frame_buffer_props);
        }

        PR_CORE_ASSERT(false, "Unknown RendererAPI when creating frame buffer");
        return nullptr;
    }
}