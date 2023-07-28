#ifndef __GameDev2D__Audio__
#define __GameDev2D__Audio__

#include <fmod.hpp>
#include <string>


namespace GameDev2D
{
    //Audio class to handle playback of both music and sounds effects in game.
    class Audio
    {
    public:
        //Constructor for the Audio class, many extensions are supported, use 'streamed' for longer files such as background music.
        //Don't stream for short files, such as sound effects. Audio files be looped.
        Audio(FMOD::System* system, FMOD::Sound* sound);
        ~Audio();

        //Play, Pause and Stop methods, can be delayed (in seconds), by default there is no delay
        void Play();
        void Pause();
        void Stop();

        //Returns wether the audio is playing
        bool IsPlaying();

        //Sets the audio file loops or not
        void SetDoesLoop(bool doesLoop);

        //Returns the audio file loops or not
        bool DoesLoop();

        //Set the frequency of the audio file
        void SetFrequency(float frequency);

        //Returns the current frequency of the audio file, must be positive
        float GetFrequency();

        //Set the volume of the audio file
        void SetVolume(float volume);

        //Returns the current volume of the audio file, range 0.0f to 1.0f
        float GetVolume();

        //Set the pan of the audio file
        void SetPan(float pan);

        //Returns the current pan of the audio file, range -1.0f to 1.0f
        float GetPan();

        //Sets the priority of the audio file
        void SetPriority(int priority);

        //Returns the audio file's priority
        int GetPriority();

        //Sets the current position of the audi0 file, in milleseconds
        void SetPosition(unsigned int position);

        //Returns the current time of the audio file, in milleseconds
        unsigned int GetPosition();

        //Returns the remaining time of the audio file, in milleseconds
        unsigned int GetRemaining();

        //Returns the total length of the audio file, in milleseconds
        unsigned int GetLength();

        //Invalidates the current audio channel, this stops all playback,
        //if you need to stop the audio, use the Stop() method instead,
        //this is called when the playback ends OR when the channel is
        //needed by a higher priority audio file
        void InvalidateChannel();

    private:
        //Member variables
        FMOD::System* m_System;
        FMOD::Sound* m_Sound;
        FMOD::Channel* m_Channel;
        unsigned int m_Length;
        float m_Frequency;
        float m_Volume;
        float m_Pan;
        int m_Priority;
        unsigned int m_Position;
        bool m_DoesLoop;
    };
}

#endif
