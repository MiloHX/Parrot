#pragma once

#include "RenderCommand.h"

namespace parrot {
    
    class Renderer {
    public:
        static void beginScene();
        static void endScene  ();
        static void submit    (const std::shared_ptr<VertexArray>& vertex_array);

        inline static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }
    };
}