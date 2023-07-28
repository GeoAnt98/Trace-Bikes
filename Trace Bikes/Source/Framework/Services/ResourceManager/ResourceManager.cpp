#include "ResourceManager.h"
#include "../../Audio/Audio.h"
#include "../../Debug/Log.h"
#include "../../Graphics/Font.h"
#include "../../Graphics/Shader.h"
#include "../../Graphics/SpriteAtlas.h"
#include "../../IO/File.h"
#include "../../Utils/Png/Png.h"
#include "../../Utils/TrueType/TrueType.h"
#include "../../Windows/Application.h"
#include "../Services.h"
#include <fmod.hpp>

namespace GameDev2D
{
    //Callback method for initializing the default Audio data
    FMOD_RESULT F_CALLBACK DefaultAudioCallback(FMOD_SOUND* /*sound*/, void* data, unsigned int dataLength);

    ResourceManager::ResourceManager() : EventHandler(),
        m_AudioSystem(nullptr),
        m_DefaultTexture(nullptr),
        m_DefaultFont(nullptr),
        m_DefaultAudio(nullptr)
    {
        //Create the FMOD system
        if (FMOD::System_Create(&m_AudioSystem) != FMOD_OK)
        {
            Log::Error(this, "Resource Manager", true, Log::Verbosity_Debug, "Failed to create FMOD instance");
        }

        //Initialize the audio channels for the FMOD system
        if (m_AudioSystem->init(MAX_AUDIO_CHANNELS, FMOD_INIT_NORMAL, NULL) != FMOD_OK)
        {
            Log::Error(this, "Resource Manager", true, Log::Verbosity_Debug, "Failed to initialize FMOD");
        }

        //Initialize the attributes for the polygon shader
        vector<string> attributes;
        attributes.push_back("in_vertices");
        attributes.push_back("in_color");

        //Initialize the uniforms for the polygon shader
        vector<string> uniforms;
        uniforms.push_back("uniform_anchor");
        uniforms.push_back("uniform_size");
        uniforms.push_back("uniform_model");
        uniforms.push_back("uniform_view");
        uniforms.push_back("uniform_projection");

        //Load the pass through shader
        ShaderInfo passthrough = ShaderInfo("passThrough", "passThrough", attributes, uniforms);
        LoadShader(&passthrough, PASSTHROUGH_POLYGON_SHADER_KEY);

        //Re-initialize the attributes for the Texture shader
        attributes.clear();
        attributes.push_back("in_vertices");
        attributes.push_back("in_uv");

        //Re-initialize the uniforms for the Texture shader
        uniforms.clear();
        uniforms.push_back("uniform_anchor");
        uniforms.push_back("uniform_size");
        uniforms.push_back("uniform_color");
        uniforms.push_back("uniform_texture");
        uniforms.push_back("uniform_model");
        uniforms.push_back("uniform_view");
        uniforms.push_back("uniform_projection");

        //Load the texture pass through shader
        ShaderInfo texturePassthrough = ShaderInfo("passThrough-texure", "passThrough-texure", attributes, uniforms);
        LoadShader(&texturePassthrough, PASSTHROUGH_TEXTURE_SHADER_KEY);

        //Re-initialize the attributes for the SpriteBatch shader
        attributes.clear();
        attributes.push_back("in_vertices");
        attributes.push_back("in_uv");
        attributes.push_back("in_color");

        //Re-initialize the uniforms for the SpriteBatch shader
        uniforms.clear();
        uniforms.push_back("uniform_texture");
        uniforms.push_back("uniform_model");
        uniforms.push_back("uniform_view");
        uniforms.push_back("uniform_projection");

        //Load the texture pass through shader
        ShaderInfo spriteBatchPassthrough = ShaderInfo("passThrough-spriteBatch", "passThrough-spriteBatch", attributes, uniforms);
        LoadShader(&spriteBatchPassthrough, PASSTHROUGH_SPRITEBATCH_SHADER_KEY);

        //Add an event listener callback for the Update event
        Services::GetApplication()->AddEventListener(this, UPDATE_EVENT);
    }

