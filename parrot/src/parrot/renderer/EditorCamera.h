#pragma once

#include "parrot/renderer/Camera.h"
#include "parrot/core/TimeStep.h"
#include "parrot/event/Event.h"
#include "parrot/event/MouseEvent.h"


namespace parrot {

    class EditorCamera : public Camera {
    public:
        EditorCamera() = default;
        EditorCamera(float fov, float aspect_ratio, float near_clip, float far_clip);

        void onUpdate(TimeStep time_step);
        void onEvent (Event&   event    );

        inline float getDistance() const { return m_distance; }
        inline void  setDistance(float distance) { m_distance = distance; }
        
        inline void setViewportSize(float width, float height) {
            m_viewport_width = width;
            m_viewport_height = height;
            updateProjection();
        }

        const glm::mat4& getViewMatrix    () const { return m_view_matrix; }
        glm::mat4        getViewProjection() const { return m_projection * m_view_matrix; }
        glm::vec3        getUpDirection     () const;
        glm::vec3        getRightDirection  () const;
        glm::vec3        getForwardDirection() const;
        const glm::vec3& getPosition        () const { return m_position; }
        glm::quat        getOrientation     () const;
        float            getPitch           () const { return m_pitch; }
        float            getYaw             () const { return m_yaw;   }

    private:
        void updateProjection();
        void updateView();
        
        bool onMouseScroll(MouseScrolledEvent& e);
        void mousePan   (const glm::vec2& delta);
        void mouseRotate(const glm::vec2& delta);
        void mouseZoom  (float delta);

        glm::vec3 calculatePosition() const;

        std::pair<float, float> panSpeed() const;
        float rotateSpeed() const;
        float zoomSpeed() const;

        float m_FOV = 45.0f;
        float m_apsect_ratio = 1.778f;
        float m_near_clip = 0.1f;
        float m_far_clip = 1000.0f;

        glm::mat4 m_view_matrix;
        glm::vec3 m_position    = { 0.0f, 0.0f, 0.0f };
        glm::vec3 m_focal_point = { 0.0f, 0.0f, 0.0f };
        glm::vec2 m_intial_mouse_position;

        float m_distance = 10.0f;
        float m_pitch    = 0.0f;
        float m_yaw      = 0.0f;
        float m_viewport_width  = 1920;
        float m_viewport_height = 1080;
    };

}
