#include "panel/HierarchyPanel.h"

#include <imgui.h>
#include <imgui_internal.h>

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

        if (ImGui::BeginPopupContextWindow(0, 1, false)) {
            if (ImGui::MenuItem("Create Empty Entity")) {
                m_scene->createEntity("Empty Entity");
            }
            ImGui::EndPopup();
        }

        ImGui::End();

        ImGui::Begin("Properties");
        if (m_selected_entity) {
            drawComponents(m_selected_entity);

            //if (ImGui::Button("Add Component")) {
            //    ImGui::OpenPopup("AddComponent");
            //}

            //if (ImGui::BeginPopup("AddComponent")) {
            //    if (ImGui::MenuItem("Camera")) {
            //        m_selected_entity.add<CameraComponent>();
            //        ImGui::CloseCurrentPopup();
            //    }

            //    if (ImGui::MenuItem("Sprite Renderer")) {
            //        m_selected_entity.add<SpriteRendererComponent>();
            //        ImGui::CloseCurrentPopup();
            //    }

            //    ImGui::EndPopup();
            //}

        }

        ImGui::End();
    }

    void HierarchyPanel::drawEntityNode(Entity& entity) {
        auto& tag = entity.get<TagComponent>().tag;

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ((m_selected_entity == entity) ? ImGuiTreeNodeFlags_Selected : 0);
        flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
        bool opened = ImGui::TreeNodeEx((void*)(size_t)(uint32_t)entity, flags, tag.c_str());
        if (ImGui::IsItemClicked()) {
            m_selected_entity = entity;
        }

        bool entity_deleted = false;
        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Delete Entity")) {
                entity_deleted = true;
            }
            ImGui::EndPopup();
        }

        if (opened) {
            ImGui::TreePop();
        }

        if (entity_deleted) {
            if (m_selected_entity == entity) {
                m_selected_entity = {};
            }
            m_scene->destroyEntity(entity);
        }
    }

    static void drawVec3Control(const std::string& label, glm::vec3& values, float reset_value=0.0f, float column_width = 100.0f) {
        ImGuiIO& io = ImGui::GetIO();
        auto bold_font = io.Fonts->Fonts[1];

        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, column_width);
        ImGui::Text(label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

        float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 button_size = { line_height + 3.0f, line_height };

        ImGui::PushStyleColor(ImGuiCol_Button       , ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f,  0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive , ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
        ImGui::PushFont(bold_font);
        if (ImGui::Button("X", button_size)) {
            values.x = reset_value;
        }
        ImGui::PopFont();
        ImGui::SameLine();
        ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::PopStyleColor(3);

        ImGui::PushStyleColor(ImGuiCol_Button       , ImVec4(0.2f, 0.7f,  0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f,  0.3f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive , ImVec4(0.2f, 0.7f,  0.2f, 1.0f));
        ImGui::PushFont(bold_font);
        if (ImGui::Button("Y", button_size)) {
            values.y = reset_value;
        }
        ImGui::PopFont();
        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::PopStyleColor(3);

        ImGui::PushStyleColor(ImGuiCol_Button       , ImVec4(0.1f, 0.25f,  0.8f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f,  0.3f,  0.9f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive , ImVec4(0.1f, 0.25f,  0.8f, 1.0f));
        ImGui::PushFont(bold_font);
        if (ImGui::Button("Z", button_size)) {
            values.z = reset_value;
        }
        ImGui::PopFont();
        ImGui::SameLine();
        ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();
    }

    template <typename T, typename UIFunction>
    static void DrawComponent(const std::string& name, Entity& entity, UIFunction uiFunction) {
        const ImGuiTreeNodeFlags tree_note_flags = 
            ImGuiTreeNodeFlags_DefaultOpen      | 
            ImGuiTreeNodeFlags_AllowItemOverlap |
            ImGuiTreeNodeFlags_SpanAvailWidth   |
            ImGuiTreeNodeFlags_Framed           |
            ImGuiTreeNodeFlags_FramePadding;
        if (entity.has<T>()) {
            auto& component = entity.get<T>();
            ImVec2 content_region_avail = ImGui::GetContentRegionAvail();

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
            float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImGui::Separator();
            bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), tree_note_flags, name.c_str());
            ImGui::PopStyleVar();
            ImGui::SameLine(content_region_avail.x - line_height * 0.5f);  // TODO Not universal
            if (ImGui::Button("+", ImVec2{ line_height, line_height })) {
                ImGui::OpenPopup("ComponentSettings");
            }
            bool remove_component = false;
            if (ImGui::BeginPopup("ComponentSettings")) {
                if (ImGui::MenuItem("Remove Component")) {
                    remove_component = true;
                }
                ImGui::EndPopup();
            }

            if (open) {
                uiFunction(component);
                ImGui::TreePop();
            }

            if (remove_component) {
                entity.remove<T>();
            }
        }
    }

    void HierarchyPanel::drawComponents(Entity& entity) {
        if (entity.has<TagComponent>()) {
            auto& tag = entity.get<TagComponent>().tag;

            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy_s(buffer, sizeof(buffer), tag.c_str());
            if (ImGui::InputText("##Tag", buffer, sizeof(buffer))) {
                tag = std::string(buffer);
            }
        }

        ImGui::SameLine();
        ImGui::PushItemWidth(-1);

        if (ImGui::Button("Add Component")) {
            ImGui::OpenPopup("AddComponent");
        }

        if (ImGui::BeginPopup("AddComponent")) {
            if (ImGui::MenuItem("Camera")) {
                m_selected_entity.add<CameraComponent>();
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("Sprite Renderer")) {
                m_selected_entity.add<SpriteRendererComponent>();
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        ImGui::PopItemWidth();

        DrawComponent<TransformComponent>("Transform", entity, [](auto& component) {

            drawVec3Control("Translation", component.translation);
            drawVec3Control("Rotation"   , component.rotation   );
            drawVec3Control("Scale"      , component.scale      , 1.0f);
        });

        DrawComponent<CameraComponent>("Camera", entity, [&](auto& component) {
            auto& camera           = component.camera;

            bool set_as_active = m_scene->isActiveCamera(entity);
            if (ImGui::Checkbox("Set As Active Camera", &set_as_active)) {
                if (set_as_active) {
                    m_scene->setActiveCamera(entity);
                } else {
                    m_scene->setActiveCamera(Entity{});
                }
            }
            ImGui::Checkbox("Fixed Aspect Ratio", &component.fixed_aspect_ratio);

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

        });

        DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component) {
            ImGui::ColorEdit4("Color", glm::value_ptr(component.color));
        });

    }

}