    ResourceManager::~ResourceManager()
    {
        //Release the FMOD system
        if (m_AudioSystem != nullptr)
        {
            m_AudioSystem->release();
            m_AudioSystem = nullptr;
        }

        //Delete the default Texture
        if (m_DefaultTexture != nullptr)
        {
            delete m_DefaultTexture;
            m_DefaultTexture = nullptr;
        }

        //Delete the default Font
        if (m_DefaultFont != nullptr)
        {
            delete m_DefaultFont;
            m_DefaultFont = nullptr;
        }

        //Delete the default Audio
        if (m_DefaultAudio != nullptr)
        {
            delete m_DefaultAudio;
            m_DefaultAudio = nullptr;
        }

        //Unload the default Shaders
        UnloadShader(PASSTHROUGH_POLYGON_SHADER_KEY);
        UnloadShader(PASSTHROUGH_TEXTURE_SHADER_KEY);
        UnloadShader(PASSTHROUGH_SPRITEBATCH_SHADER_KEY);

        //Check how many Textures remain loaded
        unsigned int texturesLeft = m_TextureMap.Count();
        if (texturesLeft > 0)
        {
            //Log that there are textures left
            Log::Error(this, "ResourceManager", false, Log::Verbosity_Debug, "%u Textures remain unloaded", texturesLeft);

            //Then cleanup the left over textures
            m_TextureMap.Cleanup();
        }

        //Check how many Fonts remain loaded
        unsigned int fontsLeft = m_FontMap.Count();
        if (fontsLeft > 0)
        {
            //Log that there are fonts left
            Log::Error(this, "ResourceManager", false, Log::Verbosity_Debug, "%u Fonts remain unloaded", fontsLeft);

            //Then cleanup the left over fonts
            m_FontMap.Cleanup();
        }

        //Check how many Sounds remain loaded
        unsigned int soundsLeft = m_AudioMap.Count();
        if (soundsLeft > 0)
        {
            //Log that there are sounds left
            Log::Error(this, "ResourceManager", false, Log::Verbosity_Debug, "%u Audio files remain unloaded", soundsLeft);

            //Then cleanup the left over sounds
            m_AudioMap.Cleanup();
        }

        //Remove the event listener callback for the Update event
        Services::GetApplication()->RemoveEventListener(this, UPDATE_EVENT);
    }

    void ResourceManager::HandleEvent(Event* aEvent)
    {
        if (aEvent != nullptr)
        {
            if (aEvent->GetEventCode() == UPDATE_EVENT)
            {
                //Update the FMOD system
                if (m_AudioSystem != nullptr)
                {
                    m_AudioSystem->update();
                }
            }
        }
    }

    void ResourceManager::LoadAudio(const std::string& aFilename, const std::string& aExtension)
    {
        if (IsAudioLoaded(aFilename, aExtension) == false)
        {
            //Safety check the filename
            if (aFilename.length() == 0)
            {
                Log::Error(this, "Resource Manager", false, Log::Verbosity_Debug, "Failed to load audio, the filename had a length of 0");
                return;
            }

            //Get the path for the audio file
            string path = Services::GetApplication()->GetPathForResourceInDirectory(aFilename.c_str(), aExtension.c_str(), "Audio");

            //Does the file exist, if it doesn't the assert below will be hit
            bool doesExist = Services::GetApplication()->DoesFileExistAtPath(path);
            assert(doesExist == true);

            //If the file doesn't exist, log an error
            if (doesExist == false)
            {
                Log::Error(this, "Resource Manager", false, Log::Verbosity_Debug, "Failed to load audio with filename: %s.%s, it doesn't exist", aFilename.c_str(), aExtension.c_str());
                return;
            }

            //Set the mode for the sound
            FMOD::Sound* sound = nullptr;
            FMOD_MODE mode = FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE;

            //Create the sound, if it fails log an error
            if (m_AudioSystem->createSound(path.c_str(), mode, 0, &sound) != FMOD_OK)
            {
                Log::Error(this, "Resource Manager", false, Log::Verbosity_Debug, "Failed to load audio with filename : %s.%s, an error occured during creation", aFilename.c_str(), aExtension.c_str());
                return;
            }

            //Create the audio data and add it to the resource map
            std::string key = std::string(aFilename + aExtension);
            m_AudioMap.Create(key, new Audio(m_AudioSystem, sound));
        }
    }

    void ResourceManager::UnloadAudio(const std::string& aFilename, const std::string& aExtension)
    {
        //Is the audio data loaded?
        if (IsAudioLoaded(aFilename, aExtension) == true)
        {
            //Get the audio resource
            std::string key = std::string(aFilename + aExtension);
            Audio* resource = m_AudioMap.Get(key);

            //Delete the resource
            if (resource != nullptr)
            {
                delete resource;
                resource = nullptr;
            }

            //Remove the resource from the Font map
            m_AudioMap.Remove(key);
        }
    }

