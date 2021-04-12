#pragma once

#include "parrot/renderer/RenderCamera.h"
#include "parrot/ecs/ScriptableEntity.h"

#include <glm/glm.hpp>
#include <glm/gtx/matrix_major_storage.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace parrot {

    struct TransformComponent {
        glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 rotation    = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 scale       = glm::vec3(1.0f, 1.0f, 1.0f);

        //glm::mat4 transform = glm::mat4();

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& translation) : translation(translation) {}

        glm::mat4 getTransform() const {
            glm::mat4 rotation_mat = glm::toMat4(glm::quat(rotation));
            //glm::mat4 rotation_mat = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3( 1, 0, 0 )) *
            //                         glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3( 0, 1, 0 )) *
            //                         glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3( 0, 0, 1 ));

            return glm::translate(glm::mat4(1.0), translation) * rotation_mat * glm::scale(glm::mat4(1.0f), scale);
        }
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