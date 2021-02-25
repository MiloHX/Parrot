#pragma once

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


    struct SpriteRendererComponent {
        glm::vec4 color = glm::vec4(1.0f);

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& color) : color(color) {}

    };

}