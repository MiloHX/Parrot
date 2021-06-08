#include "prpch.h"
#include "EditorCamera.h"

#include "parrot/core/Input.h"
#include "parrot/core/KeyCode.h"

#include <glfw/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace parrot {

    EditorCamera::EditorCamera(float fov, float aspect_ratio, float near_clip, float far_clip) :
        m_FOV(fov),
        m_apsect_ratio(aspect_ratio),
        m_near_clip(near_clip),
        m_far_clip(far_clip),
        Camera(glm::perspective(glm::radians(fov), aspect_ratio, near_clip, far_clip)) {

        updateView();
    }

    void EditorCamera::updateProjection() {
        m_apsect_ratio = m_viewport_width / m_viewport_height;
        m_projection   = glm::perspective(glm::radians(m_FOV), m_apsect_ratio, m_near_clip, m_far_clip);
    }

    void EditorCamera::updateView() {
        m_position = calculatePosition();
        glm::quat orientation = getOrientation();
        m_view_matrix = glm::translate(glm::mat4(1.0f), m_position) * glm::toMat4(orientation);
        m_view_matrix = glm::inverse(m_view_matrix);
    }

    std::pair<float, float> EditorCamera::panSpeed() const {
        float x = std::min(m_viewport_width  / 1000.0f, 2.4f);
        float y = std::min(m_viewport_height / 1000.0f, 2.4f);

        float x_factor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;
        float y_factor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

        return { x_factor, y_factor };
    }

    float EditorCamera::rotateSpeed() const {
        return 0.8f;
    }

    float EditorCamera::zoomSpeed() const {
        float distance = m_distance * 0.2f;
        distance = std::max(distance, 0.0f);
        float speed = distance * distance;
        speed = std::min(speed, 100.0f);
        return speed;
    }

    void EditorCamera::onUpdate(TimeStep time_step) {
        if (Input::isKeyPressed(KeyCode::Key_Left_Alt)) {
            const glm::vec2& mouse{ Input::getMousePosition().first, Input::getMousePosition().second };
            glm::vec2 delta = (mouse - m_intial_mouse_position) * 0.003f;
            m_intial_mouse_position = mouse;

            if (Input::isMouseButtonPressed(MouseButton::Button_Middle)) {
                mousePan(delta);
            } else if (Input::isMouseButtonPressed(MouseButton::Button_Left)) {
                mouseRotate(delta);
            } else if (Input::isMouseButtonPressed(MouseButton::Button_Right)) {
                mouseZoom(delta.y);
            }
        }

        updateView();
    }

    void EditorCamera::onEvent(Event& event) {
        EventDispatcher dispatcher(event);
        dispatcher.dispatch<MouseScrolledEvent>(PR_BIND_EVENT_FUNC(EditorCamera::onMouseScroll));
    }

    bool EditorCamera::onMouseScroll(MouseScrolledEvent& event) {
        float delta = event.getOffsetY() * 0.1f;
        mouseZoom(delta);
        updateView();
        return false;
    }

    void EditorCamera::mousePan(const glm::vec2& delta) {
        auto [x_speed, y_speed] = panSpeed();
        m_focal_point += -getRightDirection() * delta.x * x_speed * m_distance;
        m_focal_point +=  getUpDirection   () * delta.y * y_speed * m_distance;
    }

    void EditorCamera::mouseRotate(const glm::vec2& delta) {
        float yaw_sign = getUpDirection().y < 0 ? -1.0f : 1.0f;
        m_yaw   += yaw_sign * delta.x * rotateSpeed();
        m_pitch += delta.y * rotateSpeed();
    }

    void EditorCamera::mouseZoom(float delta) {
        m_distance -= delta * zoomSpeed();
        if (m_distance < 1.0f) {
            m_focal_point += getForwardDirection();
            m_distance = 1.0f;
        }
    }

    glm::vec3 EditorCamera::getUpDirection() const {
        return glm::rotate(getOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::vec3 EditorCamera::getRightDirection() const {
        return glm::rotate(getOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    glm::vec3 EditorCamera::getForwardDirection() const {
        return glm::rotate(getOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
    }

    glm::vec3 EditorCamera::calculatePosition() const {
        return m_focal_point - getForwardDirection() * m_distance;
    }

    glm::quat EditorCamera::getOrientation() const {
        return glm::quat(glm::vec3(-m_pitch, -m_yaw, 0.0f));
    }


}