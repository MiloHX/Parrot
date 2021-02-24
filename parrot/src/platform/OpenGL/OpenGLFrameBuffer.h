#pragma once

#include "parrot/renderer/FrameBuffer.h"


namespace parrot {

    class OpenGLFrameBuffer : public FrameBuffer {
    public:
        OpenGLFrameBuffer(const FrameBufferProps& frame_buffer_props);
        virtual ~OpenGLFrameBuffer();

        void invalidate();
        virtual void bind() override;
        virtual void unbind() override;

        virtual uint32_t getColorAttachmentRendererID() const override { return m_color_attachment; }
        virtual const FrameBufferProps& getFrameBufferProps() const override { return m_frame_buffer_props; }
    private:
        uint32_t         m_renderer_ID;
        uint32_t         m_color_attachment;
        uint32_t         m_depth_attachment;
        FrameBufferProps m_frame_buffer_props;
    };
}