#pragma once
#include "parrot/core/TimeStep.h"

#include <entt.hpp>

namespace parrot {
    
    class Entity;

    class Scene {
    public:
        Scene();
        ~Scene();

        Entity Scene::createEntity(const std::string& name = std::string());

        void onUpdate(TimeStep time_step);
        void onViewportResize(uint32_t width, uint32_t height);
        void setActiveCamera(Entity camera_entity);
    private:
        entt::registry m_registry;

        entt::entity   m_active_camera_entity = { entt::null };

        uint32_t m_viewprot_width  = 0;
        uint32_t m_viewport_height = 0;

        friend class Entity;
    };

}