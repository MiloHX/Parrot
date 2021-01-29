#pragma once

#include <glm/glm.hpp>

namespace parrot {

    class OrthographicCamera {
    public:
        OrthographicCamera(float left, float right, float bottom, float top);

        const glm::vec3& getPosition () const { return m_position; }
        float            getZRotation() const { return m_z_rotation; }
        void             setPosition (const glm::vec3& position) {
            m_position = position;
            recalculatreViewMatrix();
        }
        void             setZRotation(float z_rotation) {
            m_z_rotation = z_rotation; 
            recalculatreViewMatrix();
        }

        const glm::mat4& getProjectionMatrix    () const { return m_projection_matrix;      }
        const glm::mat4& getViewMatrix          () const { return m_view_matrix;            }
        const glm::mat4& getViewPorjectionMatrix() const { return m_view_projection_matrix; }
    private:
        void recalculatreViewMatrix();

        glm::mat4 m_projection_matrix;
        glm::mat4 m_view_matrix;
        glm::mat4 m_view_projection_matrix;

        glm::vec3 m_position   = glm::vec3{ 0.0f, 0.0f, 0.0f };
        float     m_z_rotation = 0.0f;
    };
}