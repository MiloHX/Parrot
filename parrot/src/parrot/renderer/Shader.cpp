#include "prpch.h"
#include "parrot/renderer/Shader.h"
#include "parrot/renderer/Renderer.h"
#include "platform/OpenGL/OpenGLShader.h"

namespace parrot {
    Ref<Shader> Shader::create(const std::string& file_path) {
        switch (Renderer::getAPI()) {
            case RendererAPI::API::None:
                PR_CORE_ASSERT(false, "RendererAPI::None is not supported");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLShader>(file_path);
        }

        PR_CORE_ASSERT(false, "Unknown RendererAPI when creating shader");
        return nullptr;
    }

    Ref<Shader> Shader::create(const std::string& name, const std::string& vertex_source, const std::string& fragment_source) {
        switch (Renderer::getAPI()) {
            case RendererAPI::API::None:
                PR_CORE_ASSERT(false, "RendererAPI::None is not supported");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLShader>(name, vertex_source, fragment_source);
        }

        PR_CORE_ASSERT(false, "Unknown RendererAPI when creating shader");
        return nullptr;
    }

    void ShaderLibrary::add(const Ref<Shader>& shader) {
        auto& name = shader->getName();
        add(name, shader);
    }

    void ShaderLibrary::add(const std::string& name, const Ref<Shader>& shader) {
        PR_CORE_ASSERT(!exists(name), "Shader already exists")
        m_shader_map[name] = shader;
    }

    Ref<Shader> ShaderLibrary::load(const std::string& file_path) {
        auto shader = Shader::create(file_path);
        add(shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::load(const std::string& name, const std::string& file_path) {
        auto shader = Shader::create(file_path);
        add(name, shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::get(const std::string& name) {
        PR_CORE_ASSERT(exists(name), "Shader not found!")
        return m_shader_map[name];
    }
    bool ShaderLibrary::exists(const std::string& name) const {
        return m_shader_map.find(name) != m_shader_map.end();
    }
}