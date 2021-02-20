/*
  ==============================================================================

	MainPanel.h
	Created: 8 Nov 2020 9:20:51am
	Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>>
#include "GrainGeneratorVis.h"
#include "GenListener.h"
#include "IGrainGenHandler.h"
#include "AudioFileVis.h"
#include "FileListener.h"
#include "IAudioFileHandler.h"
#include "Utils.h"

class MainPanel :
	public juce::Component,
	public IGrainGenHandler,
	public IAudioFileHandler,
	public juce::ValueTree::Listener,
	public juce::MouseListener
{
public:
	MainPanel(juce::ValueTree state)
		:genTree(state.getChildWithName(Ids::genTree)),
		fileTree(state.getChildWithName(Ids::fileTree))
	{
		genListener.reset(new GenListener(this, genTree));
		sendChangeMessageOnValueTree(state);
	}

	void paint(juce::Graphics& g) override
	{
		g.fillAll(juce::Colours::antiquewhite);
	}

	void resized() override
	{
		for (auto grainVis : generatorVis) {
			grainVis->setBounds(grainVis->calculateBounds().toNearestInt());
		}
	}

	void createGrainGenerator(juce::ValueTree generatorValueTree) override 
	{
		generatorVis.add(new GrainGeneratorVis(generatorValueTree));
		addAndMakeVisible(generatorVis.getLast());
		generatorVis.getLast()->setBounds(generatorVis.getLast()->calculateBounds().toNearestInt());
		generatorVis.getLast()->addMouseListener(this, false);
	
	}

	void removeGrainGenerator(int indexToRemove) override 
	{
		generatorVis.remove(indexToRemove);
	}
	
	void addAudioBuffer(juce::ValueTree newAudioSource) override 
	{
	
	};
	
	void addAudioFile(juce::ValueTree audioSource, juce::ValueTree childOfSource) 
	{
		audioFileVis.add(new AudioFileVis(childOfSource));
		addAndMakeVisible(audioFileVis.getLast());
		audioFileVis.getLast()->setBounds(audioFileVis.getLast()->calculateBounds().toNearestInt());
	};

	void mouseDoubleClick(const juce::MouseEvent& event) override 
	{
		GrainGeneratorVis* clickedGrainGen = dynamic_cast <GrainGeneratorVis*> (event.eventComponent);
		
		if (clickedGrainGen != 0) {
			genTree.removeChild(clickedGrainGen->getValueTree(), nullptr);
		}
		else
		{
			genTree.addChild(createGeneratorValueTree((float)event.getMouseDownX() / getWidth(), (float)event.getMouseDownY() / getHeight()), -1, nullptr);
		}
	}

private:

	juce::ValueTree createGeneratorValueTree(float x, float y)
	{
		juce::ValueTree newTree{ Ids::generator };
		newTree
			.setProperty(Ids::active, true, nullptr)
			.setProperty(Ids::numVoices, 20, nullptr)
			.setProperty(Ids::x, x, nullptr)
			.setProperty(Ids::y, y, nullptr);
		genTree.addChild(newTree, -1, nullptr);
		return newTree;
	}

	juce::ValueTree createAudioSourceValueTree(int parentAudioSourceIndex, float x, float y)
	{
		juce::ValueTree newTree{ Ids::audioFile};
		newTree
			.setProperty(Ids::active, true, nullptr)
			.setProperty(Ids::x, x, nullptr)
			.setProperty(Ids::y, y, nullptr);
		fileTree.getChild(parentAudioSourceIndex).addChild(newTree, -1, nullptr);
		return newTree;
	}

	void sendChangeMessageOnValueTree(juce::ValueTree state)
	{
		for (auto param : state) {
			for (auto child : param) {
				for (int i = 0; i < child.getNumProperties(); i++) {
					child.sendPropertyChangeMessage(child.getPropertyName(i));
				}
			}
		}
	}
	
	juce::OwnedArray<GrainGeneratorVis> generatorVis;
	juce::OwnedArray<AudioFileVis> audioFileVis;
	juce::ValueTree genTree, fileTree;
	std::unique_ptr<GenListener> genListener;
	std::unique_ptr<FileListener> fileListener;
};
