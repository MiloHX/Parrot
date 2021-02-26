#pragma once

#include "parrot/renderer/RenderCamera.h"
#include "parrot/ecs/ScriptableEntity.h"

#include <glm/glm.hpp>
#include <glm/gtx/matrix_major_storage.hpp>

namespace parrot {

    struct TransformComponent {
        glm::mat4 transform = glm::mat4();

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::mat4& other) : transform(other) {}

        operator       glm::mat4&()       { return transform; }
        operator const glm::mat4&() const { return transform; }
    };

    struct TagComponent {
        std::string tag;
        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& tag) : tag(tag) {}
    };


    struct SpriteRendererComponent {
        glm::vec4 color = glm::vec4(1.0f);

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& color) : color(color) {}
    };

    struct CameraComponent {
        RenderCamera camera;
        bool         fixed_aspect_ratio = false;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
    };

    struct ScriptComponent {
        ScriptableEntity* instance = nullptr;

        void(*instantiateFunction)(ScriptableEntity*&)           = nullptr;
        void(*destroyFunction    )(ScriptableEntity*&)           = nullptr;
        void(*onCreateFunction   )(ScriptableEntity* )           = nullptr;
        void(*onDestroyFunction  )(ScriptableEntity* )           = nullptr;
        void(*onUpdateFunction   )(ScriptableEntity* , TimeStep) = nullptr;

        template<typename T>
        void bind() {
            // TODO: allow missing functions
            instantiateFunction = [](ScriptableEntity*& instance) { instance = new T();  };
            destroyFunction     = [](ScriptableEntity*& instance) { delete static_cast<T*>(instance); instance = nullptr; };
            onCreateFunction    = [](ScriptableEntity*  instance) { static_cast<T*>(instance)->onCreate(); };
            onDestroyFunction   = [](ScriptableEntity*  instance) { static_cast<T*>(instance)->onDestroy(); };
            onUpdateFunction    = [](ScriptableEntity*  instance, TimeStep time_step) { static_cast<T*>(instance)->onUpdate(time_step); };
        }
    };

}