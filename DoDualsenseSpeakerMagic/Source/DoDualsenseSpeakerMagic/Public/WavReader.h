#pragma once

#include "CoreMinimal.h"

struct FWavData
{
	TArray<uint8> Data;
	int32 SampleRate = 0;
	int32 NumChannels = 0;
	int32 BitsPerSample = 0;
	FString Error;

	bool LoadFromFile(const FString& Path);
	FString GetError() const;
};
