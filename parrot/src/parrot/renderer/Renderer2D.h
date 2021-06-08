#pragma once

#include "parrot/renderer/Camera.h"
#include "parrot/renderer/EditorCamera.h"
#include "parrot/renderer/OrthographicCamera.h"
#include "parrot/renderer/Texture.h"
#include "parrot/renderer/SubTexture2D.h"

namespace parrot {

    class Renderer2D {
    public:
        static void init();
        static void shutdown();

        static void beginScene(const Camera& camera, const glm::mat4& transform);
        static void beginScene(const EditorCamera& camera);
        static void beginScene(const OrthographicCamera& camera);
        static void endScene();
        static void flush();

        static void resetStatics();

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
            const glm::vec4&      color         = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
            const glm::vec2&      texture_scale = glm::vec2(1.0)
        );

        static void drawQuad(
            const glm::mat4&      transform,
            const Ref<Texture2D>& texture, 
            const glm::vec4&      color         = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
            const glm::vec2&      texture_scale = glm::vec2(1.0)
        );

        static void drawSubTexture(
            const glm::vec3&         position, 
            const glm::vec2&         size,
            float                    rotation,
            const Ref<SubTexture2D>& sub_texture, 
            const glm::vec4&         color         = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
            const glm::vec2&         texture_scale = glm::vec2(1.0)
        );

        static void drawSubTexture(
            const glm::mat4&         transform,
            const Ref<SubTexture2D>& sub_texture, 
            const glm::vec4&         color         = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
            const glm::vec2&         texture_scale = glm::vec2(1.0)
        );

    private:
        static void startBatch();
        static void endBatch();
    };
}