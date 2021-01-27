#pragma once

#include "RendererAPI.h"

namespace parrot {

    class RenderCommand {
    public:
        inline static void setClearColor(const glm::vec4& color) {
            s_renderer_API->setClearColor(color);
        }
        inline static void clear() {
            s_renderer_API->clear();
        }
        inline static void drawIndexed(const std::shared_ptr<VertexArray>& vertex_array) {
            s_renderer_API->drawIndexed(vertex_array);
        }
    private:
        static RendererAPI* s_renderer_API;
    };

}