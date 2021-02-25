#include "prpch.h"
#include "parrot/ecs/Scene.h"
#include "parrot/ecs/Component.h"
#include "parrot/renderer/Renderer2D.h"

namespace parrot {
    Scene::Scene() {
    }

    Scene::~Scene() {
    }

    entt::entity Scene::createEntity() {
        return m_registry.create();
    }

    void Scene::onUpdate(TimeStep time_step) {
        auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

        for (auto entity : group) {
            auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
            Renderer2D::drawQuad(transform, nullptr, sprite.color);
        }

    }
}