    bool ResourceManager::IsAudioLoaded(const std::string& aFilename, const std::string& aExtension)
    {
        std::string key = std::string(aFilename + aExtension);
        return m_AudioMap.Contains(key);
    }

    Audio* ResourceManager::GetAudio(const std::string& aFilename, const std::string& aExtension)
    {
        //Is the audio data loaded?
        if (IsAudioLoaded(aFilename, aExtension) == true)
        {
            std::string key = std::string(aFilename + aExtension);
            Audio* audio = m_AudioMap.Get(key);
            return audio;
        }
        return GetDefaultAudio();
    }
    
    void ResourceManager::LoadFont(const std::string& aFilename, const std::string& aExtension, unsigned int aSize, const string& aCharacterSet)
    {
        //Check if the font loaded
        if (IsFontLoaded(aFilename, aExtension, aSize) == false)
        {
            //Safety check the filename
            if (aFilename.length() == 0)
            {
                Log::Error(this, "Resource Manager", false, Log::Verbosity_Debug, "Failed to load font, the filename had a length of 0");
                return;
            }

            //Rasterize the font atlas
            Font* font = nullptr;
            TrueType::Rasterize(aFilename, aExtension, aSize, aCharacterSet, &font);

            //Was the font atlas created successfully
            if (font != nullptr)
            {
                std::string key = std::string(aFilename + aExtension + std::to_string(aSize));
                m_FontMap.Create(key, font);
            }
            else
            {
                Log::Error(this, "Resource Manager", false, Log::Verbosity_Debug, "Failed to create font: %s.%s", aFilename.c_str(), aExtension.c_str());
            }
        }
    }

    void ResourceManager::UnloadFont(const std::string& aFilename, const std::string& aExtension, unsigned int aSize)
    {
        //Is the Font loaded?
        if (IsFontLoaded(aFilename, aExtension, aSize) == true)
        {
            //Get the resource
            std::string key = std::string(aFilename + aExtension + std::to_string(aSize));
            
            //Delete the resource
            Font* resource = m_FontMap.Get(key);
            if (resource != nullptr)
            {
                delete resource;
                resource = nullptr;
            }

            //Remove the resource from the Font map
            m_FontMap.Remove(key);
        }
    }

    bool ResourceManager::IsFontLoaded(const std::string& aFilename, const std::string& aExtension, unsigned int aSize)
    {
        std::string key = std::string(aFilename + aExtension + std::to_string(aSize));
        return m_FontMap.Contains(key);
    }

    Font* ResourceManager::GetFont(const std::string& aFilename, const std::string& aExtension, unsigned int aSize)
    {
        //Check to see if the font is even loaded
        if (IsFontLoaded(aFilename, aExtension, aSize) == true)
        {
            std::string key = std::string(aFilename + aExtension + std::to_string(aSize));
            return m_FontMap.Get(key);
        }

        return GetDefaultFont();
    }

