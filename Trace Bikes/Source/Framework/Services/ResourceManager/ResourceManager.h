#ifndef __GameDev2D__ResourceManager__
#define __GameDev2D__ResourceManager__

#include "../../Events/EventHandler.h"
#include "../../Graphics/Font.h"
#include "../../Graphics/Sprite.h"
#include "../../Graphics/Texture.h"
#include "../../Graphics/GraphicTypes.h"
#include <map>
#include <string>


//Audio constants
const unsigned int MAX_AUDIO_CHANNELS = 32;

//Shader constants
const std::string PASSTHROUGH_POLYGON_SHADER_KEY = "PolygonPassthrough";
const std::string PASSTHROUGH_TEXTURE_SHADER_KEY = "TexturePassthrough";
const std::string PASSTHROUGH_SPRITEBATCH_SHADER_KEY = "SpriteBatchPassthrough";


//Font constants
const std::string CHARACTER_SET_NUMERIC = "0123456789";
const std::string CHARACTER_SET_ALPHA_LOWER = "abcdefghijklmnopqrstuvwxyz";
const std::string CHARACTER_SET_ALPHA_UPPER = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const std::string CHARACTER_SET_ALPHA_FULL = CHARACTER_SET_ALPHA_LOWER + CHARACTER_SET_ALPHA_UPPER;
const std::string CHARACTER_SET_ALPHA_NUMERIC = CHARACTER_SET_ALPHA_FULL + CHARACTER_SET_NUMERIC;
const std::string CHARACTER_SET_ALPHA_NUMERIC_EXT = CHARACTER_SET_ALPHA_NUMERIC + " ,.?!'\"_+-=*:;#/\\@$%&^<>{}[]()";


//Forward declarations
namespace FMOD
{
    class System;
    class Sound;
}

namespace GameDev2D
{
    //Templated class to make managing Resources cleaner
    template<typename T> class ResourceMap
    {
    public:
        void Create(const std::string& key, T type)
        {
            m_Map[key] = type;
        }

        void Remove(const std::string& key)
        {
            m_Map.erase(key);
        }

        T Get(const std::string& key)
        {
            return m_Map[key];
        }

        bool Contains(const std::string& key)
        {
            return m_Map.find(key) != m_Map.end();
        }

        unsigned int Count()
        {
            return m_Map.size();
        }

        void Cleanup()
        {
            for (std::map<string, T>::iterator iterator = m_Map.begin(); iterator != m_Map.end(); iterator++)
            {
                T resource = iterator->second;
                if (resource != nullptr)
                {
                    delete resource;
                    resource = nullptr;
                }
            }

            m_Map.clear();
        }

    private:
        std::map<string, T> m_Map;
    };


    //Forward declarations
    class Audio;
    class Shader;
    struct ShaderInfo;


    //The ResourceManager is responsible for loading, unloading and making accessible Audio, Font, Texture and Shader files
    class ResourceManager : public EventHandler
    {
    public:
        ResourceManager();
        ~ResourceManager();

        //The HandleEvent is used to notify the ResourceManager class of various GameDev2D events
        void HandleEvent(Event* event);

        //Loads an Audio object for the appropriate file, only load an Audio file once
        void LoadAudio(const std::string& filename, const std::string& extension);

        //Unloads an already loaded Audio object
        void UnloadAudio(const std::string& filename, const std::string& extension);

        //Returns wether an Audio object for the appropriate file is loaded or not
        bool IsAudioLoaded(const std::string& filename, const std::string& extension);

        //Returns an Audio objectfor the appropriate file, if the file doesn't exist 
        //OR isn't loaded yet then nullptr will be returned
        Audio* GetAudio(const std::string& filename, const std::string& extension);

        //Loads a Font for the appropriate file and font size, only load a Font once
        void LoadFont(const string& filename, const string& extension, unsigned int size, const string& characterSet = CHARACTER_SET_ALPHA_NUMERIC_EXT);

        //Unloads an already loaded Font
        void UnloadFont(const std::string& filename, const std::string& extension, unsigned int size);

        //Returns wether a Shader for the appropriate file is loaded or not
        bool IsFontLoaded(const std::string& filename, const std::string& extension, unsigned int size);

        //Returns a Font for the appropriate file and font size, if the file doesn't exist 
        //OR isn't loaded yet the default Font (OpenSans-CondBold.ttf) will be returned instead        
        Font* GetFont(const std::string& filename, const std::string& extension, unsigned int size);

        //Loads a Shader for the appropriate file, only load a Shader once, the default Shaders are loaded automatically
        void LoadShader(ShaderInfo* shaderInfo, const string& key);

        //Unloads an already loaded Shader
        void UnloadShader(const string& key);

        //Returns wether a Shader for the appropriate file is loaded or not
        bool IsShaderLoaded(const string& key);

        //Returns a Shader for the appropriate key, if the Shader doesn't exist 
        //OR isn't loaded yet, then nullptr will be returned
        Shader* GetShader(const string& key);

        //Loads a Texture for the appropriate file, only load a Texture once
        void LoadTexture(const string& filename);

        //Unloads an already loaded Texture
        void UnloadTexture(const string& filename);

        //Returns wether a Texture for the appropriate file is loaded or not
        bool IsTextureLoaded(const string& filename);

        //Returns a Texture for the appropriate file, if the file doesn't exist 
        //OR isn't loaded yet a checkerboard Texture will be returned instead
        Texture* GetTexture(const string& filename);

        //Load the SpriteAtlas frames for the appropriate file is loaded or not
        void LoadAtlas(const string& filename);

        //Unload the SpriteAtlas frames for the appropriate file is loaded or not
        void UnloadAtlas(const string& filename);

        //Returns wether the SpriteAtlas frames for the appropriate file is loaded or not
        bool IsAtlasLoaded(const string& filename);

        //Returns a Rect for the appropriate file and atlas key, if the frame doesn't exist 
        //OR isn't loaded yet an empty Rect value will be returned instead
        Rect GetAtlasFrame(const string& filename, const string& atlasKey);

        //Returns the placeholder checkerboard texture
        Texture* GetDefaultTexture();

        //Returns the default font (OpenSans-CondBold.ttf)
        Font* GetDefaultFont();

        //Returns a default Audio sound (1 second tone at frequency 650)
        Audio* GetDefaultAudio();

    private:
        //Member variables
        FMOD::System* m_AudioSystem;
        ResourceMap<Audio*> m_AudioMap;
        ResourceMap<Font*> m_FontMap;
        ResourceMap<Shader*> m_ShaderMap;
        ResourceMap<Texture*> m_TextureMap;
        ResourceMap<std::map<string, Rect>> m_AtlasMap;
        Texture* m_DefaultTexture;
        Font* m_DefaultFont;
        Audio* m_DefaultAudio;
    };
}

#endif