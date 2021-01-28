#pragma once

#include "parrot/renderer/RendererAPI.h"

namespace parrot {

    class OpenGLRendererAPI : public RendererAPI {
    public:
        virtual void setClearColor(const glm::vec4& color) override;
        virtual void clear() override;
        virtual void drawIndexed(const Ref<VertexArray>& vertex_array) override;
    };

}