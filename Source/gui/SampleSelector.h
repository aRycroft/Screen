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
	SampleSelector(ScreenAudioProcessor& p)
		: audioProcessor(p)
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
		chooser->launchAsync(juce::FileBrowserComponent::canSelectFiles, [this](const juce::FileChooser& fileChooser)
			{
				juce::File file = fileChooser.getResult();
				if (file.existsAsFile())
				{
					audioProcessor.createAudioFileValueTree(file);
				}
			});
	}
private:
	juce::TextButton chooseNewSampleButton;
	std::unique_ptr<juce::FileChooser> chooser;
	ScreenAudioProcessor& audioProcessor;
};

