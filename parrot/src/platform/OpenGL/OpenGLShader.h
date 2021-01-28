#pragma once

#include "parrot/renderer/Shader.h"

#include <string>
#include <glm/glm.hpp>

namespace parrot {

    class OpenGLShader : public Shader {
    public:
        OpenGLShader(const std::string& vertex_source, const std::string& fragment_source);
        virtual ~OpenGLShader();

        virtual void bind  () const override;
        virtual void unbind() const override;


        void uploadUniformInt   (const std::string& name,       int        value );
        void uploadUniformFloat (const std::string& name,       float      value );
        void uploadUniformFloat2(const std::string& name, const glm::vec2& values);
        void uploadUniformFloat3(const std::string& name, const glm::vec3& values);
        void uploadUniformFloat4(const std::string& name, const glm::vec4& values);
        void uploadUniformMat3  (const std::string& name, const glm::mat3& matrix);
        void uploadUniformMat4  (const std::string& name, const glm::mat4& matrix);
    private:
        uint32_t m_renderer_id = 0;
    };

}