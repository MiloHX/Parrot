#include "prpch.h"
#include "Renderer.h"

namespace parrot {

    Renderer::SceneData* Renderer::m_scene_data = new Renderer::SceneData();

    void Renderer::beginScene(OrthographicCamera& camera) {
        m_scene_data->view_projection_matrix = camera.getViewPorjectionMatrix();
    }

    void Renderer::endScene() {

    }

    void Renderer::submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertex_array) {
        shader->bind();
        shader->uploadUniformMat4("u_view_projection", m_scene_data->view_projection_matrix);
        vertex_array->bind();
        RenderCommand::drawIndexed(vertex_array);
    }

}