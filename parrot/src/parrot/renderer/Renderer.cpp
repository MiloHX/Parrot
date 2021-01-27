#include "prpch.h"
#include "Renderer.h"

namespace parrot {

    void Renderer::beginScene() {

    }

    void Renderer::endScene() {

    }

    void Renderer::submit(const std::shared_ptr<VertexArray>& vertex_array) {
        vertex_array->bind();
        RenderCommand::drawIndexed(vertex_array);
    }

}