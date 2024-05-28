#ifndef AUDIO_H
#define AUDIO_H



namespace Cober {

    class CB_API Audio
    {
    public:
        static void Init();
        static void Exit();

        static bool PlaySound(std::string soundName);
        static void StopSound(std::string soundName);
        static bool LoadSound(std::string soundName);
        static void LoopSound(std::string soundName, bool loop);
        static void SetVolume(int volume);
    };
}

#endif