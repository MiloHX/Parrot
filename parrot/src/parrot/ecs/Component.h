#pragma once

#include "parrot/renderer/RenderCamera.h"

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

}