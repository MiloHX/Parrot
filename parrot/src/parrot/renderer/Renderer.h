#pragma once

#include "parrot/renderer/RenderCommand.h"
#include "parrot/renderer/OrthographicCamera.h"
#include "parrot/renderer/Shader.h"

namespace parrot {
    
    class Renderer {
    public:
        static void init      ();
        static void onWindowResized(uint32_t width, uint32_t height);
        static void beginScene(OrthographicCamera& camera);
        static void endScene  ();
        static void submit    (const Ref<Shader>& shader, const Ref<VertexArray>& vertex_array, const glm::mat4& transform = glm::mat4(1.0f));

        inline static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }

    private:
        struct SceneData {
            glm::mat4 view_projection_matrix;
        };

        static SceneData* m_scene_data;
    };
}