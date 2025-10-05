#include "E:\Unreal_Project\UE5FPS\Intermediate\Build\Win64\x64\UE5FPSEditor\Development\UnrealEd\SharedPCH.UnrealEd.Project.ValApi.ValExpApi.Cpp20.h"
#if WITH_WASAPI
#include "WindowsWASAPIRenderer.h"
#include "WavReader.h"
#include "Misc/ScopeLock.h"
#include "HAL/RunnableThread.h"
#include "HAL/Runnable.h"
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <avrt.h>
#include <Mmdeviceapi.h>
#include <Functiondiscoverykeys_devpkey.h>  // ✅ 这里定义了 PKEY_Device_FriendlyName
#include <propvarutil.h>
#include <Propsys.h>
#include <Audioclient.h>
#include <combaseapi.h>


#pragma comment(lib, "Avrt.lib")
#pragma comment(lib, "Ole32.lib")

struct FPlaybackTask : public FRunnable
{
    FString DeviceName;
    FWavData Wav;
    FThreadSafeBool bStop = false;

    virtual uint32 Run() override
    {
        // Initialize COM for this thread
        CoInitializeEx(NULL, COINIT_MULTITHREADED);

        HRESULT hr = S_OK;
        IMMDeviceEnumerator* enumerator = nullptr;
        hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, IID_PPV_ARGS(&enumerator));
        if (FAILED(hr) || !enumerator) { CoUninitialize(); return 1; }

        IMMDeviceCollection* collection = nullptr;
        hr = enumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &collection);
        if (FAILED(hr) || !collection) { enumerator->Release(); CoUninitialize(); return 2; }

        UINT count = 0; collection->GetCount(&count);
        IMMDevice* targetDevice = nullptr;

        for (UINT i=0;i<count;i++)
        {
            IMMDevice* dev = nullptr;
            if (collection->Item(i, &dev) == S_OK && dev)
            {
                IPropertyStore* props = nullptr;
                if (dev->OpenPropertyStore(STGM_READ, &props) == S_OK && props)
                {
                    PROPVARIANT varName; PropVariantInit(&varName);
                    if (props->GetValue(PKEY_Device_FriendlyName, &varName) == S_OK)
                    {
                        wchar_t* val = varName.pwszVal;
                        FString Friendly = FString(val);
                        if (Friendly.Contains(DeviceName))
                        {
                            targetDevice = dev; // keep reference
                            PropVariantClear(&varName);
                            props->Release();
                            break;
                        }
                        PropVariantClear(&varName);
                    }
                    props->Release();
                }
                dev->Release();
            }
        }

        if (!targetDevice)
        {
            collection->Release();
            enumerator->Release();
            CoUninitialize();
            return 3;
        }

        IAudioClient* audioClient = nullptr;
        if (targetDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void**)&audioClient) != S_OK)
        {
            targetDevice->Release(); collection->Release(); enumerator->Release(); CoUninitialize(); return 4;
        }

        WAVEFORMATEX waveFormat = {};
        waveFormat.wFormatTag = WAVE_FORMAT_PCM;
        waveFormat.nChannels = (WORD)Wav.NumChannels;
        waveFormat.nSamplesPerSec = Wav.SampleRate;
        waveFormat.wBitsPerSample = (WORD)Wav.BitsPerSample;
        waveFormat.nBlockAlign = (waveFormat.wBitsPerSample/8) * waveFormat.nChannels;
        waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;

        REFERENCE_TIME hnsBufferDuration = 10000000; // 1 second
        HRESULT hrInit = audioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, hnsBufferDuration, 0, &waveFormat, NULL);
        if (FAILED(hrInit))
        {
            audioClient->Release(); targetDevice->Release(); collection->Release(); enumerator->Release(); CoUninitialize(); return 5;
        }

        IAudioRenderClient* renderClient = nullptr;
        if (audioClient->GetService(__uuidof(IAudioRenderClient), (void**)&renderClient) != S_OK)
        {
            audioClient->Release(); targetDevice->Release(); collection->Release(); enumerator->Release(); CoUninitialize(); return 6;
        }

        UINT32 bufferFrameCount = 0; audioClient->GetBufferSize(&bufferFrameCount);
        BYTE* pData = nullptr;

        if (audioClient->Start() != S_OK)
        {
            renderClient->Release(); audioClient->Release(); targetDevice->Release(); collection->Release(); enumerator->Release(); CoUninitialize(); return 7;
        }

        // Convert WAV byte data (PCM) to frames and write to buffer
        int32 bytesPerSample = Wav.BitsPerSample / 8;
        int64 totalBytes = Wav.Data.Num();
        int64 pos = 0;

        while (pos < totalBytes && !bStop)
        {
            UINT32 padding = 0; audioClient->GetCurrentPadding(&padding);
            UINT32 framesAvailable = bufferFrameCount - padding;
            if (framesAvailable == 0) { FPlatformProcess::Sleep(0.005f); continue; }

            if (renderClient->GetBuffer(framesAvailable, &pData) != S_OK) break;

            int64 bytesWanted = (int64)framesAvailable * waveFormat.nBlockAlign;
            int64 bytesToCopy = FMath::Min(bytesWanted, totalBytes - pos);
            FMemory::Memzero(pData, bytesWanted);
            FMemory::Memcpy(pData, Wav.Data.GetData() + pos, bytesToCopy);
            pos += bytesToCopy;

            renderClient->ReleaseBuffer(framesAvailable, 0);
        }

        audioClient->Stop();
        renderClient->Release();
        audioClient->Release();
        targetDevice->Release();
        collection->Release();
        enumerator->Release();
        CoUninitialize();
        return 0;
    }
};

bool PlayWavToDevice_Windows(const FString& DeviceName, const FString& WavFilePath)
{
    FWavData Wav;
    if (!Wav.LoadFromFile(WavFilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("WAV load failed: %s"), *Wav.GetError());
        return false;
    }

    FPlaybackTask* Task = new FPlaybackTask();
    Task->DeviceName = DeviceName;
    Task->Wav = MoveTemp(Wav);

    FRunnableThread* Thread = FRunnableThread::Create(Task, TEXT("ControllerAudio_Playback"));
    if (!Thread) { delete Task; return false; }

    // Optionally, detach and let it run until finish. For simplicity, wait until thread finishes.
    Thread->WaitForCompletion();
    delete Thread; delete Task;
    return true;
}

#endif // WITH_WASAPI