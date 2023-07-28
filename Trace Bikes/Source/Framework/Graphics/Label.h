#ifndef __GameDev2D__Label__
#define __GameDev2D__Label__

#include "../Core/Drawable.h"
#include <string>
#include <vector>


namespace GameDev2D
{
    //Forward declarations
    class Font;
    class SpriteBatch;

    //Creates a label from a .ttf or .otf font file. The label can be transform (translated, rotatated, scaled)
    class Label : public Drawable
    {
    public:
        //Contructs a Label using a TrueTypeFont, the font size must be specified
        Label(Font* font);
        ~Label();

        //Draws the Label
        void Draw();

        //Returns the size of the label
        float GetWidth();
        float GetHeight();

        //Sets the text that the Label will display
        void SetText(const std::string& text);

        //Returns the text the Label is displaying
        std::string GetText();

        //Set the spacing between each character
        void SetCharacterSpacing(float characterSpacing);

        //Returns the spacing between each character
        float GetCharacterSpacing();

        //Label Justification text can be left, center and right justified
        enum Justification
        {
            JustifyLeft = 0,
            JustifyCenter,
            JustifyRight
        };

        //Sets the justification of the Label
        void SetJustification(Justification justification);

        //Returns the justification of the Label
        Justification GetJustification();

        //
        struct CharacterData
        {
            CharacterData(Color color) :
                color(color),
                scale(1.0f, 1.0f),
                anchor(0.0f, 0.0f),
                angle(0.0f)
            {}

            CharacterData() :
                color(Color::WhiteColor()),
                scale(1.0f, 1.0f),
                anchor(0.0f, 0.0f),
                angle(0.0f)
            {}

            CharacterData(const CharacterData& characterData) :
                color(characterData.color),
                scale(characterData.scale),
                anchor(characterData.anchor),
                angle(characterData.angle)
            {}

            Color color;
            Vector2 scale;
            Vector2 anchor;            
            float angle;
        };

        //Returns a reference to the CharacterData so that you can modify each character's data individually
        std::vector<CharacterData>& GetCharacterData();

    protected:
        //Conveniance method to calculate the size of the Label, based on the text
        void CalculateSize();
        
        //Member variables
        Font* m_Font;
        SpriteBatch* m_SpriteBatch;
        std::string m_Text;
        std::vector<float> m_LineWidth;
        std::vector<CharacterData> m_CharacterData;
        Vector2 m_Size;
        Justification m_Justification;
        float m_CharacterSpacing;
    };
}

#endif