    void ResourceManager::LoadShader(ShaderInfo* aShaderInfo, const string& aKey)
    {
        //Is the Shader loaded?
        if (IsShaderLoaded(aKey) == false)
        {
            //Safety check the filenames
            if (aShaderInfo->vertex.length() == 0 || aShaderInfo->fragment.length() == 0)
            {
                if (aShaderInfo->vertex.length() == 0)
                {
                    Log::Error(this, "Resource Manager", false, Log::Verbosity_Debug, "Failed to load the Shader %s, the .vsh shader file had a length of 0", aKey.c_str());
                }

                if (aShaderInfo->fragment.length() == 0)
                {
                    Log::Error(this, "Resource Manager", false, Log::Verbosity_Debug, "Failed to load the Shader %s, the .fsh shader file had a length of 0", aKey.c_str());
                }
                return;
            }

            //Was .vsh appended to the filename? If it was, remove it
            string vertexShader = string(aShaderInfo->vertex);
            size_t found = vertexShader.find(".vsh");
            if (found != std::string::npos)
            {
                vertexShader.erase(found, 4);
            }

            //Get the path for the vertex shader file
            string vertexPath = Services::GetApplication()->GetPathForResourceInDirectory(vertexShader.c_str(), "vsh", "Shaders");

            //Load the vertex shader
            File vertexShaderFile(vertexPath);

            //Safety check the vertex shader
            if (vertexShaderFile.GetBufferSize() == 0)
            {
                Log::Error(this, "Resource Manager", false, Log::Verbosity_Debug, "Failed to load the vertex shader %s", aKey.c_str());
                return;
            }

            //Was .fsh appended to the filename? If it was, remove it
            string fragmentShader = string(aShaderInfo->fragment);
            found = fragmentShader.find(".fsh");
            if (found != std::string::npos)
            {
                fragmentShader.erase(found, 4);
            }

            //Get the path for the fragment shader file
            string fragmentPath = Services::GetApplication()->GetPathForResourceInDirectory(fragmentShader.c_str(), "fsh", "Shaders");

            //Load the fragment shader
            File fragmentShaderFile(fragmentPath);

            //Safety check the fragment shader
            if (fragmentShaderFile.GetBufferSize() == 0)
            {
                Log::Error(this, "Resource Manager", false, Log::Verbosity_Debug, "Failed to load the fragment shader %s", aKey.c_str());
                return;
            }

            //Declare the shader pointer
            Shader* shader = nullptr;

            //Is there a geometry shader?
            if (aShaderInfo->geometry.length() > 0)
            {
                //Was .gsh appended to the filename? If it was, remove it
                string geometryShader = string(aShaderInfo->geometry);
                found = geometryShader.find(".gsh");
                if (found != std::string::npos)
                {
                    geometryShader.erase(found, 4);
                }

                //Get the path for the geometry shader file
                string geometryPath = Services::GetApplication()->GetPathForResourceInDirectory(geometryShader.c_str(), "gsh", "Shaders");

                //Load the geometry shader
                File geometryShaderFile(geometryPath);

                //Safety check the geometry shader
                if (geometryShaderFile.GetBufferSize() == 0)
                {
                    Log::Error(this, "Resource Manager", false, Log::Verbosity_Debug, "Failed to load the geometry shader %s", aKey.c_str());
                    return;
                }

                //Create a new shader with the vertex and fragment shaders
                ShaderSource shaderSource = ShaderSource(vertexShaderFile.GetBuffer(), fragmentShaderFile.GetBuffer(), geometryShaderFile.GetBuffer());
                shader = new Shader(shaderSource);
            }
            else
            {
                //Create a new shader with the vertex and fragment shaders
                ShaderSource shaderSource = ShaderSource(vertexShaderFile.GetBuffer(), fragmentShaderFile.GetBuffer());
                shader = new Shader(shaderSource);
            }

            //The shader SHOULDN'T be null
            assert(shader != nullptr);

            //Set the shader key
            shader->SetKey(aKey);

            //Set the shader map pair for the filename key
            m_ShaderMap.Create(aKey, shader);

            //Cycle through the attributes and add them to the shader
            for (unsigned int i = 0; i < aShaderInfo->attributes.size(); i++)
            {
                shader->AddAttribute(aShaderInfo->attributes.at(i));
            }

            //Cycle through the uniforms and add them to the shader
            for (unsigned int i = 0; i < aShaderInfo->uniforms.size(); i++)
            {
                shader->AddUniform(aShaderInfo->uniforms.at(i));
            }
        }
    }

    void ResourceManager::UnloadShader(const string& aKey)
    {
        //Is the Shader loaded?
        if (IsShaderLoaded(aKey) == true)
        {
            //Get the Shader
            Shader* resource = m_ShaderMap.Get(aKey);

            //Delete the Shader
            if (resource != nullptr)
            {
                delete resource;
                resource = nullptr;
            }

            //Remove the resource from the Shader map
            m_ShaderMap.Remove(aKey);
        }
    }

    bool ResourceManager::IsShaderLoaded(const string& aKey)
    {
        return m_ShaderMap.Contains(aKey);
    }

    Shader* ResourceManager::GetShader(const string& aKey)
    {
        if (m_ShaderMap.Contains(aKey) == true)
        {
            return m_ShaderMap.Get(aKey);
        }

        return m_ShaderMap.Get(PASSTHROUGH_TEXTURE_SHADER_KEY);
    }

