#include "prpch.h"
#include "SubTexture2D.h"

namespace parrot {

    SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max) :
        m_texture(texture) {
        m_texture_coord_list[0] = glm::vec2(min.x, min.y);
        m_texture_coord_list[1] = glm::vec2(max.x, min.y);
        m_texture_coord_list[2] = glm::vec2(max.x, max.y);
        m_texture_coord_list[3] = glm::vec2(min.x, max.y);
    }

    Ref<SubTexture2D> SubTexture2D::createFromCoords(
        const Ref<Texture2D>& texture, 
        const glm::vec2&      coords , 
        const glm::vec2&      cell_size,
        const glm::vec2&      cell_counts
    ) {
        glm::vec2 min = glm::vec2(((coords.x                )*cell_size.x) / texture->getWidth(), ((coords.y                )*cell_size.y) / texture->getHeight()); 
        glm::vec2 max = glm::vec2(((coords.x + cell_counts.x)*cell_size.x) / texture->getWidth(), ((coords.y + cell_counts.y)*cell_size.y) / texture->getHeight()); 

        return createRef<SubTexture2D>(texture, min, max);
    }

}