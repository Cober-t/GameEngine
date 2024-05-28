#ifndef AUDIO_H
#define AUDIO_H


namespace Cober {

    class CB_API Audio
    {
    public:
        static void Init();
        static void Update();
        static void Stop();

        static bool PlaySound(const char* soundName);
        static void StopSound(const char* soundName);
        static bool LoadSound(const char* soundName);
        static void LoopSound(bool loop);
        static void SetVolume(int volume);
    };
}

#endif