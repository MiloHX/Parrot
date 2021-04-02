#pragma once

#include "parrot/ecs/Scene.h"

namespace parrot {

    class SceneSerializer {
    public:
        SceneSerializer(const Ref<Scene>& scene);

        void serialize(const std::string& file_path);
        void serializeBinary(const std::string& file_path);
        bool deserialize(const std::string& file_path);
        bool deserializeBinary(const std::string& file_path);
    private:
        Ref<Scene> m_scene;
    };

}