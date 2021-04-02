#include "prpch.h"
#include "SceneSerializer.h"
#include "parrot/ecs/Component.h"

#include <yaml-cpp/yaml.h>
#include <fstream>

namespace YAML {
    template<>
    struct convert<glm::vec3> {
        static Node encode(const glm::vec3& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            return node;
        }

        static bool decode(const Node& node, glm::vec3& rhs) {
            if (!node.IsSequence() || node.size() != 3) {
                return false;
            }

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec4> {
        static Node encode(const glm::vec4& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            return node;
        }

        static bool decode(const Node& node, glm::vec4& rhs) {
            if (!node.IsSequence() || node.size() != 4) {
                return false;
            }

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };

}

namespace parrot {

    YAML::Emitter& operator <<(YAML::Emitter& out, const glm::vec3& v) {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator <<(YAML::Emitter& out, const glm::vec4& v) {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
        return out;
    }

    SceneSerializer::SceneSerializer(const Ref<Scene>& scene) : m_scene(scene) { }

    static void serializeEntity(YAML::Emitter& out, Entity& entity) {
        out << YAML::BeginMap;
        out << YAML::Key << "Entity" << YAML::Value << "1234567890"; // TODO: Need unique entity ID

        if (entity.has<TagComponent>()) {
            out << YAML::Key << "TagComponent" << YAML::BeginMap;

            auto& tag = entity.get<TagComponent>().tag;

            out << YAML::Key << "Tag" << YAML::Value << tag;

            out << YAML::EndMap;
        }

        if (entity.has<TransformComponent>()) {
            out << YAML::Key << "TransformComponent" << YAML::BeginMap;
            
            auto& transform_comp = entity.get<TransformComponent>();

            out << YAML::Key << "Translation" << YAML::Value << transform_comp.translation;
            out << YAML::Key << "Rotation"    << YAML::Value << transform_comp.rotation   ;
            out << YAML::Key << "Scale"       << YAML::Value << transform_comp.scale      ;

            out << YAML::EndMap;
        }

        if (entity.has<CameraComponent>()) {
            out << YAML::Key << "CameraComponent" << YAML::BeginMap;

            auto& camera_comp = entity.get<CameraComponent>();
            auto& camera      = camera_comp.camera;

            out << YAML::Key << "Camera"           << YAML::Value << YAML::BeginMap;
            out << YAML::Key << "ProjectionType"   << YAML::Value << (int)camera.getProjectionType      ();
            out << YAML::Key << "ProjectionFOV"    << YAML::Value <<      camera.getPerspectiveFOV      ();
            out << YAML::Key << "PespectiveNear"   << YAML::Value <<      camera.getPerspectiveNearClip ();
            out << YAML::Key << "PespectiveFar"    << YAML::Value <<      camera.getPerspectiveFarClip  ();
            out << YAML::Key << "OrthographicSize" << YAML::Value <<      camera.getOrthographicSize    ();
            out << YAML::Key << "OrthographicNear" << YAML::Value <<      camera.getOrthographicNearClip();
            out << YAML::Key << "OrthographicFar"  << YAML::Value <<      camera.getOrthographicFarClip ();
            out << YAML::EndMap;

            out << YAML::Key << "FixedAspectRatio" << YAML::Value << camera_comp.fixed_aspect_ratio;
            out << YAML::EndMap;
        }

        if (entity.has<SpriteRendererComponent>()) {
            out << YAML::Key << "SpriteRendererComponent" << YAML::BeginMap;

            auto& sprite_render_comp = entity.get<SpriteRendererComponent>();
            out << YAML::Key << "Color" << YAML::Value << sprite_render_comp.color;

            out << YAML::EndMap;
        }

        out << YAML::EndMap;
    }

    void SceneSerializer::serialize(const std::string& file_path) {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key   << "Scene"              << YAML::Value << "Utitled";
        out << YAML::Key   << "ActiveCameraEntity" << YAML::Value << (int)(m_scene->m_active_camera_entity);
        out << YAML::Key   << "Entities"           << YAML::Value << YAML::BeginSeq;
        m_scene->m_registry.each([&](auto entity_ID) {
            Entity entity = { entity_ID, m_scene.get() };
            if (!entity) {
                return;
            }
            serializeEntity(out, entity);
        });
        out << YAML::EndSeq;
        out << YAML::EndMap;

        std::ofstream fout(file_path);
        fout << out.c_str();

    }

    void SceneSerializer::serializeBinary(const std::string& file_path) {

    }

    bool SceneSerializer::deserialize(const std::string& file_path) {
        std::ifstream     stream(file_path);
        std::stringstream str_stream;
        str_stream << stream.rdbuf();

        YAML::Node data = YAML::Load(str_stream.str());
        if (!data["Scene"]) {
            return false;
        }

        std::string scene_name = data["Scene"].as<std::string>();
        PR_CORE_INFO("Deserializing scene '{0}'", scene_name);

        //uint32_t active_camera_entity = data["ActiveCameraEntity"].as<int>(); // TODO: This will not work, need other solution
        //m_scene->m_active_camera_entity = (entt::entity)active_camera_entity;

        auto entities = data["Entities"];
        if (entities) {
            for (auto entity : entities) {
                uint64_t uuid = entity["Entity"].as<uint64_t>();
                
                std::string name;
                auto tag_comp_entry = entity["TagComponent"];
                if (tag_comp_entry) {
                    name = tag_comp_entry["Tag"].as<std::string>();
                }
                PR_CORE_INFO("Deserializing entity with ID =  {0}, name = {1}", uuid, name);

                Entity deserialized_entity = m_scene->createEntity(name); // TODO: Should be created with UUID

                auto transform_comp_entry = entity["TransformComponent"];
                if (transform_comp_entry) {
                    auto& transform_comp = deserialized_entity.get<TransformComponent>(); // Alwasy has Transform 
                    transform_comp.translation = transform_comp_entry["Translation"].as<glm::vec3>();
                    transform_comp.rotation    = transform_comp_entry["Rotation"   ].as<glm::vec3>();
                    transform_comp.scale       = transform_comp_entry["Scale"      ].as<glm::vec3>();
                }

                auto camera_comp_entry = entity["CameraComponent"];
                if (camera_comp_entry) {
                    auto& camera_comp = deserialized_entity.add<CameraComponent>();
                    auto& camera_props_entry = camera_comp_entry["Camera"];

                    camera_comp.camera.setProjectionType      ((ProjectionType)camera_props_entry["ProjectionType"  ].as<int  >());
                    camera_comp.camera.setPerspectiveFOV      (                camera_props_entry["ProjectionFOV"   ].as<float>());
                    camera_comp.camera.setPerspectiveNearClip (                camera_props_entry["PespectiveNear"  ].as<float>());
                    camera_comp.camera.setPerspectiveFarClip  (                camera_props_entry["PespectiveFar"   ].as<float>());
                    camera_comp.camera.setOrthographicSize    (                camera_props_entry["OrthographicSize"].as<float>());
                    camera_comp.camera.setOrthographicNearClip(                camera_props_entry["OrthographicNear"].as<float>());
                    camera_comp.camera.setOrthographicFarClip (                camera_props_entry["OrthographicFar" ].as<float>());

                    camera_comp.fixed_aspect_ratio = camera_comp_entry["FixedAspectRatio"].as<bool>();
                }

                auto sprite_renderer_comp_entry = entity["SpriteRendererComponent"];
                if (sprite_renderer_comp_entry) {
                    auto& sprite_renderer_comp = deserialized_entity.add<SpriteRendererComponent>();
                    sprite_renderer_comp.color = sprite_renderer_comp_entry["Color"].as<glm::vec4>();
                }
            }
        }

        return true;
    }

    bool SceneSerializer::deserializeBinary(const std::string& file_path) {
        return false;
    }

}