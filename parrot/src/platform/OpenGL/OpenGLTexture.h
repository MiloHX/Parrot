#pragma once

#include "parrot/renderer/Texture.h"

#include <glad/glad.h>

namespace parrot {

    class OpenGLTexture2D : public Texture2D {
    public:
        OpenGLTexture2D(uint32_t width, uint32_t height);
        OpenGLTexture2D(const std::string& path);
        virtual ~OpenGLTexture2D();

        virtual uint32_t getRendererID() const override { return m_renderer_id; }
        virtual uint32_t getWidth     () const override { return m_width      ; }
        virtual uint32_t getHeight    () const override { return m_height     ; }

        virtual void     bind     (uint32_t slot) const       override; 
        virtual void     setData  (void* data, uint32_t size) override;

        virtual bool     operator==(const Texture& other) const override {
            return m_renderer_id == ((OpenGLTexture2D&)other).m_renderer_id;
        }

    private:
        std::string m_path;
        uint32_t    m_width       = 0;
        uint32_t    m_height      = 0;
        uint32_t    m_renderer_id = 0;
        GLenum      m_internal_format;
        GLenum      m_data_format;
    };

}