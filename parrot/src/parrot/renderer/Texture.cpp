#include "prpch.h"
#include "parrot/renderer/Texture.h"

#include "parrot/renderer/Renderer.h"
#include "platform/OpenGL/OpenGLTexture.h"

namespace parrot {
    Ref<Texture2D> Texture2D::create(uint32_t width, uint32_t height) {
        switch (Renderer::getAPI()) {
            case RendererAPI::API::None:
                PR_CORE_ASSERT(false, "RendererAPI::None is not supported");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return createRef<OpenGLTexture2D>(width, height);
        }

        PR_CORE_ASSERT(false, "Unknown RendererAPI when creating vertex buffer");
        return nullptr;
    }

    Ref<Texture2D> Texture2D::create(const std::string& path) {
        switch (Renderer::getAPI()) {
            case RendererAPI::API::None:
                PR_CORE_ASSERT(false, "RendererAPI::None is not supported");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return createRef<OpenGLTexture2D>(path);
        }

        PR_CORE_ASSERT(false, "Unknown RendererAPI when creating vertex buffer");
        return nullptr;
    }

}