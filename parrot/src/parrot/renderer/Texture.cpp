#include "prpch.h"
#include "parrot/renderer/Texture.h"

#include "parrot/renderer/Renderer.h"
#include "platform/OpenGL/OpenGLTexture.h"

namespace parrot {

    Ref<Texture2D> Texture2D::create(const std::string& path) {
        switch (Renderer::getAPI()) {
            case RendererAPI::API::None:
                PR_CORE_ASSERT(false, "RendererAPI::None is not supported");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLTexture2D>(path);
        }

        PR_CORE_ASSERT(false, "Unknown RendererAPI when creating vertex buffer");
        return nullptr;
    }

}