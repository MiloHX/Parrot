#include "prpch.h"
#include "parrot/renderer/OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace parrot {

    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top) 
        : m_projection_matrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_view_matrix(1.0f) {
        m_view_projection_matrix = m_projection_matrix * m_view_matrix;
    }

    void OrthographicCamera::setProjection(float left, float right, float bottom, float top) {
        m_projection_matrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
        m_view_projection_matrix = m_projection_matrix * m_view_matrix;
    }

    void OrthographicCamera::recalculatreViewMatrix() {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position) * 
                              glm::rotate(glm::mat4(1.0f), m_z_rotation, glm::vec3(0, 0, 1));

        m_view_matrix            = glm::inverse(transform);
        m_view_projection_matrix = m_projection_matrix * m_view_matrix;
    }
}