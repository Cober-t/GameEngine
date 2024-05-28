#include <pch.h>
#define MINIAUDIO_IMPLEMENTATION
#include "Audio/miniaudio.h"
#include "Audio/Audio.h"


#define MAX_SOUNDS 32

namespace Cober {

    static ma_result m_Result;
    static ma_engine m_Engine;
    static ma_uint32 m_EngineCount = 0;
    static ma_context m_Context;
    static ma_device m_Device;
    static ma_device_config m_DeviceConfig;
    static ma_engine_config m_EngineConfig;
    static ma_resource_manager m_ResourceManager;
    static ma_resource_manager_config m_ResourceManagerConfig;
    static ma_device_info* m_PlaybackDeviceInfos;
    static ma_uint32 m_PlaybackDeviceCount;
    static ma_uint32 iAvailableDevice;
    static ma_sound sounds[MAX_SOUNDS];
    static ma_uint32 soundCount;
    static ma_uint32 iSound;

    void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
    {
        (void)pInput;
        ma_engine_read_pcm_frames((ma_engine*)pDevice->pUserData, pOutput, frameCount, NULL);
    }

    void Audio::Init() 
    {
        soundCount = 0;
        // Init Resource Manager
        m_ResourceManagerConfig = ma_resource_manager_config_init();
        m_Result = ma_resource_manager_init(&m_ResourceManagerConfig, &m_ResourceManager);
        m_ResourceManagerConfig.decodedFormat     = ma_format_f32;
        m_ResourceManagerConfig.decodedChannels   = 0;
        m_ResourceManagerConfig.decodedSampleRate = 48000;
        if (m_Result != MA_SUCCESS) 
        {
            ma_device_uninit(&m_Device);
            printf("Failed to initialize the audio resource manager.");
            return;
        }

        // Init Context
        m_Result = ma_context_init(NULL, 0, NULL, &m_Context);
        if (m_Result != MA_SUCCESS) 
        {
            ma_device_uninit(&m_Device);
            LOG_CORE_ERROR("Failed to initialize the audio resource manager.");
            return;
        }

        m_Result = ma_context_get_devices(&m_Context, &m_PlaybackDeviceInfos, &m_PlaybackDeviceCount, NULL, NULL);
        if (m_Result != MA_SUCCESS) {
            LOG_CORE_ERROR("Failed to enumerate playback devices.");
            ma_context_uninit(&m_Context);
            return;
        }

        for (ma_uint32 iAvailableDevice = 0; iAvailableDevice < m_PlaybackDeviceCount; iAvailableDevice += 1)
            LOG_WARNING(m_PlaybackDeviceInfos[iAvailableDevice].name);
        
        m_DeviceConfig = ma_device_config_init(ma_device_type_playback);
        // m_DeviceConfig.playback.pDeviceID = &m_PlaybackDeviceInfos[2].id;
        m_DeviceConfig.playback.format    = m_ResourceManager.config.decodedFormat;
        m_DeviceConfig.playback.channels  = 0;
        m_DeviceConfig.sampleRate         = m_ResourceManager.config.decodedSampleRate;
        m_DeviceConfig.dataCallback       = data_callback;
        m_DeviceConfig.pUserData          = &m_Engine;
        m_Result = ma_device_init(&m_Context, &m_DeviceConfig, &m_Device);
        if (m_Result != MA_SUCCESS) 
        {
            LOG_CORE_ERROR("Failed to initialize device");
            return;
        }

        m_EngineConfig = ma_engine_config_init();
        m_EngineConfig.pDevice          = &m_Device;
        m_EngineConfig.pResourceManager = &m_ResourceManager;
        m_EngineConfig.noAutoStart      = MA_TRUE;
        m_Result = ma_engine_init(&m_EngineConfig, &m_Engine);
        if (m_Result != MA_SUCCESS) 
        {
            LOG_CORE_ERROR("Failed to initialize engine");
            ma_device_uninit(&m_Device);
            return ;
        }

        // Start Engine
        m_Result = ma_engine_start(&m_Engine);
        if (m_Result != MA_SUCCESS) 
        {
            LOG_CORE_ERROR("Failed to start audio engine");
            return;
        }

        // TEST
        Audio::PlaySound("hit.mp3");
    }
    
    void Audio::Stop() 
    {
        for (ma_int32 iSound = 0; iSound < soundCount; iSound += 1)
            ma_sound_uninit(&sounds[iSound]);

        ma_engine_uninit(&m_Engine);
        ma_device_uninit(&m_Device);
        ma_context_uninit(&m_Context);
        ma_resource_manager_uninit(&m_ResourceManager);
    }

    void Audio::Update() 
    {
        
    }

    bool Audio::PlaySound(const char* soundName) 
    {
        if (!&m_Engine)
            return false;

        std::filesystem::path soundPath = std::filesystem::current_path() / "assets" / "audio" / soundName;
        // m_Result = ma_engine_play_sound(&m_Engine, soundPath.string().c_str(), NULL);
        m_Result = ma_sound_init_from_file(&m_Engine, soundPath.string().c_str(), 
                        MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_DECODE | 
                        MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_ASYNC | 
                        MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_STREAM, NULL, NULL, 
                        &sounds[soundCount]);

        if (m_Result != MA_SUCCESS) 
        {
            LOG_WARNING("Failed to load sound {0}", soundName);
            return false;
        }
        m_Result = ma_sound_start(&sounds[soundCount]);
        if (m_Result != MA_SUCCESS) 
        {
            LOG_WARNING("Failed to start sound.");
        }
        soundCount += 1;

        return true;
    }

    bool Audio::LoadSound(const char* soundName) 
    {
        return true;
    }

    void Audio::StopSound(const char* soundName) 
    {   
        // Map sounds name  
        // ma_sound_uninit(&sounds[iSound]);
    }

    void Audio::LoopSound(bool loop) 
    {
    }

    void Audio::SetVolume(int volume)
    {
    }
}