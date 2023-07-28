#include "SpriteAtlas.h"
#include "../Services/Services.h"
#include "../Windows/Application.h"
#include <json.h>
#include <fstream>
#include <assert.h>


namespace GameDev2D
{
    SpriteAtlas::SpriteAtlas(const std::string& aFilename) : Sprite(Services::GetResourceManager()->GetTexture(aFilename)),
        m_Filename(aFilename)
    {
        //Load the texture
        if (Services::GetResourceManager()->IsTextureLoaded(aFilename) == false)
        {
            Log::Error(this, "SpriteAtlas", false, Log::Verbosity_Graphics, "The SpriteAtlas: %s didn't have the Texture loaded ahead of time", aFilename.c_str());
        }

        //Load the atlas frames
        Services::GetResourceManager()->LoadAtlas(m_Filename);

        //Ensure the atlas frames were properly loaded
        assert(Services::GetResourceManager()->IsAtlasLoaded(m_Filename) == true);
    }

    SpriteAtlas::~SpriteAtlas()
    {
        Services::GetResourceManager()->UnloadAtlas(m_Filename);
    }

    void SpriteAtlas::UseFrame(const std::string& aAtlasKey)
    {
        //Get the atlas frame for the key
        Rect frame = Services::GetResourceManager()->GetAtlasFrame(m_Filename, aAtlasKey);
        
        //Set the Sprite's frame
        SetFrame(frame);
    }

    bool SpriteAtlas::Unpack(const std::string& aPath, std::map<std::string, Rect>* aAtlasData)
    {
        //Does the json file exist, if it doesn't the assert below will be hit
        bool doesExist = Services::GetApplication()->DoesFileExistAtPath(aPath);
        assert(doesExist == true);

        //If the json files exists, load the atlas frae
        if (doesExist == true)
        {
            //Open the input stream
            std::ifstream inputStream;
            inputStream.open(aPath.c_str(), std::ifstream::in);

            //Parse the json data
            Json::Value root;
            Json::Reader reader;
            if (reader.parse(inputStream, root, false) == true)
            {
                //Get the frames
                const Json::Value frames = root["frames"];

                //Cycle through the frames and parse the frame data
                for (unsigned int index = 0; index < frames.size(); ++index)
                {
                    //Safety check the filename
                    if (frames[index]["filename"].isString() == true)
                    {
                        //Get the atlas key
                        string atlasKey = string(frames[index]["filename"].asString());

                        //Load the source frame
                        Json::Value frame = frames[index]["frame"];

                        //Get the values from the frame
                        unsigned int x = frame["x"].asInt();
                        unsigned int y = frame["y"].asInt();
                        unsigned int width = frame["w"].asInt();
                        unsigned int height = frame["h"].asInt();

                        //Return the source frame
                        (*aAtlasData)[atlasKey] = Rect(Vector2((float)x, (float)y), Vector2((float)width, (float)height));
                    }
                }
            }

            //Close the input stream
            inputStream.close();

            //The unpack was successful
            return true;
        }

        //The unpack failed
        return false;
    }
}