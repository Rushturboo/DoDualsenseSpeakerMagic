#include "DoDualsenseSpeakerMagicBPLibrary.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"

#if WITH_WASAPI
#include "WindowsWASAPIRenderer.h"
#endif

bool UDoDualsenseSpeakerMagicBPLibrary::PlayWavFileToDevice(const FString& DeviceName, const FString& WavFilePath)
{
#if WITH_WASAPI
	Async(EAsyncExecution::Thread, [DeviceName, WavFilePath]()
{
	PlayWavToDevice_Windows(DeviceName, WavFilePath);
});

	return true;

#else
	UE_LOG(LogTemp, Warning, TEXT("PlayWavFileToDevice: WASAPI not available on this platform."));
	return false;
#endif
}
