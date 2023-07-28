#ifndef __GameDev2D__Font__
#define __GameDev2D__Font__

#include "GraphicTypes.h"
#include "Texture.h"
#include <string>
#include <map>


namespace GameDev2D
{
    //Font leverages the FreeType library to dynamically generate a texture atlas for a given font size and
    //character set. It renders the font data to a RenderTarget. This class supports both .TTF and .OTF font extensions.
    class Font
    {
    public:
        //A font should only be generated through the ResourceManager
        Font(FontData& fontData, Texture& texture);
        ~Font();

        //Returns the name of the font and the extension
        std::string GetName();
        std::string GetExtension();

        //Returns the character set for the font, the characters that the font can render
        std::string GetCharacterSet();

        //Returns the line height, base line and font size for the font
        unsigned int GetLineHeight();
        unsigned int GetBaseLine();
        unsigned int GetSize();

        //Returns the Texture
        Texture* GetTexture();

        //Methods to get the rect, x advance and bearing data for a given character
        Rect GetTextureRectForCharacter(char character);
        unsigned short GetAdvanceXForCharacter(char character);
        char GetBearingXForCharacter(char character);
        char GetBearingYForCharacter(char character);

    private:
        //Member variables
        Texture* m_Texture;
        std::map<char, GlyphData> m_GlyphData;
        const std::string m_Name;
        const std::string m_Extension;
        const std::string m_CharacterSet;
        const unsigned int m_LineHeight;
        const unsigned int m_BaseLine;
        const unsigned int m_Size;
    };
}

#endif
