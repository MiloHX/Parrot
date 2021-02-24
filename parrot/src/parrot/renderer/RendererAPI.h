#pragma once

#include "parrot/renderer/VertexArray.h"

#include <memory>
#include <glm/glm.hpp>

namespace parrot {

    class RendererAPI {
    public:
        virtual ~RendererAPI() = default;

        enum class API {
            None   = 0,
            OpenGL = 1
        };

        virtual void      init() = 0;
        virtual void      setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
        virtual void      setClearColor(const glm::vec4& color) = 0;
        virtual void      clear()                               = 0;
        virtual void      drawIndexed(const Ref<VertexArray>& vertex_array, uint32_t count = 0) = 0;
        inline static API getAPI() { return s_API; }
    private:
        static API s_API;

    };
}