#pragma once

#include "VertexArray.h"

#include <memory>
#include <glm/glm.hpp>

namespace parrot {

    class RendererAPI {
    public:
        enum class API {
            None   = 0,
            OpenGL = 1
        };

        virtual void      setClearColor(const glm::vec4& color) = 0;
        virtual void      clear()                               = 0;
        virtual void      drawIndexed(const Ref<VertexArray>& vertex_array) = 0;
        inline static API getAPI() { return s_API; }
    private:
        static API s_API;

    };
}