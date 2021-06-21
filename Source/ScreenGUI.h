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
#include "AudioBufferSelectorVis.h"
constexpr auto NUMBEROFSECTIONS = 10;

class ScreenGUI : public juce::Component,
	public IAudioFileHandler,
	public juce::MouseListener
{
public:
	ScreenGUI(juce::ValueTree state)
		: vTree(state)
	{
		fileListener.reset(new FileListener(this, state.getChildWithName(Ids::fileTree)));

		mainPanel.reset(new MainPanel{ state });
		addAndMakeVisible(*mainPanel);

		sampleSelector.reset(new SampleSelector{ state.getChildWithName(Ids::fileTree) });
		addAndMakeVisible(*sampleSelector);

		formatManager.registerBasicFormats();
	}

	void syncState()
	{
		mainPanel->sendChangeMessagesOnValueTree();
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
		int audioSourceId = vTree.getChildWithName(Ids::fileTree).indexOf(newAudioSource);
		std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));
		generateSampleSections(audioSourceId, NUMBEROFSECTIONS, reader->lengthInSamples);
		layoutSampleSections();
	}

	void addAudioBuffer(juce::ValueTree audioSource, juce::ValueTree childOfSource) override {};

	void mouseUp(const juce::MouseEvent& event) override
	{
		auto* audioBufferVis = dynamic_cast <AudioBufferSelectorVis*> (event.eventComponent);
		if (audioBufferVis != nullptr && audioBufferVisIsInMainPanel(audioBufferVis)) 
		{
			createAudioBufferValueTree(vTree.getChildWithName(Ids::fileTree),
				audioBufferVis->audioFileTreeId,
				(float) audioBufferVis->getX() / mainPanel->getWidth(),
				(float) audioBufferVis->getY() / mainPanel->getHeight(),
				audioBufferVis->lowSample,
				audioBufferVis->highSample);
		}
		layoutSampleSections();
	}

private:
	void createAudioBufferValueTree(juce::ValueTree fileTree, int parentAudioSourceIndex, float x, float y, int startSample, int endSample)
	{
		juce::ValueTree newTree{ Ids::audioBuffer };
		newTree.setProperty(Ids::active, true, nullptr)
			.setProperty(Ids::x, x, nullptr)
			.setProperty(Ids::y, y, nullptr)
			.setProperty(Ids::lowSample, startSample, nullptr)
			.setProperty(Ids::highSample, endSample, nullptr);
		auto parentTree = fileTree.getChild(parentAudioSourceIndex);
		parentTree.addChild(newTree, -1, nullptr);
		auto addedTree = parentTree.getChild(parentTree.getNumChildren() - 1);
		addedTree.sendPropertyChangeMessage(Ids::x);
		addedTree.sendPropertyChangeMessage(Ids::y);
	}

	void generateSampleSections(int audioFileTreeId, int numberOfSections, int numSamplesInFile)
	{
		sampleSections.clearQuick(true);
		int samplesPerSection = numSamplesInFile / numberOfSections;
		for (int i = 0; i < numberOfSections; i++)
		{
			addAndMakeVisible(sampleSections.add(new AudioBufferSelectorVis(audioFileTreeId, i * samplesPerSection, (i + 1) * samplesPerSection)));
		}
	}

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

	bool audioBufferVisIsInMainPanel(AudioBufferSelectorVis* audioBuffer)
	{
		auto position = audioBuffer->getPosition();
		auto mainPanelBounds = mainPanel->getScreenBounds();
		return mainPanel->getBounds().contains(position);
	}

	std::unique_ptr<MainPanel> mainPanel;
	std::unique_ptr<SampleSelector> sampleSelector;
	std::unique_ptr<FileListener> fileListener;
	juce::OwnedArray<AudioBufferSelectorVis> sampleSections;
	juce::ValueTree vTree;
	juce::AudioFormatManager formatManager;
};
