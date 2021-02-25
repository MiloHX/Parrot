#pragma once

#include "parrot/core/TimeStep.h"

#include <entt.hpp>

namespace parrot {
    
    class Scene {
    public:
        Scene();
        ~Scene();

        entt::entity    createEntity();
        entt::registry& getRegistry() { return m_registry; }

        void onUpdate(TimeStep time_step);
    private:
        entt::registry m_registry;
    };

}