#include "prpch.h"
#include "parrot/core/Core.h"
#include "platform/OpenGL/OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace parrot {
    OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferProps& frame_buffer_props) : 
        m_frame_buffer_props(frame_buffer_props) {
        invalidate();
    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer() {
        glDeleteFramebuffers(1, &m_renderer_ID);
    }

    void OpenGLFrameBuffer::invalidate() {
        glCreateFramebuffers(1, &m_renderer_ID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_ID);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_color_attachment);
        glBindTexture   (GL_TEXTURE_2D, m_color_attachment);
        glTextureStorage2D(m_color_attachment, 1, GL_RGBA8, m_frame_buffer_props.width, m_frame_buffer_props.height);
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color_attachment, 0);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_depth_attachment);
        glBindTexture   (GL_TEXTURE_2D, m_depth_attachment);
        glTextureStorage2D(m_depth_attachment, 1, GL_DEPTH24_STENCIL8, m_frame_buffer_props.width, m_frame_buffer_props.height);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depth_attachment, 0);

        PR_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete")

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFrameBuffer::bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_ID);
    }

    void OpenGLFrameBuffer::unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}