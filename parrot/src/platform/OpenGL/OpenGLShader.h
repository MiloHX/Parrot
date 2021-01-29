#pragma once

#include "parrot/renderer/Shader.h"

#include <string>
#include <glm/glm.hpp>


// TOBE REMOVE
typedef unsigned int GLenum;

namespace parrot {

    class OpenGLShader : public Shader {
    public:
        OpenGLShader(const std::string& file_path);
        OpenGLShader(const std::string& name, const std::string& vertex_source, const std::string& fragment_source);
        virtual ~OpenGLShader();

        virtual void bind  () const override;
        virtual void unbind() const override;

        virtual void setInt   (const std::string& name, const int        value) override;
        virtual void setFloat3(const std::string& name, const glm::vec3& value) override;
        virtual void setFloat4(const std::string& name, const glm::vec4& value) override;
        virtual void setMat4  (const std::string& name, const glm::mat4& value) override;

        virtual const std::string& getName() const override { return m_name; }

        void uploadUniformInt   (const std::string& name,       int        value );
        void uploadUniformFloat (const std::string& name,       float      value );
        void uploadUniformFloat2(const std::string& name, const glm::vec2& values);
        void uploadUniformFloat3(const std::string& name, const glm::vec3& values);
        void uploadUniformFloat4(const std::string& name, const glm::vec4& values);
        void uploadUniformMat3  (const std::string& name, const glm::mat3& matrix);
        void uploadUniformMat4  (const std::string& name, const glm::mat4& matrix);
    private:
        std::string                             readFile(const std::string& file_path);
        std::unordered_map<GLenum, std::string> preprocess(const std::string& shader_source);
        void                                    compile(const std::unordered_map<GLenum, std::string>& processed_shader_source);

        uint32_t    m_renderer_id = 0;
        std::string m_name;
    };

}