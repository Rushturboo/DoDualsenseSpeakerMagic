#include "E:\Unreal_Project\UE5FPS\Intermediate\Build\Win64\x64\UE5FPSEditor\Development\UnrealEd\SharedPCH.UnrealEd.Project.ValApi.ValExpApi.Cpp20.h"
#include "WavReader.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
bool FWavData::LoadFromFile(const FString& Path)
{
    Data.Empty();

    TArray<uint8> FileBytes;
    if (!FFileHelper::LoadFileToArray(FileBytes, *Path))
    {
        Error = TEXT("Failed to load file");
        return false;
    }

    const uint8* Ptr = FileBytes.GetData();
    int32 Size = FileBytes.Num();
    int32 Offset = 0;

    auto ReadLE32 = [&](uint32& Out)
    {
        if (Offset + 4 > Size) return false;
        Out = Ptr[Offset] | (Ptr[Offset+1]<<8) | (Ptr[Offset+2]<<16) | (Ptr[Offset+3]<<24);
        Offset += 4; return true;
    };
    auto ReadLE16 = [&](uint16& Out)
    {
        if (Offset + 2 > Size) return false;
        Out = Ptr[Offset] | (Ptr[Offset+1]<<8);
        Offset += 2; return true;
    };

    uint32 ChunkID;
    if (!ReadLE32(ChunkID) || ChunkID != 0x46464952) // 'RIFF'
    { Error = TEXT("Not a RIFF file"); return false; }

    uint32 ChunkSize; if (!ReadLE32(ChunkSize)) { Error = TEXT("Truncated"); return false; }

    uint32 Format; if (!ReadLE32(Format) || Format != 0x45564157) // 'WAVE'
    { Error = TEXT("Not a WAVE file"); return false; }

    // Find fmt  chunk
    uint16 audioFormat = 0; uint16 numChannels = 0; uint32 sampleRate = 0; uint16 bitsPerSample = 0; uint32 dataSize = 0;
    while (Offset + 8 <= Size)
    {
        uint32 subID; ReadLE32(subID);
        uint32 subSize; ReadLE32(subSize);
        if (subID == 0x20746d66) // 'fmt '
        {
            uint16 audioFmt; ReadLE16(audioFmt); audioFormat = audioFmt;
            ReadLE16(numChannels);
            uint32 sr; ReadLE32(sr); sampleRate = sr;
            uint32 byteRate; ReadLE32(byteRate);
            uint16 blockAlign; ReadLE16(blockAlign);
            ReadLE16(bitsPerSample);
            Offset += (subSize - 16);
        }
        else if (subID == 0x61746164) // 'data'
        {
            dataSize = subSize;
            if ((size_t)Offset + dataSize > Size) { Error = TEXT("Data chunk truncated"); return false; }
            Data.Append(Ptr + Offset, dataSize);
            Offset += dataSize;
            break;
        }
        else
        {
            Offset += subSize; // skip
        }
    }

    if (Data.Num() == 0) { Error = TEXT("No PCM data found"); return false; }

    SampleRate = sampleRate;
    NumChannels = numChannels;
    BitsPerSample = bitsPerSample;
    return true;
}

FString FWavData::GetError() const { return Error; }