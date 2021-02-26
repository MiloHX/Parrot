#pragma once

#include "parrot/ecs/Entity.h"

namespace parrot {

    class ScriptableEntity {
    public:
        template<typename T>
        T& get() {
            return m_entity.get<T>();
        }
    private:
        Entity       m_entity;
        friend class Scene;

    };

}