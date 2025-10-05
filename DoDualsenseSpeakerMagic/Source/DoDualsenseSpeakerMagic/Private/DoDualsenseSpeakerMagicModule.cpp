// Copyright Epic Games, Inc. All Rights Reserved.

#include "DoDualsenseSpeakerMagicModule.h"
#include "DoDualsenseSpeakerMagicBPLibrary.h"

#define LOCTEXT_NAMESPACE "FDoDualsenseSpeakerMagicModule"

IMPLEMENT_MODULE(FDoDualsenseSpeakerMagicModule, DoDualsenseSpeakerMagic);



// Forward declaration implemented in WindowsWASAPIRenderer.cpp
#if WITH_WASAPI
bool PlayWavToDevice_Windows(const FString& DeviceName, const FString& WavFilePath);
#endif

void FDoDualsenseSpeakerMagicModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FDoDualsenseSpeakerMagicModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