    void ResourceManager::LoadTexture(const string& aFilename)
    {
        //Is the Texture loaded?
        if (IsTextureLoaded(aFilename) == false)
        {
            //Safety check the filename
            if (aFilename.length() == 0)
            {
                Log::Error(this, "Resource Manager", false, Log::Verbosity_Debug, "Failed to load texture, the filename had a length of 0");
                return;
            }

            //Append the filename to the directory
            string filename = string(aFilename);

            //Was .png appended to the filename? If it was, remove it
            size_t found = filename.find(".png");
            if (found != std::string::npos)
            {
                filename.erase(found, 4);
            }

            //Get the path for the texture
            string path = Services::GetApplication()->GetPathForResourceInDirectory(filename.c_str(), "png", "Images");

            //Does the image exist at the path
            bool doesExist = Services::GetApplication()->DoesFileExistAtPath(path);

            //If the path isn't empty, set the place holder texture
            if (doesExist == true)
            {
                //Attempt to load the png image and store its texture data in the TextureData struct
                ImageData* imageData = nullptr;
                bool success = Png::LoadFromPath(path, &imageData);

                //Did the image load successfully
                if (success == true && imageData != nullptr)
                {
                    //Create a new texture object
                    Texture* texture = new Texture(*imageData);

                    //Create the entry for the textureData
                    m_TextureMap.Create(aFilename, texture);
                }
                else
                {
                    //Log an error message
                    Log::Error(this, "Resource Manager", false, Log::Verbosity_Debug, "Failed to load texture : %s", aFilename.c_str());
                }

                //Delete the imageData, we don't need it anymore
                if (imageData != nullptr)
                {
                    delete imageData;
                    imageData = nullptr;
                }
            }
        }
    }

    void ResourceManager::UnloadTexture(const string& aFilename)
    {
        //Is the Texture loaded
        if (IsTextureLoaded(aFilename) == true)
        {
            //Get the resource
            Texture* resource = m_TextureMap.Get(aFilename);

            //Delete the resource
            if (resource != nullptr)
            {
                delete resource;
                resource = nullptr;
            }

            //Remove the resource from the Texture map
            m_TextureMap.Remove(aFilename);
        }
    }

    bool ResourceManager::IsTextureLoaded(const string& aFilename)
    {
        return m_TextureMap.Contains(aFilename);
    }

    Texture* ResourceManager::GetTexture(const string& aFilename)
    {
        //Set the texture data
        if (IsTextureLoaded(aFilename) == true)
        {
            return m_TextureMap.Get(aFilename);
        }

        //If the texture still isn't loaded, it doesn't exist, set 
        //the default texture data instead
        return GetDefaultTexture();
    }

    void ResourceManager::LoadAtlas(const string& aFilename)
    {
        if (IsAtlasLoaded(aFilename) == false)
        {
            //Get the json path
            string jsonPath = Services::GetApplication()->GetPathForResourceInDirectory(aFilename.c_str(), "json", "Images");

            //Unpack the sprite atlas
            map<string, Rect> frames;
            SpriteAtlas::Unpack(jsonPath, &frames);

            //Add the unpacked sprite atlas data to the ResourceMap
            m_AtlasMap.Create(aFilename, frames);
        }
    }

    void ResourceManager::UnloadAtlas(const string& aFilename)
    {
        if (IsAtlasLoaded(aFilename) == true)
        {
            m_AtlasMap.Remove(aFilename);
        }
    }

    bool ResourceManager::IsAtlasLoaded(const string& aFilename)
    {
        return m_AtlasMap.Contains(aFilename);
    }

    Rect ResourceManager::GetAtlasFrame(const string& aFilename, const string& aAtlasKey)
    {
        //Ensure this time that the atlas data has been loaded
        if (IsAtlasLoaded(aFilename) == true)
        {
            //Get the specific TextureFrame from the atlas ResourceMap
            map<string, Rect> atlasFrames = m_AtlasMap.Get(aFilename);
            if (atlasFrames.find(aAtlasKey) != atlasFrames.end())
            {
                //Set the texture frame
                return atlasFrames[aAtlasKey];
            }
        }

        return Rect();
    }

