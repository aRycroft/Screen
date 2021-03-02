/*
  ==============================================================================

    AudioFile.h
    Created: 2 Mar 2021 7:45:17pm
    Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "AudioBuffer.h"

class AudioFile : public juce::AudioBuffer<float>
{
public:
    juce::OwnedArray<MyAudioBuffer> allSounds;
private:
};
