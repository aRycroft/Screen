/*
  ==============================================================================

    AudioSource.h
    Created: 27 Sep 2020 3:00:36pm
    Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
class AudioSource
{
public:
    AudioSource(juce::String relativeFilePath)
        : sourceFile(relativeFilePath)
    {
        jassert(sourceFile.existsAsFile());
    }

private:
    juce::File sourceFile;
};
