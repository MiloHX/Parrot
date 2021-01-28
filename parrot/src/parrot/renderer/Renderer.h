#pragma once

#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace parrot {
    
    class Renderer {
    public:
        static void beginScene(OrthographicCamera& camera);
        static void endScene  ();
        static void submit    (const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertex_array, const glm::mat4& transform = glm::mat4(1.0f));

        inline static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }

    private:
        struct SceneData {
            glm::mat4 view_projection_matrix;
        };

        static SceneData* m_scene_data;
    };
}