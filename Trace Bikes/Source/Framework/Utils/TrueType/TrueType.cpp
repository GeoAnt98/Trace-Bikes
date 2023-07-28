#include "TrueType.h"
#include "../../Debug/Log.h"
#include "../../Debug/Profile.h"
#include "../../Graphics/Font.h"
#include "../../Graphics/RenderTarget.h"
#include "../../Graphics/Texture.h"
#include "../../Services/Services.h"
#include "../../Windows/Application.h"
#include <ft2build.h>       
#include FT_FREETYPE_H


namespace GameDev2D
{
    void TrueType::Rasterize(const std::string& aFilename, const std::string& aExtension, unsigned int aSize, const std::string& aCharacterSet, Font** aFont)
    {
        //The FreeType library used to load .ttf and .otf fonts
        FT_Library freeType;

        //Initialize the FreeType lib, returns zero if successful
        if (FT_Init_FreeType(&freeType) != 0)
        {
            //The FreeType lib failed to initialize
            Log::Error(nullptr, "TrueType", false, Log::Verbosity_Debug, "The FreeType lib failed to initialize");
            return;
        }

        //Get the path
        string path = Services::GetApplication()->GetPathForResourceInDirectory(aFilename.c_str(), aExtension.c_str(), "Fonts");

        //Verify the font exists, if it doesn't the assert below will be hit
        bool doesExist = Services::GetApplication()->DoesFileExistAtPath(path);
        if (doesExist == false)
        {
            Log::Error(nullptr, "TrueType", false, Log::Verbosity_Debug, "Failed to font find file: %s.%s, the file doesn't exist", aFilename.c_str(), aExtension.c_str());
            return;
        }

        //Generate a font face for the desired ttf font
        FT_Face face;
        if (FT_New_Face(freeType, path.c_str(), 0, &face) != 0)
        {
            //Did the font face get successfully created? Log an error if it didn't
            Log::Error(nullptr, "TrueType", false, Log::Verbosity_Debug, "Failed to create a font face for font file: %s.%s", aFilename.c_str(), aExtension.c_str());
            return;
        }

        //Set the pixel size for the font
        FT_Set_Pixel_Sizes(face, 0, aSize);

        //Create the FontData object
        FontData fontData;
        fontData.name = aFilename;
        fontData.extension = aExtension;
        fontData.characterSet = aCharacterSet;
        fontData.size = aSize;

        //Local variables used below
        unsigned int cellWidth = 0;
        unsigned int cellHeight = 0;
        int maxBearing = 0;
        int minHang = 0;
        unsigned int lineHeight = 0;
        map<char, unsigned char*> glyphBuffer;

        //Cycle through all the characters in the character set
        for (unsigned int i = 0; i < fontData.characterSet.size(); i++)
        {
            //Get the character for the current index
            char character = fontData.characterSet.at(i);

            //Load the character
            if (FT_Load_Char(face, character, FT_LOAD_RENDER) != 0)
            {
                //Did the font load the character? Log an error if it didn't
                Log::Error(nullptr, "TrueType", false, Log::Verbosity_Debug, "Failed to load character: %c for font %s.%s", character, aFilename.c_str(), aExtension.c_str());

                //Continue loading characters
                continue;
            }

            //Get the glyph data for the character
            GlyphData glyphData = fontData.glyphData[character];

            //Calculate the buffer size needed for the glyph
            size_t size = face->glyph->bitmap.rows * face->glyph->bitmap.width;

            //Get the buffer
            unsigned char* buffer = glyphBuffer[character];
            if (buffer == nullptr)
            {
                buffer = new unsigned char[size];
            }

            //Cycle through and copy glyph data
            for (int j = 0; j < face->glyph->bitmap.rows; j++)
            {
                int offsetA = (face->glyph->bitmap.width * ((int)face->glyph->bitmap.rows - 1 - j));
                int offsetB = (int)(size - offsetA - face->glyph->bitmap.width);
                memcpy(buffer + offsetA, face->glyph->bitmap.buffer + offsetB, face->glyph->bitmap.width);
            }

            //Set the buffer
            glyphBuffer[character] = buffer;

            //Set the glyph data properties
            glyphData.width = face->glyph->bitmap.width;
            glyphData.height = face->glyph->bitmap.rows;
            glyphData.advanceX = (unsigned char)(face->glyph->advance.x / 64);
            glyphData.bearingX = face->glyph->bitmap_left;
            glyphData.bearingY = face->glyph->bitmap_top;

            //Calculate max bearing
            if (glyphData.bearingY > maxBearing)
            {
                maxBearing = glyphData.bearingY;
            }

            //Calculate max width
            if (glyphData.width > cellWidth)
            {
                cellWidth = glyphData.width;
            }

            //Calculate the line height
            if (glyphData.height > lineHeight)
            {
                lineHeight = glyphData.height;
            }

            //Calculate gylph hang
            int glyphHang = glyphData.bearingY - glyphData.height;
            if (glyphHang < minHang)
            {
                minHang = glyphHang;
            }

            //Set the glyph data
            fontData.glyphData[character] = glyphData;
        }

        //Create bitmap font
        cellHeight = maxBearing - minHang;

        //Set the baseline and line height
        fontData.baseLine = maxBearing;
        fontData.lineHeight = lineHeight;

        //Local variables used below
        unsigned int numberOfCells = (unsigned int)fontData.characterSet.size();
        unsigned int cellsPerRow = (unsigned int)ceil(sqrt(numberOfCells));
        unsigned int cellsPerColumn = cellsPerRow;

        //Local variables used below
        Rect rect(Vector2(0.0f, 0.0f), Vector2((float)cellWidth, (float)cellHeight));
        Shader* shader = Services::GetResourceManager()->GetShader(PASSTHROUGH_TEXTURE_SHADER_KEY);
        Texture texture;
        int offsetX = 0;
        int offsetY = 0;
        unsigned int rows = 0;
        unsigned int columns = 0;

        //Set the render target size
        unsigned int viewWidth = cellWidth * cellsPerRow;
        unsigned int viewHeight = cellHeight * cellsPerColumn;
        PixelFormat format = PixelFormat(PixelFormat::RGBA, PixelFormat::UnsignedByte);
        ImageData imageData = ImageData(format, viewWidth, viewHeight);
        Texture fontTexture(imageData);

        //Create the render target and create the frame buffer with the textureframe
        RenderTarget* renderTarget = new RenderTarget(&fontTexture);

        //Set the render target as the active render target
        renderTarget->Begin();

        //Cycle through all the characters in the character set and render the glyph data to the render target
        for (unsigned int i = 0; i < fontData.characterSet.size(); i++)
        {
            //Get the character for the index
            char character = fontData.characterSet.at(i);

            //Get the glyph data for the character
            GlyphData glyphData = fontData.glyphData[character];

            //
            if (glyphData.width == 0 && glyphData.height == 0)
            {
                continue;
            }

            //Set base offsets
            offsetX = cellWidth * columns;
            offsetY = cellHeight * rows;

            //set the sourcce frame for the glyph
            glyphData.frame.origin.x = (float)offsetX;
            glyphData.frame.origin.y = viewHeight - (float)offsetY - glyphData.height;
            glyphData.frame.size.x = glyphData.width;
            glyphData.frame.size.y = glyphData.height;

            //Set the glyph data
            fontData.glyphData[character] = glyphData;

            //Calculate the rgba data buffer size and then allocate it
            const size_t size = glyphData.width * glyphData.height;
            const int colorSize = 4;
            unsigned char* rgbaData = new unsigned char[size * colorSize];

            //Cycle through the glyph buffer data and set the rgba data and convert it to black and white
            int index = 0;
            for (unsigned int j = 0; j < size * colorSize; j += colorSize)
            {
                unsigned char* buffer = glyphBuffer[character];
                unsigned char value = buffer[index++];

                if (value > 0)
                {
                    rgbaData[j] = 255;
                    rgbaData[j + 1] = 255;
                    rgbaData[j + 2] = 255;
                    rgbaData[j + 3] = value;
                }
                else
                {
                    rgbaData[j] = 0;
                    rgbaData[j + 1] = 0;
                    rgbaData[j + 2] = 0;
                    rgbaData[j + 3] = 0;
                }
            }

            //We can now delete the glyph buffer data
            if (glyphBuffer[character] != nullptr)
            {
                delete[] glyphBuffer[character];
                glyphBuffer[character] = nullptr;
            }

            //Resize the Texture object with the rgba data
            PixelFormat format = PixelFormat(PixelFormat::RGBA, PixelFormat::UnsignedByte);
            ImageData imageData = ImageData(format, glyphData.width, glyphData.height, rgbaData);
            texture.SetData(imageData);

            //Delete the rgba buffer data
            if (rgbaData != nullptr)
            {
                delete[] rgbaData;
                rgbaData = nullptr;
            }

            //Then draw the rgba data
            Services::GetGraphics()->DrawTexture(&texture, Vector2((float)offsetX, (float)offsetY), 0.0f);

            //Increment the columns (and rows if necessary)
            columns++;
            if (columns == cellsPerColumn)
            {
                rows++;
                columns = 0;
            }
        }

        //Set the Graphic's original render target back
        renderTarget->End();

        //Delete the renderTarget, we are done with it
        if (renderTarget != nullptr)
        {
            delete renderTarget;
            renderTarget = nullptr;
        }

        //Free the FreeType library
        FT_Done_FreeType(freeType);

        //Create the Font object
        *aFont = new Font(fontData, fontTexture);
    }
}