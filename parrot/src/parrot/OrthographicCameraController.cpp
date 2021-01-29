#include "prpch.h"
#include "OrthographicCameraController.h"
#include "parrot/Input.h"
#include "parrot/KeyCode.h"

namespace parrot {

    OrthographicCameraController::OrthographicCameraController(float aspect_ratio, bool rotation) 
    : m_aspect_ratio(aspect_ratio), m_rotation(rotation), m_camera(-m_aspect_ratio * m_zoom_level, m_aspect_ratio *m_zoom_level, -m_zoom_level, m_zoom_level) {
    }

    void OrthographicCameraController::onUpdate(TimeStep time_step) {

        if (Input::isKeyPressed(PR_KEY_A)) {
            m_camera_position.x -= m_camera_move_speed * time_step;
        } else if (Input::isKeyPressed(PR_KEY_D)) {
            m_camera_position.x += m_camera_move_speed * time_step;
        }

        if (Input::isKeyPressed(PR_KEY_W)) {
            m_camera_position.y += m_camera_move_speed * time_step;
        } else if (Input::isKeyPressed(PR_KEY_S)) {
            m_camera_position.y -= m_camera_move_speed * time_step;
        }

        if (m_rotation) {
            if (Input::isKeyPressed(PR_KEY_Q)) {
                m_camera_rotation += m_camera_rotation_speed * time_step;
            } else if (Input::isKeyPressed(PR_KEY_E)) {
                m_camera_rotation -= m_camera_rotation_speed * time_step;
            }
            m_camera.setZRotation(m_camera_rotation);
        }

        m_camera.setPosition(m_camera_position);

        m_camera_move_speed = m_zoom_level;
    }

    void OrthographicCameraController::onEvent(Event& event) {
        EventDispatcher dispatcher(event);
        dispatcher.dispatch<MouseScrolledEvent>(PR_BIND_EVENT_FUNC(OrthographicCameraController::onMouseScrolled));
        dispatcher.dispatch<WindowResizedEvent>(PR_BIND_EVENT_FUNC(OrthographicCameraController::onWindowResized));
    }

    bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& event) {

        m_zoom_level -= event.getOffsetY() * 0.5f;
        m_zoom_level = glm::clamp(m_zoom_level, 0.25f, 10.0f);
        m_camera.setProjection(-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level);
        return false;
    }

    bool OrthographicCameraController::onWindowResized(WindowResizedEvent& event) {
        m_aspect_ratio = static_cast<float>(event.getWidth()) / static_cast<float>(event.getHeight());
        m_camera.setProjection(-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level);
        return false;
    }

}