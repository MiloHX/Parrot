#pragma once

#include "parrot/ecs/Scene.h"

#include <entt.hpp>

namespace parrot {
    class Entity {
    public:
        Entity() = default;
        Entity(entt::entity handle, Scene* scene) : m_entity_handle(handle), m_scene(scene) {}
        Entity(const Entity& other) = default;

        template<typename T, typename... Args>
        T& addComponent(Args&&... args) {
            PR_CORE_ASSERT(!hasComponent<T>(), "Entity already has component!")
            return m_scene->m_registry.emplace<T>(m_entity_handle, std::forward<Args>(args)...);
        }

        template<typename T>
        T& get() {
            PR_CORE_ASSERT(hasComponent<T>(), "Entity doesn't have component when getting it!")
            return m_scene->m_registry.get<T>(m_entity_handle);
        }

        template<typename T>
        void removeComponent() {
            PR_CORE_ASSERT(hasComponent<T>(), "Entity doesn't have component when removing it!")
            m_scene->m_registry.remove<T>(m_entity_handle);
        }

        template<typename T>
        bool hasComponent() {
            return m_scene->m_registry.has<T>(m_entity_handle);
        }

        bool valid() {
            return m_scene->m_registry.valid(m_entity_handle);
        }

    private:
        entt::entity m_entity_handle{ entt::null };
        Scene*       m_scene;
        friend       Scene;
    };
}