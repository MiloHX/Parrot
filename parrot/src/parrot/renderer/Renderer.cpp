#include "prpch.h"
#include "Renderer.h"

#include "platform/OpenGL/OpenGLShader.h"

namespace parrot {

    Renderer::SceneData* Renderer::m_scene_data = new Renderer::SceneData();

    void Renderer::beginScene(OrthographicCamera& camera) {
        m_scene_data->view_projection_matrix = camera.getViewPorjectionMatrix();
    }

    void Renderer::endScene() {

    }

    void Renderer::submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertex_array, const glm::mat4& transform) {
        shader->bind();
        std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("u_view_projection", m_scene_data->view_projection_matrix);
        std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("u_transform", transform);

        vertex_array->bind();
        RenderCommand::drawIndexed(vertex_array);
    }

}