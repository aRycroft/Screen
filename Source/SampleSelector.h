/*
  ==============================================================================

    SampleSelector.h
    Created: 30 Dec 2020 10:29:18am
    Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class SampleSelector : public juce::Component,
    public juce::TextButton::Listener
{
public:
    SampleSelector(juce::ValueTree vTree)
        : fileTree(vTree)
    {
        addAndMakeVisible(chooseNewSampleButton);
        chooseNewSampleButton.addListener(this);
        chooser.reset(new juce::FileChooser("Select a Wave file to play...",
            {},
            "*.wav;*.mp3"));
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::aqua);
    }

    void resized() override
    {
        chooseNewSampleButton.setBounds(0, 0, 50, 50);
    }

    void buttonClicked(juce::Button* buttonThatWasClicked) 
    {
        if (chooser->browseForFileToOpen())
        {
            juce::File file = chooser->getResult();
            if (file.existsAsFile())
            {
                addAudioSourceTree(file);
            }
        }
    }

private:
    void addAudioSourceTree(juce::File audioSourceFile) 
    {
        if (!fileTree.getChildWithProperty(Ids::relativePath, audioSourceFile.getFullPathName()).isValid()) 
        {
            juce::ValueTree newFileValueTree(Ids::audioFile);
            newFileValueTree.setProperty(Ids::relativePath, audioSourceFile.getFullPathName(), nullptr);
            fileTree.addChild(newFileValueTree, fileTree.getNumChildren(), nullptr);
        }
    }

    juce::ValueTree fileTree;
    juce::TextButton chooseNewSampleButton;
    juce::OwnedArray<DraggableComponent> sampleSections;
    std::unique_ptr<juce::FileChooser> chooser;
};

