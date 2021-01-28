#include "prpch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

namespace parrot {

    static GLenum shaderTypeFromString(const std::string& type) {
        if (type == "vertex") {
            return GL_VERTEX_SHADER;
        } else if (type == "fragment" || type == "pixel") {
            return GL_FRAGMENT_SHADER;
        }

        PR_CORE_ASSERT(false, "Unsupported Shader Type: {0}", type);
        return 0;
    }

    OpenGLShader::OpenGLShader(const std::string& file_path) {
        std::string shader_source = readFile(file_path);
        auto processed_shader_source = preprocess(shader_source);
        compile(processed_shader_source);
    }

    OpenGLShader::OpenGLShader(const std::string& vertex_source, const std::string& fragment_source) {
        std::unordered_map<GLenum, std::string> processed_shader_source;
        processed_shader_source[GL_VERTEX_SHADER  ] = vertex_source;
        processed_shader_source[GL_FRAGMENT_SHADER] = fragment_source;
        compile(processed_shader_source);
    }

    OpenGLShader::~OpenGLShader() {
        glDeleteProgram(m_renderer_id);
    }

    std::string OpenGLShader::readFile(const std::string& file_path) {
        std::string result;
        std::ifstream in(file_path, std::ios::in, std::ios::binary);
        if (in) {
            in.seekg(0, std::ios::end);
            result.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&result[0], result.size());
            in.close();
        } else {
            PR_CORE_ERROR("Could not open file at {0}", file_path);
        }

        return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::preprocess(const std::string& shader_source) {
        std::unordered_map<GLenum, std::string> processed_shader_source;

        const char* type_token = "#type";
        size_t type_token_length = strlen(type_token);
        size_t pos = shader_source.find(type_token, 0);
        while (pos != std::string::npos) {
            size_t eol = shader_source.find_first_of("\r\n", pos);
            PR_CORE_ASSERT(eol != std::string::npos, "Syntax error");
            size_t begin = pos + type_token_length + 1;
            std::string type = shader_source.substr(begin, eol - begin);
            size_t next_line_pos = shader_source.find_first_not_of("\r\n", eol);
            pos = shader_source.find(type_token, next_line_pos);
            processed_shader_source[shaderTypeFromString(type)] =
                shader_source.substr(next_line_pos, pos - (next_line_pos == std::string::npos ? shader_source.size() - 1 : next_line_pos));
        }

        return processed_shader_source;
    }


    void OpenGLShader::compile(const std::unordered_map<GLenum, std::string>& processed_shader_source) {

        GLuint program = glCreateProgram();
        std::vector<GLenum> processed_shader_id_list(processed_shader_source.size());
        for (auto& source_item : processed_shader_source) {
            GLenum shader_type = source_item.first;
            const std::string& source = source_item.second;
            GLuint shader = glCreateShader(shader_type);
            const GLchar* source_string = source.c_str();
            glShaderSource(shader, 1, &source_string, 0);
            glCompileShader(shader);
            GLint is_compiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);  // Check compile result
            if (is_compiled == GL_FALSE) {                                  // Error handling
                GLint max_length = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);
                std::vector<GLchar> info_log_list(max_length);
                glGetShaderInfoLog(shader, max_length, &max_length, &info_log_list[0]);
                glDeleteShader(shader);

                PR_CORE_ERROR("{0}", info_log_list.data());
                PR_CORE_ASSERT(false, "shader compilation error");
                break;
            }
            glAttachShader(program, shader);   // Attach vertex shader
            processed_shader_id_list.push_back(shader);
        }
        // Program

        glLinkProgram(program);                     // Link program

        GLint is_linked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&is_linked);
        if (is_linked == GL_FALSE) {
            GLint max_length = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);
            std::vector<GLchar> info_log_list(max_length);
            glGetProgramInfoLog(program, max_length, &max_length, &info_log_list[0]);

            for (auto shader_id : processed_shader_id_list) {
                glDeleteShader(shader_id);
            }
            glDeleteProgram(program);

            PR_CORE_ERROR("{0}", info_log_list.data());
            PR_CORE_ASSERT(false, "Shader program link error");

            return;
        }

        for (auto shader_id : processed_shader_id_list) {
            glDetachShader(program, shader_id);
        }

        m_renderer_id = program;
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