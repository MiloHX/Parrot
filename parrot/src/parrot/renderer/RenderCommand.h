#pragma once

#include "RendererAPI.h"

namespace parrot {

    class RenderCommand {
    public:
        inline static void init() {
            s_renderer_API->init();
        }

        inline static void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
            s_renderer_API->setViewport(x, y, width, height);
        }

        inline static void setClearColor(const glm::vec4& color) {
            s_renderer_API->setClearColor(color);
        }

        inline static void clear() {
            s_renderer_API->clear();
        }

        inline static void drawIndexed(const Ref<VertexArray>& vertex_array) {
            s_renderer_API->drawIndexed(vertex_array);
        }

    private:
        static RendererAPI* s_renderer_API;
    };

}