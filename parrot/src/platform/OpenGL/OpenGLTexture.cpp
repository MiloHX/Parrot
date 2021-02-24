#include "prpch.h"
#include "platform/OpenGL/OpenGLTexture.h"


#include <stb_image.h>

namespace parrot {

    OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height) : m_width(width), m_height(height) {

        m_internal_format = GL_RGBA8;
        m_data_format     = GL_RGBA;

        glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);
        glTextureStorage2D(m_renderer_id, 1, m_internal_format, m_width, m_height);

        glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_T, GL_REPEAT);

    }

    OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : m_path(path){
        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        PR_CORE_ASSERT(data, "Failed to load image from {0}", path);
        m_width  = width;
        m_height = height;

        if (channels == 4) {
            m_internal_format = GL_RGBA8;
            m_data_format     = GL_RGBA;
        } else if (channels == 3) {
            m_internal_format = GL_RGB8;
            m_data_format     = GL_RGB;
        } else {
            m_internal_format = GL_INVALID_ENUM;
            m_data_format     = GL_INVALID_ENUM;
        }
        

        PR_CORE_ASSERT(m_internal_format != GL_INVALID_ENUM && m_data_format != GL_INVALID_ENUM, "Texture format not supported");

        glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);
        glTextureStorage2D(m_renderer_id, 1, m_internal_format, m_width, m_height);

        glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(m_renderer_id, 0, 0, 0, m_width, m_height, m_data_format, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    OpenGLTexture2D::~OpenGLTexture2D() {
        glDeleteTextures(1, &m_renderer_id);
    }

    void OpenGLTexture2D::bind(uint32_t slot) const {
        glBindTextureUnit(slot, m_renderer_id);
    }

    void OpenGLTexture2D::setData(void* data, uint32_t size) {
        uint32_t byte_per_pixel = m_data_format == GL_RGBA ? 4 : 3;
        PR_CORE_ASSERT(size == m_width * m_height * byte_per_pixel, "Texture Data is not matching" );
        glTextureSubImage2D(m_renderer_id, 0, 0, 0, m_width, m_height, m_data_format, GL_UNSIGNED_BYTE, data);
    }


}