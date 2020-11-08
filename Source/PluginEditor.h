/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GrainGeneratorVis.h"
#include "MainPanel.h"

//==============================================================================
/**
*/
class ScreenAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ScreenAudioProcessorEditor (ScreenAudioProcessor&, juce::AudioProcessorValueTreeState& state);
    ~ScreenAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void updateFromValueTree();
    ScreenAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& apvts;
    //std::unique_ptr<GrainGeneratorVis> generatorVis[NUM_NODES];
    juce::OwnedArray<GrainGeneratorVis> generatorVis;
    std::unique_ptr<MainPanel> mainPanel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScreenAudioProcessorEditor)
};
