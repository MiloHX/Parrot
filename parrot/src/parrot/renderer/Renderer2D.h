#pragma once

#include "parrot/renderer/OrthographicCamera.h"
#include "parrot/renderer/Texture.h"

namespace parrot {

    class Renderer2D {
    public:
        static void init();
        static void shutdown();

        static void beginScene(const OrthographicCamera camera);
        static void endScene();
        static void flush();

        static void       resetStatics();

        struct Statistics {
            uint32_t draw_calls = 0;
            uint32_t quad_count = 0;

            uint32_t getTotalVertexCount() { return quad_count * 4; }
            uint32_t getTotalIndexCount () { return quad_count * 6; }
        };

        static Statistics getStatics  ();

        static void drawQuad(
            const glm::vec3&      position, 
            const glm::vec2&      size,
            float                 rotation,
            const Ref<Texture2D>& texture, 
            const glm::vec4&      color = glm::vec4(1.0),
            const glm::vec2&      texture_scale = glm::vec2(1.0)
        );
    private:
        static void startBatch();
        static void endBatch();
    };
}