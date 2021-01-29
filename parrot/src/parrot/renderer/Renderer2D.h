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

        static void drawQuad(
            const glm::vec3&      position, 
            const glm::vec2&      size, 
            const Ref<Texture2D>& texture, 
            const glm::vec4&      color = glm::vec4(1.0),
            const glm::vec2&      texture_scale = glm::vec2(1.0)
        );
    };
}