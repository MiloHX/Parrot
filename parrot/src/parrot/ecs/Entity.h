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
        T& add(Args&&... args) {
            PR_CORE_ASSERT(!has<T>(), "Entity already has component!")
            return m_scene->m_registry.emplace<T>(m_entity_handle, std::forward<Args>(args)...);
        }

        template<typename T>
        T& get() {
            PR_CORE_ASSERT(has<T>(), "Entity doesn't have component when getting it!")
            return m_scene->m_registry.get<T>(m_entity_handle);
        }

        template<typename T>
        void remove() {
            PR_CORE_ASSERT(has<T>(), "Entity doesn't have component when removing it!")
            m_scene->m_registry.remove<T>(m_entity_handle);
        }

        template<typename T>
        bool has() {
            return m_scene->m_registry.has<T>(m_entity_handle);
        }

        bool valid() {
            return m_scene ? m_scene->m_registry.valid(m_entity_handle) : false;
        }

        operator bool        () const { return m_entity_handle != entt::null; }
        operator uint32_t    () const { return static_cast<uint32_t>(m_entity_handle); }
        operator entt::entity() const { return m_entity_handle; }
        bool operator ==(const Entity& other)  const { return (m_entity_handle == other.m_entity_handle) && (m_scene == other.m_scene); }
        bool operator !=(const Entity& other)  const { return (m_entity_handle != other.m_entity_handle) || (m_scene != other.m_scene); }
    private:
        entt::entity m_entity_handle{ entt::null };
        Scene*       m_scene = nullptr;
        friend class Scene;
    };
}