#include "prpch.h"
#include "platform/OpenGL/OpenGLRendererAPI.h"

#include "glad/glad.h"

namespace parrot {
    
    void OpenGLRendererAPI::init() {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
    }

    void OpenGLRendererAPI::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
        glViewport(x, y, width, height);
    }

    void OpenGLRendererAPI::setClearColor(const glm::vec4& color) {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::drawIndexed(const Ref<VertexArray>& vertex_array, uint32_t index_count) {
        uint32_t count = index_count ? index_count : vertex_array->getIndexBuffer()->getCount();
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    }

}
