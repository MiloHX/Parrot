#include "prpch.h"
#include "Shader.h"
#include "Renderer.h"
#include "platform/OpenGL/OpenGLShader.h"

namespace parrot {

    Shader* Shader::create(const std::string& vertex_source, const std::string& fragment_source) {
        switch (Renderer::getAPI()) {
            case RendererAPI::API::None:
                PR_CORE_ASSERT(false, "RendererAPI::None is not supported");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return new OpenGLShader(vertex_source, fragment_source);
        }

        PR_CORE_ASSERT(false, "Unknown RendererAPI when creating vertex buffer");
        return nullptr;
    }
}