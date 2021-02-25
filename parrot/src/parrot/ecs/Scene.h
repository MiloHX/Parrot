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
    private:
        entt::registry m_registry;

        friend class Entity;
    };

}