#pragma once

namespace parrot {
    
    enum class RendererAPI {
        None   = 0,
        OpenGL = 1
    };

    class Renderer {
    public:
        inline static RendererAPI getAPI() { return s_renderer_API; }
    private:
        static RendererAPI s_renderer_API;
    };
}