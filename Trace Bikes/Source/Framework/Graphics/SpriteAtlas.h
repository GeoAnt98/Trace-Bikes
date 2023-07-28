#ifndef __GameDev2D__SpriteAtlas__
#define __GameDev2D__SpriteAtlas__

#include "Sprite.h"
#include <map>
#include <string>


namespace GameDev2D
{
    class SpriteAtlas : public Sprite
    {
    public: 
        //Creates a SpriteAtlas for the given filename, all the frames in the atlas will 
        //be stored and can be set using the UseFrame method(). Make sure to load the Texture 
        //that the atlas uses ahead of time
        SpriteAtlas(const std::string& filename);
        ~SpriteAtlas();

        //Sets the specific frame for the atlasKey
        void UseFrame(const std::string& atlasKey);

        //Unpacks the Atlas .json file
        static bool Unpack(const std::string& path, std::map<std::string, Rect>* atlasData);

    private:
        //Member variables
        std::map<std::string, Rect> m_AtlasMap;
        std::string m_Filename;
    };
}

#endif