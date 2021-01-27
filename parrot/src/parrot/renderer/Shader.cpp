#include "prpch.h"
#include "Shader.h"

#include <glad/glad.h>

namespace parrot {

    Shader::Shader(const std::string& vertex_source, const std::string& fragment_source) {
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

            PR_INT_ERROR("{0}", info_log_list.data());
            PR_INT_ASSERT(false, "Vertex shader compilation error");

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

            PR_INT_ERROR("{0}", info_log_list.data());
            PR_INT_ASSERT(false, "Fragment shader compilation error");

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

            PR_INT_ERROR("{0}", info_log_list.data());
            PR_INT_ASSERT(false, "Shader program link error");

            return;
        }

        glDetachShader(program, vertex_shader  );
        glDetachShader(program, fragment_shader);

    }

    Shader::~Shader() {
        glDeleteProgram(m_renderer_id);
    }

    void Shader::bind() const {
        glUseProgram(m_renderer_id);
    }

    void Shader::unbind() const {
        glUseProgram(0);
    }
    
}