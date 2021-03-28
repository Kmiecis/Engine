#include "SubTexture2D.h"

namespace Engine
{
    SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture) :
        SubTexture2D(texture, { 0.0f, 0.0f }, { 1.0f, 1.0f })
    {
    }

    SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max) :
        m_Texture(texture)
    {
        m_TexCoords[0] = { min.x, min.y };
        m_TexCoords[1] = { max.x, min.y };
        m_TexCoords[2] = { max.x, max.y };
        m_TexCoords[3] = { min.x, max.y };
    }
    
    Ref<SubTexture2D> SubTexture2D::Create(const Ref<Texture2D>& texture)
    {
        return CreateRef<SubTexture2D>(texture);
    }

    Ref<SubTexture2D> SubTexture2D::Create(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cell, const glm::vec2& size)
    {
        glm::vec2 min = { coords.x * cell.x / texture->GetWidth(), coords.y * cell.y / texture->GetHeight() };
        glm::vec2 max = { (coords.x + size.x) * cell.x / texture->GetWidth(), (coords.y + size.y) * cell.y / texture->GetHeight() };

        return CreateRef<SubTexture2D>(texture, min, max);
    }
}