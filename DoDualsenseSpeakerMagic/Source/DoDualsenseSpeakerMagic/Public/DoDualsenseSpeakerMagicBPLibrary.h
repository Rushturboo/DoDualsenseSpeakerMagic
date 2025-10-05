#pragma once


#include "Kismet/BlueprintFunctionLibrary.h"
#include "DoDualsenseSpeakerMagicBPLibrary.generated.h"


UCLASS()
class DODUALSENSESPEAKERMAGIC_API UDoDualsenseSpeakerMagicBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	/**
	* Play a WAV file to the first audio endpoint whose friendly name contains DeviceName.
	* DeviceName can be a substring (e.g., "DualSense" or "Wireless Controller").
	* Returns true on success.
	*/
	UFUNCTION(BlueprintCallable, Category = "DoDualsenseSpeakerMagic|Audio")
	static bool PlayWavFileToDevice(const FString& DeviceName, const FString& WavFilePath);
};