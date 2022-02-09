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
#include "BufferOptionsPanel.h"
#include "GrainOptionsPanel.h"
#include "AudioBufferSelectorVis.h"
constexpr auto NUMBEROFSECTIONS = 10;

class ScreenGUI : public juce::Component,
	public IAudioFileHandler,
	public juce::MouseListener,
	public juce::ChangeListener
{
public:
	ScreenGUI(juce::ValueTree state)
		: vTree(state)
	{
		fileListener = std::make_unique<FileListener>(this, state.getChildWithName(Ids::fileTree));

		mainPanel = std::make_unique<MainPanel>(state);
		addAndMakeVisible(*mainPanel);

		sampleSelector = std::make_unique<SampleSelector>(state.getChildWithName(Ids::fileTree));
		addAndMakeVisible(*sampleSelector);

		bufferOptionsPanel = std::make_unique<BufferOptionsPanel>();
		addChildComponent(*bufferOptionsPanel);

		grainOptionsPanel = std::make_unique<GrainOptionsPanel>();
		addChildComponent(*grainOptionsPanel);

		formatManager.registerBasicFormats();
		groupDragMouseListener = std::make_unique<GroupDragMouseListener>();

		mainPanel->selectedOption->addChangeListener(this);
	}

	void syncState()
	{
		mainPanel->sendChangeMessagesOnValueTree();
		mainPanel->resized();
	}

	void resized() override
	{
		juce::Grid grid;

		using Track = juce::Grid::TrackInfo;
		using Fr = juce::Grid::Fr;

		grid.templateColumns = { Track(Fr(1)) };
		grid.templateRows = { Track(Fr(4)), Track(Fr(1)) };
		
		hideAllPanels();

		switch (selectedMenu)
		{
		case MenuOption::Sample:
			sampleSelector->setVisible(true);
			showSampleSections(true);
			grid.items = { juce::GridItem(*mainPanel), juce::GridItem(*sampleSelector) };
			layoutSampleSections();
			showSampleSections(true);
			break;
		case MenuOption::GrainMenu:
			grainOptionsPanel->setVisible(true);
			grainOptionsPanel->setValueTree(mainPanel->selectedOption->valueTree);
			grid.items = { juce::GridItem(*mainPanel), juce::GridItem(*grainOptionsPanel) };
			break;
		case MenuOption::Buffer:
			bufferOptionsPanel->setVisible(true);
			bufferOptionsPanel->setValueTree(mainPanel->selectedOption->valueTree);
			grid.items = { juce::GridItem(*mainPanel), juce::GridItem(*bufferOptionsPanel) };
			break;
		default:
			break;
		}
		grid.performLayout(getLocalBounds());
	}

	void hideAllPanels()
	{
		bufferOptionsPanel->setVisible(false);
		grainOptionsPanel->setVisible(false);
		sampleSelector->setVisible(false);
		showSampleSections(false);
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
			createAudioBufferValueTree(vTree.getChildWithName(Ids::fileTree), audioBufferVis);
		}
		layoutSampleSections();
	}

	void changeListenerCallback(juce::ChangeBroadcaster* source) override 
	{
		selectedMenu = mainPanel->selectedOption->getMenu();
		resized();
		repaint();
	}

private:
	void createAudioBufferValueTree(juce::ValueTree fileTree, AudioBufferSelectorVis* audioBufferVis)
	{
		juce::ValueTree newTree{ Ids::audioBuffer };
		newTree.setProperty(Ids::active, true, nullptr)
			.setProperty(Ids::x, (float)audioBufferVis->getX() / mainPanel->getWidth(), nullptr)
			.setProperty(Ids::y, (float)audioBufferVis->getY() / mainPanel->getHeight(), nullptr)
			.setProperty(Ids::lowSample, audioBufferVis->lowSample, nullptr)
			.setProperty(Ids::highSample, audioBufferVis->highSample, nullptr)
			.setProperty(Ids::maxSample, audioBufferVis->maxSample, nullptr);
		auto parentTree = fileTree.getChild(audioBufferVis->audioFileTreeId);
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
			addAndMakeVisible(sampleSections.add(std::make_unique<AudioBufferSelectorVis>(audioFileTreeId, i * samplesPerSection, (i + 1) * samplesPerSection, numSamplesInFile)));
			sampleSections[i]->addMouseListener(groupDragMouseListener.get(), false);
		}
	}

	void showSampleSections(bool shouldBeShown)
	{
		for (auto section : sampleSections)
		{
			section->setVisible(shouldBeShown);
		}
	}

	void layoutSampleSections()
	{
		juce::FlexBox fb;
		
		fb.flexWrap = juce::FlexBox::Wrap::wrap;
		fb.justifyContent = juce::FlexBox::JustifyContent::center;
		fb.alignContent = juce::FlexBox::AlignContent::center;

		for (auto* section : sampleSections)
		{
			fb.items.add(juce::FlexItem(*section).withMinWidth(50.0f).withMinHeight(50.0f));
		}

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
	std::unique_ptr<BufferOptionsPanel> bufferOptionsPanel;
	std::unique_ptr<GrainOptionsPanel> grainOptionsPanel;
	std::unique_ptr<FileListener> fileListener;
	std::unique_ptr<GroupDragMouseListener> groupDragMouseListener;
	juce::OwnedArray<AudioBufferSelectorVis> sampleSections;
	juce::ValueTree vTree;
	juce::AudioFormatManager formatManager;
	MenuOption selectedMenu = MenuOption::Sample;
};
