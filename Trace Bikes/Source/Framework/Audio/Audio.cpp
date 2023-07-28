#include "Audio.h"
#include "../Services/Services.h"
#include "../Events/Event.h"


namespace GameDev2D
{
    FMOD_RESULT F_CALLBACK ChannelCallback(FMOD_CHANNEL* channel, FMOD_CHANNEL_CALLBACKTYPE type, void* commanddata1, void* commanddata2);


    Audio::Audio(FMOD::System* aSystem, FMOD::Sound* aSound) :
        m_System(aSystem),
        m_Sound(aSound),
        m_Channel(nullptr),
        m_Length(0),
        m_Frequency(0.0f),
        m_Volume(0.0f),
        m_Pan(0.0f),
        m_Priority(0),
        m_Position(0),
        m_DoesLoop(false)
    {
        //Get the length of the sound
        m_Sound->getLength(&m_Length, FMOD_TIMEUNIT_MS);

        //Initialize the audio defaults
        m_Sound->getDefaults(&m_Frequency, &m_Volume, &m_Pan, &m_Priority);
    }

    Audio::~Audio()
    {
        //release the audio data
        if (m_Sound != nullptr)
        {
            m_Sound->release();
            m_Sound = nullptr;
        }
    }

    void Audio::Play()
    {
        if (m_System != nullptr)
        {
            if (m_Channel == nullptr)
            {
                //Play the sound
                m_System->playSound(FMOD_CHANNEL_FREE, m_Sound, true, &m_Channel);

                //Set the channel's user data and call back function
                m_Channel->setUserData(this);
                m_Channel->setCallback(&ChannelCallback);

                //Set the channel's frequency, volume, pan, position and priority settings
                m_Channel->setFrequency(m_Frequency);
                m_Channel->setVolume(m_Volume);
                m_Channel->setPan(m_Pan);
                m_Channel->setPriority(m_Priority);
                m_Channel->setPosition(m_Position, FMOD_TIMEUNIT_MS);
            }

            //Safety check the channel
            if (m_Channel != nullptr)
            {
                //Play the audio
                m_Channel->setPaused(false);
            }
        }
    }

    void Audio::Pause()
    {
        //Safety check the channel isn't null
        if (m_Channel != nullptr)
        {
            //Pause the audio channel
            m_Channel->setPaused(true);
        }
    }

    void Audio::Stop()
    {
        //Safety check the channel isn't null
        if (m_Channel != nullptr)
        {
            //Stop the audio channel
            m_Channel->stop();
        }
    }

    bool Audio::IsPlaying()
    {
        //Safety check the channel
        if (m_Channel != nullptr)
        {
            //Determine if the channel playing?
            bool isPlaying = false;
            m_Channel->isPlaying(&isPlaying);
            return isPlaying;
        }
        return false;
    }

    void Audio::SetDoesLoop(bool aDoesLoop)
    {
        if (m_DoesLoop != aDoesLoop)
        {
            m_DoesLoop = aDoesLoop;
            FMOD_MODE mode = (m_DoesLoop == true ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF) | FMOD_2D | FMOD_HARDWARE;
            m_Sound->setMode(mode);
        }
    }

    bool Audio::DoesLoop()
    {
        return m_DoesLoop;
    }

    void Audio::SetFrequency(float aFrequency)
    {
        if (m_Frequency != aFrequency)
        {
            //Bounds check the frequency, it can't be negative
            m_Frequency = fmaxf(aFrequency, 0.0f);

            //Safety check the channel and set the channel's frequency
            if (m_Channel != nullptr)
            {
                m_Channel->setFrequency(m_Frequency);
            }
        }
    }

    float Audio::GetFrequency()
    {
        return m_Frequency;
    }

    void Audio::SetVolume(float aVolume)
    {
        if (m_Volume != aVolume)
        {
            //Bounds check the volume
            m_Volume = fminf(fmaxf(aVolume, 0.0f), 1.0f);

            //Safety check the channel and set the channel's volume
            if (m_Channel != nullptr)
            {
                m_Channel->setVolume(m_Volume);
            }
        }
    }

    float Audio::GetVolume()
    {
        return m_Volume;
    }

    void Audio::SetPan(float aPan)
    {
        if (m_Pan != aPan)
        {
            //Bounds check the volume
            m_Pan = fminf(fmaxf(aPan, -1.0f), 1.0f);

            //Safety check the channel and set the channel's volume
            if (m_Channel != nullptr)
            {
                m_Channel->setPan(m_Pan);
            }
        }
    }

    float Audio::GetPan()
    {
        return m_Pan;
    }

    void Audio::SetPriority(int aPriority)
    {
        //Set the priority
        m_Priority = aPriority;

        //Safety check the channel and set the channel's volume
        if (m_Channel != nullptr)
        {
            m_Channel->setPriority(m_Priority);
        }
    }

    int Audio::GetPriority()
    {
        return m_Priority;
    }

    void Audio::SetPosition(unsigned int aPosition)
    {
        //Bounds check the audio's position
        m_Position = aPosition < GetLength() ? aPosition : GetLength();

        //Set the position
        if (m_Channel != nullptr)
        {
            m_Channel->setPosition(aPosition, FMOD_TIMEUNIT_MS);
        }
    }

    unsigned int Audio::GetPosition()
    {
        //Safety check the channel and get the position
        if (m_Channel != nullptr)
        {
            m_Channel->getPosition(&m_Position, FMOD_TIMEUNIT_MS);
        }
        return m_Position;
    }

    unsigned int Audio::GetRemaining()
    {
        return GetLength() - GetPosition();
    }

    unsigned int Audio::GetLength()
    {
        return m_Length;
    }

    void Audio::InvalidateChannel()
    {
        m_Channel = nullptr;
    }

    FMOD_RESULT F_CALLBACK ChannelCallback(FMOD_CHANNEL* aChannel, FMOD_CHANNEL_CALLBACKTYPE aType, void* aCommanddata1, void* aCommanddata2)
    {
        FMOD::Channel* channel = (FMOD::Channel *)aChannel;

        //Playback has reach the end
        if (aType == FMOD_CHANNEL_CALLBACKTYPE_END)
        {
            //Get the user data (audio*) from the channel
            void* userData;
            channel->getUserData(&userData);

            //Safety check the user data
            if (userData != nullptr)
            {
                //Dispatch an audio playback finished event and invalidate the channel
                ((Audio*)userData)->InvalidateChannel();
            }
        }

        return FMOD_OK;
    }
}