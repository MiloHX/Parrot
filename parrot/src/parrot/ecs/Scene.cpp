#include "prpch.h"
#include "parrot/ecs/Scene.h"
#include "parrot/ecs/Component.h"
#include "parrot/ecs/Entity.h"
#include "parrot/renderer/Renderer2D.h"

namespace parrot {
    Scene::Scene() {
    }

    Scene::~Scene() {
    }

    Entity Scene::createEntity(const std::string& name) {
        Entity entity = Entity(m_registry.create(), this);
        entity.addComponent<TransformComponent>();
        auto& tag_component = entity.addComponent<TagComponent>(name);
        tag_component.tag = name.empty() ? "Entity" : name;
        return entity;
    }

    void Scene::onUpdate(TimeStep time_step) {
        auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

        for (auto entity : group) {
            auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
            Renderer2D::drawQuad(transform, nullptr, sprite.color);
        }

    }
}