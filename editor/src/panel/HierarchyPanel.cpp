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

        if (entity.hasComponent<CameraComponent>()) {

            if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {
                auto& camera_component = entity.get<CameraComponent>();
                auto& camera           = camera_component.camera;

                ImGui::Checkbox("Fixed Aspect Ratio", &camera_component.fixed_aspect_ratio);

                const char* projection_type_strings[] = { "Perspective", "Orthographic" };
                const char* current_projection_type_string = projection_type_strings[(int)camera.getProjectionType()];
                if (ImGui::BeginCombo("Projection", current_projection_type_string)) {

                    for (int i = 0; i < 2; i++) {
                        bool is_selected = current_projection_type_string == projection_type_strings[i];
                        if (ImGui::Selectable(projection_type_strings[i], is_selected)) {
                            current_projection_type_string = projection_type_strings[i];
                            camera.setProjectionType((ProjectionType)i);
                        }

                        if (is_selected) {
                            ImGui::SetItemDefaultFocus();
                        }
                    }

                    ImGui::EndCombo();
                }

                // TODO Bad to calculate projection multiple times, need review
                if (camera.getProjectionType() == ProjectionType::Perspective) {
                    float persp_fov  = camera.getPerspectiveFOV    ();
                    float persp_near = camera.getPerspectiveNearClip();
                    float persp_far  = camera.getPerspectiveFarClip ();

                    if (ImGui::DragFloat("FOV", &persp_fov)) {
                        camera.setPerspectiveFOV(persp_fov);
                    }
                    if (ImGui::DragFloat("Near", &persp_near)) {
                        camera.setPerspectiveNearClip(persp_near);
                    }
                    if (ImGui::DragFloat("Far", &persp_far)) {
                        camera.setPerspectiveFarClip(persp_far);
                    }
                }

                if (camera.getProjectionType() == ProjectionType::Orthographic) {
                    float ortho_size = camera.getOrthographicSize    ();
                    float ortho_near = camera.getOrthographicNearClip();
                    float ortho_far  = camera.getOrthographicFarClip ();

                    if (ImGui::DragFloat("Size", &ortho_size)) {
                        camera.setOrthographicSize(ortho_size);
                    }
                    if (ImGui::DragFloat("Near", &ortho_near)) {
                        camera.setOrthographicNearClip(ortho_near);
                    }
                    if (ImGui::DragFloat("Far", &ortho_far)) {
                        camera.setOrthographicFarClip(ortho_far);
                    }
                }

                ImGui::TreePop();
            }

        }

    }

}