#pragma once

#include "parrot/renderer/Camera.h"

namespace parrot {

    enum class ProjectionType {
        Perspective  = 0,
        Orthographic = 1
    };

    class RenderCamera : public Camera {
    public:
        RenderCamera();
        virtual ~RenderCamera() = default;

        void setPerspective (float fov , float near_clip, float far_clip);
        void setOrthographic(float size, float near_clip, float far_clip);
        void setViewportSize(uint32_t width, uint32_t height);

        float getPerspectiveFOV     () const { return m_perspective_FOV ; }
        float getPerspectiveNearClip() const { return m_perspective_near; }
        float getPerspectiveFarClip () const { return m_perspective_far ; }

        void  setPerspectiveFOV(float fov) { 
            m_perspective_FOV = fov; 
            recalculateProjection();
        }

        void  setPerspectiveNearClip(float near_clip) {
            m_perspective_near = near_clip;
            recalculateProjection();
        }
        void  setPerspectiveFarClip (float far_clip ) {
            m_perspective_far = far_clip ;
            recalculateProjection();
        }

        float getOrthographicSize    () const { return m_orthographic_size; }
        float getOrthographicNearClip() const { return m_orthographic_near; }
        float getOrthographicFarClip () const { return m_orthographic_far ; }

        void  setOrthographicSize(float size) { 
            m_orthographic_size = size; 
            recalculateProjection();
        }

        void  setOrthographicNearClip(float near_clip) {
            m_orthographic_near = near_clip;
            recalculateProjection();
        }
        void  setOrthographicFarClip (float far_clip ) {
            m_orthographic_far = far_clip ;
            recalculateProjection();
        }

        ProjectionType getProjectionType() const { return m_projection_type; }
        void           setProjectionType(ProjectionType type) {
            m_projection_type = type; 
            recalculateProjection();
        }
    private:
        void recalculateProjection();

        ProjectionType m_projection_type = ProjectionType::Orthographic;

        float m_perspective_FOV  = 45.0f;
        float m_perspective_near = 0.01f;
        float m_perspective_far  = 1000.0f;

        float m_orthographic_size = 10.0f;
        float m_orthographic_near = -1.0f;
        float m_orthographic_far  =  1.0f;

        float m_aspect_ratio = 1.0f;
    };

    
}