#pragma once

#include "CoreMinimal.h"

#if WITH_WASAPI
bool PlayWavToDevice_Windows(const FString& DeviceName, const FString& WavFilePath);
#endif
