#include "prpch.h"
#include "parrot/renderer/RenderCamera.h"

namespace parrot {

    RenderCamera::RenderCamera() {
        recalculateProjection();
    }

    void RenderCamera::setPerspective(float fov, float near_clip, float far_clip) {
        m_projection_type = ProjectionType::Perspective;
        m_perspective_FOV  = fov;
        m_perspective_near = near_clip;
        m_perspective_far  = far_clip;
        recalculateProjection();
    }

    void RenderCamera::setOrthographic(float size, float near_clip, float far_clip) {
        m_projection_type = ProjectionType::Orthographic;
        m_orthographic_size = size;
        m_orthographic_near = near_clip;
        m_orthographic_far  = far_clip;
        recalculateProjection();
    }

    void RenderCamera::setViewportSize(uint32_t width, uint32_t height) {
        m_aspect_ratio     = static_cast<float>(width) / static_cast<float>(height);
        recalculateProjection();
    }

    void RenderCamera::recalculateProjection() {
        if (m_projection_type == ProjectionType::Perspective) {
            m_projection = glm::perspective(m_perspective_FOV, m_aspect_ratio, m_perspective_near, m_perspective_far);
        } else {
            float ortho_left   = -m_orthographic_size * m_aspect_ratio * 0.5f;
            float ortho_right  =  m_orthographic_size * m_aspect_ratio * 0.5f;
            float ortho_bottom = -m_orthographic_size * 0.5f;
            float ortho_top    =  m_orthographic_size * 0.5f;
            m_projection = glm::ortho(ortho_left, ortho_right, ortho_bottom, ortho_top, m_orthographic_near, m_orthographic_far);
        }
    }

}