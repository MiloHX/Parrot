#include "prpch.h"
#include "parrot/renderer/VertexArray.h"
#include "parrot/renderer/Renderer.h"
#include "platform/OpenGL/OpenGLVertexArray.h"

namespace parrot {

    Ref<VertexArray> VertexArray::create() {
        switch (Renderer::getAPI()) {
            case RendererAPI::API::None:
                PR_CORE_ASSERT(false, "RendererAPI::None is not supported");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLVertexArray>();
        }

        PR_CORE_ASSERT(false, "Unknown RendererAPI when creating vertex buffer");
        return nullptr;
    }


}