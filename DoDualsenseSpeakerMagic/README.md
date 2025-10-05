# 🎮 DoDualsenseSpeakerMagic
Unreal Engine 5.6 plugin that allows playing audio directly through the DualSense controller speaker using native WASAPI, **without Wwise or other middleware**. Man! What Can I Say!

## ✨ Features
- Output specific sounds to DualSense speaker
- Full control via Blueprint (`PlayWavFileToDevice`)
- No external dependencies (pure Win32 + WASAPI)
- Tested on UE 5.6

## 🧩 Installation
1. Clone this repository or download as ZIP
2. Place it in your project under `Plugins/`
3. Rebuild your project
4. In Blueprint, call `PlayWavFileToDevice(DeviceName, WavFilePath)`

## 🧠 Notes
- DualSense must be connected by USB (I have not tested on Bluetooth yet because the response on bluetooth is suck...)
- WAV file must be 16-bit PCM, 48kHz
- Use `PrintAllAudioDevices()` (debug build) to see available device names (to check if the dualsense controller is connecting **(Which named as `dualsense wireless controller`)**)

## SKIPMYBULLSH*T
- I Write this plugin because https://github.com/rafaelvaloto/WindowsDualsenseUnreal this project didn't give a way to make my dualsense speaker play sounds like *scanning in Stella Blade* and *radio noise in Silent Hill 2 Remake*. If u want to get your game a better performance on dualsense then use WindowsDualsenseUnreal plugin *(which support basic control and mutiple vibe and led stuff and so on)* and this plugin.
- Can Ropz comeback to play in FazeClan??? Apex is suck these days... I miss the old Faze Clan...
- Why Silent Hill F is so f**kin expensive???
- F*ck Konami
- I Love Hideo Kojima and Tyler the creator
- Follow *Fucked up Thursday* and *Summertime Sadness* plzzzzzzzz