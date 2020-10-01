/*
  ==============================================================================

    Utils.h
    Created: 11 Sep 2020 9:13:03pm
    Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace Ids
{
#define MAKE_ID(name) const juce::Identifier name (#name)
    /*AUDIO SOURCE*/
    MAKE_ID(sourceTree);
    MAKE_ID(audioSource);
    MAKE_ID(lowSample);
    MAKE_ID(highSample);
    /*FILE*/
    MAKE_ID(fileTree);
    MAKE_ID(relativePath);

}


namespace util {
    static void fillAudioBufferWithValue(juce::AudioBuffer<float>* bufferToFill, float value)
    {
        for (int i{ 0 }; i < bufferToFill->getNumSamples(); i++) {
            for (int channel{ 0 }; channel < bufferToFill->getNumChannels(); channel++) {
                bufferToFill->setSample(channel, i, value);
            }
        }
    }

    static juce::ValueTree createSourceValueTree(int sourceID, juce::String relativePath)
    {
        juce::ValueTree sourceTree{ juce::String{sourceID} };
        sourceTree.setProperty(Ids::relativePath, relativePath, nullptr);
        return sourceTree;
    }

    static void addAudioFileToTree(juce::ValueTree* sourceTree,
        int x,
        int y,
        int lowSample,
        int highSample
    )
    {
        juce::ValueTree newAudioFileTree(Ids::audioSource);
        newAudioFileTree.setProperty("x", x, nullptr);
        newAudioFileTree.setProperty("y", y, nullptr);
        newAudioFileTree.setProperty(Ids::lowSample, lowSample, nullptr);
        newAudioFileTree.setProperty(Ids::highSample, highSample, nullptr);
        sourceTree->addChild(newAudioFileTree, sourceTree->getNumChildren(), nullptr);
    }
}