    Texture* ResourceManager::GetDefaultTexture()
    {
        if (m_DefaultTexture == nullptr)
        {
            //Local constants
            const int placeHolderSize = 128;  //Width and Height
            const int colorSize = 4;  //RGBA

            //Local variables used below
            int checkerSize = placeHolderSize >> 1;
            int rowBytes = placeHolderSize * colorSize;
            Color colorA(0x555555c8);
            Color colorB(0xbbbbbbc8);

            //Create our image buffer
            unsigned int dataSize = colorSize * placeHolderSize * placeHolderSize;
            unsigned char* data = new unsigned char[dataSize];

            //Cycle through and set the checkered board pattern
            int index = 0;
            for (int i = 0; i < checkerSize; ++i)
            {
                for (int j = 0; j < checkerSize; ++j)
                {
                    //Red
                    data[index] = data[index + checkerSize * (colorSize + rowBytes)] = (unsigned char)(colorA.r * 255.0f);
                    data[index + checkerSize * colorSize] = data[index + checkerSize * rowBytes] = (unsigned char)(colorB.r * 255.0f);
                    index++;

                    //Green
                    data[index] = data[index + checkerSize * (colorSize + rowBytes)] = (unsigned char)(colorA.g * 255.0f);
                    data[index + checkerSize * colorSize] = data[index + checkerSize * rowBytes] = (unsigned char)(colorB.g * 255.0f);
                    index++;

                    //Blue
                    data[index] = data[index + checkerSize * (colorSize + rowBytes)] = (unsigned char)(colorA.b * 255.0f);
                    data[index + checkerSize * colorSize] = data[index + checkerSize * rowBytes] = (unsigned char)(colorB.b * 255.0f);
                    index++;

                    //Alpha
                    data[index] = data[index + checkerSize * (colorSize + rowBytes)] = (unsigned char)(colorA.a * 255.0f);
                    data[index + checkerSize * colorSize] = data[index + checkerSize * rowBytes] = (unsigned char)(colorB.a * 255.0f);
                    index++;
                }
                index += checkerSize * colorSize;
            }

            //Create the texture object
            PixelFormat format = PixelFormat(PixelFormat::RGBA, PixelFormat::UnsignedByte);
            ImageData imageData = ImageData(format, placeHolderSize, placeHolderSize, data);
            m_DefaultTexture = new Texture(imageData);

            //Delete the data buffer, we are done with it
            if (data != nullptr)
            {
                delete[] data;
                data = nullptr;
            }
        }

        //Return the default texture
        return m_DefaultTexture;
    }

    Font* ResourceManager::GetDefaultFont()
    {
        if (m_DefaultFont == nullptr)
        {
            Font* font = nullptr;
            TrueType::Rasterize("OpenSans-CondBold", "ttf", 32, CHARACTER_SET_ALPHA_NUMERIC_EXT, &m_DefaultFont);
        }

        return m_DefaultFont;
    }

    Audio* ResourceManager::GetDefaultAudio()
    {
        //If the default audio file isn't created, create it, then return it
        if (m_DefaultAudio == nullptr)
        {
            //Local variables
            int channels = 1;       //Mono
            int duration = 2;       //In seconds;
            int frequency = 8192;

            //Initialize the mode for the sound
            FMOD::Sound* sound = nullptr;
            FMOD_MODE mode = FMOD_LOOP_OFF | FMOD_2D | FMOD_OPENUSER;

            //Initialize the sound data information
            FMOD_CREATESOUNDEXINFO exinfo;
            memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
            exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);  
            exinfo.numchannels = channels;                         
            exinfo.defaultfrequency = frequency;                   
            exinfo.decodebuffersize = frequency;                       
            exinfo.length = exinfo.defaultfrequency * exinfo.numchannels * sizeof(signed short) * duration;
            exinfo.format = FMOD_SOUND_FORMAT_PCM16;         
            exinfo.pcmreadcallback = DefaultAudioCallback;

            //Create the sound, if it fails log an error
            if (m_AudioSystem->createSound(0, mode, &exinfo, &sound) != FMOD_OK)
            {
                Log::Error(this, "Resource Manager", false, Log::Verbosity_Debug, "Failed to load the default Audio object");
            }
            else
            {
                //Create the Default audio file
                m_DefaultAudio = new Audio(m_AudioSystem, sound);
            }
        }

        return m_DefaultAudio;
    }

    FMOD_RESULT F_CALLBACK DefaultAudioCallback(FMOD_SOUND* /*sound*/, void* data, unsigned int dataLength)
    {
        int frequency = 650;
        int sampleRate = 8192;
        unsigned int frames = dataLength / 2; //16bit mono (2 bytes per sample)

        for (unsigned int i = 0; i < frames; i++)
        {
            ((short *)data)[i] = (short)(SHRT_MAX * sin((2 * M_PI * frequency) / sampleRate * i));
        }

        return FMOD_OK;
    }
}