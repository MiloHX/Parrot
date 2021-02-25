#pragma once

#include "parrot/renderer/Camera.h"

namespace parrot {

    class RenderCamera : public Camera {
    public:
        RenderCamera();
        virtual ~RenderCamera() = default;

        void setOrthographic(float size, float near_clip, float far_clip);
        void setViewportSize(uint32_t width, uint32_t height);

        float getOrthographicSize() const { return m_orthographic_size; }
        void  setOrthographicSize(float size) { 
            m_orthographic_size = size; 
            recalculateProjection();
        }
    private:
        void recalculateProjection();

        float m_orthographic_size = 10.0f;
        float m_orthographic_near = -1.0f;
        float m_orthographic_far  =  1.0f;

        float m_aspect_ratio = 1.0f;
    };

    
}