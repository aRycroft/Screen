/*
  ==============================================================================

    ScreenGUI.h
    Created: 30 Dec 2020 10:24:08am
    Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "MainPanel.h"
#include "SampleSelector.h"
#define NUMBEROFSECTIONS 10

class ScreenGUI : public juce::Component,
                  public IAudioFileHandler
{
public:
    ScreenGUI(juce::ValueTree state)
    {
        fileListener.reset(new FileListener(this, state.getChildWithName(Ids::fileTree)));
        mainPanel.reset(new MainPanel{ state });
        addAndMakeVisible(*mainPanel);

        sampleSelector.reset(new SampleSelector{ state.getChildWithName(Ids::fileTree)});
        addAndMakeVisible(*sampleSelector);
    }

    void resized() override
    {
        juce::Grid grid;

        using Track = juce::Grid::TrackInfo;
        using Fr = juce::Grid::Fr;

        grid.templateColumns = { Track(Fr(1)) };
        grid.templateRows = { Track(Fr(4)), Track(Fr(1)) };

        grid.items = { juce::GridItem(*mainPanel), juce::GridItem(*sampleSelector) };

        grid.performLayout(getLocalBounds());

        layoutSampleSections();
    }

    void addAudioFile(juce::ValueTree newAudioSource) override
    {
        juce::File file{ newAudioSource.getProperty(Ids::relativePath) };
        generateSampleSections(file, NUMBEROFSECTIONS);
        layoutSampleSections();
    }

    void addAudioBuffer(juce::ValueTree audioSource, juce::ValueTree childOfSource) override {};

private:
    void layoutSampleSections()
    {
        juce::FlexBox fb;
        fb.flexWrap = juce::FlexBox::Wrap::wrap;
        fb.justifyContent = juce::FlexBox::JustifyContent::center;
        fb.alignContent = juce::FlexBox::AlignContent::center;

        for (auto* section : sampleSections)
            fb.items.add(juce::FlexItem(*section).withMinWidth(50.0f).withMinHeight(50.0f));

        fb.performLayout(sampleSelector->getBounds().toFloat());
    }

    void generateSampleSections(juce::File audioSourceFile, int numberOfSections)
    {
        sampleSections.clearQuick(true);
        for (int i = 0; i < numberOfSections; i++)
        {
            addAndMakeVisible(sampleSections.add(new DraggableComponent()));
        }
    }

    std::unique_ptr<MainPanel> mainPanel;
    std::unique_ptr<SampleSelector> sampleSelector;
    juce::OwnedArray<DraggableComponent> sampleSections;
    std::unique_ptr<FileListener> fileListener;
};
