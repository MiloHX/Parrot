#include "prpch.h"
#include "parrot/ecs/Scene.h"
#include "parrot/ecs/Entity.h"
#include "parrot/ecs/Component.h"
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
        // Script
        m_registry.view<ScriptComponent>().each([=](auto entity, auto& script) {
            if (!script.instance) {
                script.instantiateFunction(script.instance);
                if (script.instance) {
                    script.instance->m_entity = Entity{ entity, this };
                }
                if (script.onCreateFunction) {
                    script.onCreateFunction(script.instance);
                }
            }
            if (script.onUpdateFunction) {
                script.onUpdateFunction(script.instance, time_step);
            }
        });

        // Render
        if (m_registry.valid(m_active_camera_entity)) {
            auto& camera          = m_registry.get<CameraComponent   >(m_active_camera_entity).camera;
            auto& camera_tranform = m_registry.get<TransformComponent>(m_active_camera_entity).transform;
            Renderer2D::beginScene(camera, camera_tranform);
            auto render_group = m_registry.view<TransformComponent, SpriteRendererComponent>();

            for (auto entity : render_group) {
                auto& [transform, sprite] = render_group.get<TransformComponent, SpriteRendererComponent>(entity);
                Renderer2D::drawQuad(transform, nullptr, sprite.color);
            }

            Renderer2D::endScene();
        }

    }

    void Scene::onViewportResize(uint32_t width, uint32_t height) {
        auto view = m_registry.view<CameraComponent>();
        for (auto entity : view) {
            auto& camera_component = view.get<CameraComponent>(entity);
            if (!camera_component.fixed_aspect_ratio) {
                camera_component.camera.setViewportSize(width, height);
            }
        }
    }
    void Scene::setActiveCamera(Entity camera_entity){
        m_active_camera_entity = camera_entity.m_entity_handle;
    }
}