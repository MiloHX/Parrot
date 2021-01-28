#include "prpch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace parrot {

    OpenGLShader::OpenGLShader(const std::string& vertex_source, const std::string& fragment_source) {
        // Vertex Shader
        GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);        // Create an empty vertex shader
        const GLchar* source = vertex_source.c_str();                   // Convert vertex shader source to GLchar*
        glShaderSource(vertex_shader, 1, &source, 0);                   // Send the converted result to GL
        glCompileShader(vertex_shader);                                 // Compile the vertex shader

        GLint is_compiled = 0;
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &is_compiled);  // Check compile result
        if (is_compiled == GL_FALSE) {                                  // Error handling
            GLint max_length = 0;
            glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &max_length);
            std::vector<GLchar> info_log_list(max_length);
            glGetShaderInfoLog(vertex_shader, max_length, &max_length, &info_log_list[0]);
            glDeleteShader(vertex_shader);

            PR_CORE_ERROR("{0}", info_log_list.data());
            PR_CORE_ASSERT(false, "Vertex shader compilation error");

            return;
        }

        // Fragment Shader
        GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);    // Create an empty vertex shader
        source = fragment_source.c_str();                               // Convert fragement shader source to GLchar*
        glShaderSource(fragment_shader, 1, &source, 0);                 // Send the converted result to GL
        glCompileShader(fragment_shader);                               // Compile the fragment shader

        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &is_compiled);// Check compile result
        if (is_compiled == GL_FALSE) {                                  // Error handling
            GLint max_length = 0;
            glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &max_length);
            std::vector<GLchar> info_log_list(max_length);
            glGetShaderInfoLog(fragment_shader, max_length, &max_length, &info_log_list[0]);

            glDeleteShader(vertex_shader  );
            glDeleteShader(fragment_shader);

            PR_CORE_ERROR("{0}", info_log_list.data());
            PR_CORE_ASSERT(false, "Fragment shader compilation error");

            return;
        }

        // Program
        m_renderer_id = glCreateProgram();
        GLuint program = m_renderer_id;
        glAttachShader(program, vertex_shader  );   // Attach vertex shader
        glAttachShader(program, fragment_shader);   // Attach fragment shader
        glLinkProgram(program);                     // Link program

        GLint is_linked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&is_linked);
        if (is_linked == GL_FALSE) {
            GLint max_length = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);
            std::vector<GLchar> info_log_list(max_length);
            glGetProgramInfoLog(program, max_length, &max_length, &info_log_list[0]);

            glDeleteShader (vertex_shader  );
            glDeleteShader (fragment_shader);
            glDeleteProgram(program        );

            PR_CORE_ERROR("{0}", info_log_list.data());
            PR_CORE_ASSERT(false, "Shader program link error");

            return;
        }

        glDetachShader(program, vertex_shader  );
        glDetachShader(program, fragment_shader);

    }

    OpenGLShader::~OpenGLShader() {
        glDeleteProgram(m_renderer_id);
    }

    void OpenGLShader::bind() const {
        glUseProgram(m_renderer_id);
    }

    void OpenGLShader::unbind() const {
        glUseProgram(0);
    }

    void OpenGLShader::uploadUniformInt(const std::string& name, int value) {
        GLint location =  glGetUniformLocation(m_renderer_id, name.c_str());
        glUniform1i(location, value);
    }

    void OpenGLShader::uploadUniformFloat(const std::string& name, float value) {
        GLint location =  glGetUniformLocation(m_renderer_id, name.c_str());
        glUniform1f(location, value);
    }

    void OpenGLShader::uploadUniformFloat2(const std::string& name, const glm::vec2& values) {
        GLint location =  glGetUniformLocation(m_renderer_id, name.c_str());
        glUniform2f(location, values.x, values.y);
    }

    void OpenGLShader::uploadUniformFloat3(const std::string& name, const glm::vec3& values) {
        GLint location =  glGetUniformLocation(m_renderer_id, name.c_str());
        glUniform3f(location, values.x, values.y, values.z);
    }

    void OpenGLShader::uploadUniformFloat4(const std::string& name, const glm::vec4& values) {
        GLint location =  glGetUniformLocation(m_renderer_id, name.c_str());
        glUniform4f(location, values.x, values.y, values.z, values.w);
    }

    void OpenGLShader::uploadUniformMat3(const std::string& name, const glm::mat3& matrix) {
        GLint location =  glGetUniformLocation(m_renderer_id, name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void OpenGLShader::uploadUniformMat4(const std::string& name, const glm::mat4& matrix) {
        GLint location =  glGetUniformLocation(m_renderer_id, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));

    }


}