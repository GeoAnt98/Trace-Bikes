#include "Font.h"
#include "../Services/Services.h"


namespace GameDev2D
{
    Font::Font(FontData& aFontData, Texture& aTexture) :
        m_Texture(nullptr),
        m_GlyphData(aFontData.glyphData),
        m_Name(aFontData.name),
        m_Extension(aFontData.extension),
        m_CharacterSet(aFontData.characterSet),
        m_LineHeight(aFontData.lineHeight),
        m_BaseLine(aFontData.baseLine),
        m_Size(aFontData.size)
    {
        m_Texture = new Texture(aTexture);
    }

    Font::~Font()
    {
        if (m_Texture != nullptr)
        {
            delete m_Texture;
            m_Texture = nullptr;
        }
    }

    std::string Font::GetName()
    {
        return m_Name;
    }

    std::string Font::GetExtension()
    {
        return m_Extension;
    }

    std::string Font::GetCharacterSet()
    {
        return m_CharacterSet;
    }

    unsigned int Font::GetLineHeight()
    {
        return m_LineHeight;
    }

    unsigned int Font::GetBaseLine()
    {
        return m_BaseLine;
    }

    unsigned int Font::GetSize()
    {
        return m_Size;
    }

    Texture* Font::GetTexture()
    {
        return m_Texture;
    }

    Rect Font::GetTextureRectForCharacter(char aCharacter)
    {
        return m_GlyphData[aCharacter].frame;
    }

    unsigned short Font::GetAdvanceXForCharacter(char aCharacter)
    {
        return m_GlyphData[aCharacter].advanceX;
    }

    char Font::GetBearingXForCharacter(char aCharacter)
    {
        return m_GlyphData[aCharacter].bearingX;
    }

    char Font::GetBearingYForCharacter(char aCharacter)
    {
        return m_GlyphData[aCharacter].bearingY;
    }
}