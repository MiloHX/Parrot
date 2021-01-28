#pragma once

#include <string>
#include <glm/glm.hpp>

namespace parrot {

    class Shader {
    public:
        Shader(const std::string& vertex_source, const std::string& fragment_source);
        ~Shader();

        void bind() const;
        void unbind() const;

        void uploadUniformMat4(const std::string& name, const glm::mat4& matrix);
    private:
        uint32_t m_renderer_id = 0;
    };

}
