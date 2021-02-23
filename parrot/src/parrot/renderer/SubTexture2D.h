#pragma once

#include "Texture.h"

#include <glm/glm.hpp>

namespace parrot {

    class SubTexture2D {
    public:
        SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

        const Ref<Texture2D> getTexture     () const { return m_texture; }
        const glm::vec2*     getTexCoordList() const { return m_texture_coord_list; }

        static Ref<SubTexture2D> createFromCoords(
            const Ref<Texture2D>& texture, 
            const glm::vec2&      coords , 
            const glm::vec2&      cell_size,
            const glm::vec2&      cell_counts = glm::vec2(1, 1)
        );
    private:
        Ref<Texture2D> m_texture;
        glm::vec2      m_texture_coord_list[4];
    };
}