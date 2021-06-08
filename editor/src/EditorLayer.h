#pragma once

#include "panel/HierarchyPanel.h"

#include <Parrot.h>


namespace parrot {

    class EditorLayer : public Layer {
    public:
        EditorLayer();
        virtual ~EditorLayer() = default;

        void onAttach() override;
        void onDetach() override;
        void onUpdate(TimeStep time_step) override;
        void onImGuiRender() override;
        void onEvent(Event& event) override;
    private:
        bool onKeyPressed(KeyPressedEvent& event);
        void newScene();
        void openScene();
        void saveSceneAs();

        OrthographicCameraController m_camera_controller;
        Ref<VertexArray>             m_vertex_array;
        Ref<Shader>                  m_flat_color_shader;
        Ref<Texture2D>               m_checkerboard_texture;
        Ref<FrameBuffer>             m_frame_buffer;
        glm::vec2                    m_viewport_size    = glm::vec2(0, 0);
        bool                         m_viewport_focused = false;
        bool                         m_viewport_hovered = false;

        Ref<Scene> m_active_scene;
        Entity     m_square_entity;
        Entity     m_camera_primary;
        Entity     m_camera_secondary;
        bool       m_use_primary_camera = false;

        EditorCamera m_editor_camera;

        int m_gizmo_type = -1;

        HierarchyPanel m_hierarchy_panel;
    };
}