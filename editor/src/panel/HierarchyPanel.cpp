#include "panel/HierarchyPanel.h"

#include <imgui.h>

namespace parrot {

    HierarchyPanel::HierarchyPanel(const Ref<Scene>& scene) {
        setScene(scene);
    }

    void HierarchyPanel::setScene(const Ref<Scene>& scene) {
        m_scene = scene;
    }

    void HierarchyPanel::onImGuiRender() {
        ImGui::Begin("Hierarchy");

        m_scene->m_registry.each([&](auto entity_ID) {
            Entity entity{ entity_ID, m_scene.get() };
            drawEntityNode(entity);
        });

        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
            m_selected_entity = {};
        }

        ImGui::End();

        ImGui::Begin("Properties");
        if (m_selected_entity) {
            drawComponents(m_selected_entity);
        }

        ImGui::End();
    }

    void HierarchyPanel::drawEntityNode(Entity entity) {
        auto& tag = entity.get<TagComponent>().tag;

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ((m_selected_entity == entity) ? ImGuiTreeNodeFlags_Selected : 0);
        bool opened = ImGui::TreeNodeEx((void*)(size_t)(uint32_t)entity, flags, tag.c_str());
        if (ImGui::IsItemClicked()) {
            m_selected_entity = entity;
        }

        if (opened) {
            ImGui::TreePop();
        }
    }

    void HierarchyPanel::drawComponents(Entity entity) {
        if (entity.hasComponent<TagComponent>()) {
            auto& tag = entity.get<TagComponent>().tag;

            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy_s(buffer, sizeof(buffer), tag.c_str());
            if (ImGui::InputText("Tag", buffer, sizeof(buffer))) {
                tag = std::string(buffer);
            }

        }

        if (entity.hasComponent<TransformComponent>()) {

            if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {
                auto& transform = entity.get<TransformComponent>().transform;
                ImGui::DragFloat3("position", glm::value_ptr(transform[3]), 0.1f);

                ImGui::TreePop();
            }

        }
    }

}