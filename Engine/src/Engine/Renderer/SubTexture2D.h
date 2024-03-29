#pragma once

#include "Engine/Core/Base.h"
#include "Engine/Renderer/Texture.h"

#include <glm/glm.hpp>

namespace Engine
{
    class SubTexture2D
    {
    public:
        SubTexture2D(const Ref<Texture2D>& texture);
        SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

        const Ref<Texture2D>& GetTexture() const { return m_Texture; }
        const glm::vec2* GetTextureCoords() const { return m_TexCoords; }
        
        static Ref<SubTexture2D> Create(const Ref<Texture2D>& texture);
        static Ref<SubTexture2D> Create(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cell, const glm::vec2& size = { 1.0f, 1.0f });

    private:
        Ref<Texture2D> m_Texture;
        glm::vec2 m_TexCoords[4];
    };
}