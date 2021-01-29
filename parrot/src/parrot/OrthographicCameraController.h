#pragma once

#include "renderer/OrthographicCamera.h"
#include "parrot/core/TimeStep.h"
#include "parrot/event/ApplicationEvent.h"
#include "parrot/event/MouseEvent.h"

namespace parrot {

    class OrthographicCameraController {
    public:
        OrthographicCameraController(float aspect_ratio, bool rotation = false);

        OrthographicCamera&       getCamera()       { return m_camera; }
        const OrthographicCamera& getCamera() const { return m_camera; }
        void onUpdate(TimeStep time_step);
        void onEvent(Event& event);
    private:
        bool onMouseScrolled(MouseScrolledEvent& event);
        bool onWindowResized(WindowResizedEvent& event);

        float              m_aspect_ratio          = 1.0f;
        float              m_zoom_level            = 1.0f;
        bool               m_rotation              = false;
        glm::vec3          m_camera_position       = glm::vec3{ 0.0f, 0.0f, 0.0f };
        float              m_camera_rotation       = 0.0f;
        float              m_camera_move_speed     = 1.0f;
        float              m_camera_rotation_speed = 30.0f;
        OrthographicCamera m_camera;
    };

}

