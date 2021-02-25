#pragma once

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
        OrthographicCameraController m_camera_controller;
        Ref<VertexArray>             m_vertex_array;
        Ref<Shader>                  m_flat_color_shader;
        Ref<Texture2D>               m_checkerboard_texture;
        Ref<FrameBuffer>             m_frame_buffer;
        glm::vec2                    m_viewport_size    = glm::vec2(0, 0);
        bool                         m_viewport_focused = false;
        bool                         m_viewport_hovered = false;

        Ref<Scene>                   m_active_scene;
        entt::entity                 m_square_entity;
    };
}