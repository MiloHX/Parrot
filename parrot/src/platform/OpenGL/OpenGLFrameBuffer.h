#pragma once

#include "parrot/renderer/FrameBuffer.h"


namespace parrot {

    class OpenGLFrameBuffer : public FrameBuffer {
    public:
        OpenGLFrameBuffer(const FrameBufferProps& frame_buffer_props);
        virtual ~OpenGLFrameBuffer();

        void invalidate();
        virtual void bind  () override;
        virtual void unbind() override;
        virtual void resize(uint32_t width, uint32_t height) override;

        virtual uint32_t getColorAttachmentRendererID() const override { return m_color_attachment; }
        virtual const FrameBufferProps& getFrameBufferProps() const override { return m_frame_buffer_props; }
    private:
        uint32_t         m_renderer_ID      = 0;
        uint32_t         m_color_attachment = 0;
        uint32_t         m_depth_attachment = 0;
        FrameBufferProps m_frame_buffer_props;
    };
}