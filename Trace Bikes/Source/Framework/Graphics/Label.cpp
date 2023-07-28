#include "Label.h"
#include "Font.h"
#include "SpriteBatch.h"
#include "../Utils/Text/Text.h"
#include "../Services/Services.h"
#include "../Services/Graphics/Graphics.h"
#include <GameDev2D.h>


namespace GameDev2D
{
    Label::Label(Font* aFont) : Drawable(),
        m_Font(aFont),
        m_Justification(JustifyLeft),
        m_Text(""),
        m_Size(Vector2(0.0f, 0.0f)),
        m_CharacterSpacing(0.0f)
    {
        //Create the SpriteBatch
        m_SpriteBatch = new SpriteBatch();
    }

    Label::~Label()
    {
        //Delete the SpriteBatch
        if (m_SpriteBatch != nullptr)
        {
            delete m_SpriteBatch;
            m_SpriteBatch = nullptr;
        }
    }

    void Label::Draw()
    {
        //Is there any text to render?
        if (m_Text.length() == 0)
        {
            return;
        }

        //calculate the baseline and origin for the label
        unsigned int baseline = m_Font->GetLineHeight() - m_Font->GetBaseLine();
        unsigned int numberOfLines = Text::NumberOfLines(m_Text);
        Vector2 origin = Vector2(0.0f, 0.0f);
        Vector2 position = Vector2(0.0f, 0.0f);
        position.y += baseline + (m_Font->GetLineHeight() * (numberOfLines - 1));
        int lineIndex = 0;

        //What justification are we dealing with
        if (m_Justification == JustifyLeft)
        {
            position.x = 0.0f;
        }
        else if (m_Justification == JustifyCenter)
        {
            position.x = (GetWidth() - m_LineWidth.at(lineIndex)) * 0.5f;
        }
        else if (m_Justification == JustifyRight)
        {
            position.x = GetWidth() - m_LineWidth.at(lineIndex);
        }

        //Begin the SpriteBatch
        m_SpriteBatch->Begin();

        //Cycle through the characters in the text label
        for (unsigned int i = 0; i < m_Text.length(); i++)
        {
            //Did we reach a new line?
            if (m_Text.at(i) == '\n')
            {
                //Increment the line index
                lineIndex++;

                //Calculate the line's origin based on the justification
                if (m_Justification == JustifyLeft)
                {
                    position.x = 0.0f;
                }
                else if (m_Justification == JustifyCenter)
                {
                    position.x = (GetWidth() - m_LineWidth.at(lineIndex)) * 0.5f;
                }
                else if (m_Justification == JustifyRight)
                {
                    position.x = GetWidth() - m_LineWidth.at(lineIndex);
                }

                //Set the y line origin based on the line height of the font
                position.y -= m_Font->GetLineHeight();
                continue;
            }
            //Calculate the character position based on the x and y bearing
            Rect characterRect = m_Font->GetTextureRectForCharacter(m_Text.at(i));
            Vector2 anchorOffset = Vector2(-GetWidth() * GetAnchor().x, -GetHeight() * GetAnchor().y);
            Vector2 charPosition = position + anchorOffset;
            charPosition.x += m_Font->GetBearingXForCharacter(m_Text.at(i));
            charPosition.y += m_Font->GetBearingYForCharacter(m_Text.at(i)) - characterRect.size.y;

            //Calculate the Matrix
            Matrix parent = GetTransformMatrix();
            Matrix matrix = Matrix::Make(charPosition, m_CharacterData.at(i).angle, m_CharacterData.at(i).scale);

            //Draw each character
            m_SpriteBatch->Draw(m_Font->GetTexture(), parent * matrix, m_CharacterData.at(i).color, m_CharacterData.at(i).anchor, characterRect);

            //Increment the position
            position.x += m_Font->GetAdvanceXForCharacter(m_Text.at(i)) + GetCharacterSpacing();
        }

        //End the SpriteBatch
        m_SpriteBatch->End();
    }

    float Label::GetWidth()
    {
        return m_Size.x;
    }

    float Label::GetHeight()
    {
        return m_Size.y;
    }

    void Label::SetText(const std::string& aText)
    {
        //Is the text the exact same, if so return
        if (aText != m_Text)
        {
            //Set the text
            m_Text = std::string(aText);

            //Clear the character data
            m_CharacterData.clear();

            //Add a CharacterData object for each letter
            for (unsigned int i = 0; i < m_Text.length(); i++)
            {
                m_CharacterData.push_back(CharacterData(GetColor()));
            }

            //Re-calculate the size
            CalculateSize();
        }
    }

    std::string Label::GetText()
    {
        return m_Text;
    }

    void Label::SetCharacterSpacing(float aCharacterSpacing)
    {
        if (m_CharacterSpacing != aCharacterSpacing)
        {
            //Set the character spacing
            m_CharacterSpacing = aCharacterSpacing;

            //Re-calculate the size
            CalculateSize();
        }
    }

    float Label::GetCharacterSpacing()
    {
        return m_CharacterSpacing;
    }

    void Label::SetJustification(Label::Justification aJustification)
    {
        //Safety check that the justification isn't already set
        if (aJustification != m_Justification)
        {
            //Set the justification
            m_Justification = aJustification;
        }
    }

    Label::Justification Label::GetJustification()
    {
        return m_Justification;
    }

    void Label::CalculateSize()
    {
        //Safety check that the font pointer is null
        if (m_Font == nullptr)
        {
            return;
        }

        //Used to track the max width and height
        float x = 0.0f;
        Vector2 maxSize(0.0f, (float)m_Font->GetLineHeight());

        //Clear the line width's vector
        m_LineWidth.clear();

        //Cycle through all the characters in the text string
        for (unsigned int i = 0; i < m_Text.length(); i++)
        {
            //Did we reach a new line?
            if (m_Text.at(i) == '\n')
            {
                //Add the line width to the line widths vector
                m_LineWidth.push_back(x);

                //Calculate the max width and increment the max height
                maxSize.x = fmaxf(x, maxSize.x);
                maxSize.y += m_Font->GetLineHeight();

                //Set x back to zero
                x = 0.0f;
                continue;
            }

            //Advance the x, by the x-advance of the character
            x += m_Font->GetAdvanceXForCharacter(m_Text.at(i)) + GetCharacterSpacing();
        }

        //We are done, add the line width to the line widths vector
        m_LineWidth.push_back(x);

        //Calculate the max width
        maxSize.x = fmaxf(x, maxSize.x);

        //Set the size
        m_Size = maxSize;
    }

    std::vector<Label::CharacterData>& Label::GetCharacterData()
    {
        return m_CharacterData;
    }
}