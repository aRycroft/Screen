/*
  ==============================================================================

	MainPanel.h
	Created: 8 Nov 2020 9:20:51am
	Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "GrainGeneratorVis.h"
#include "GenListener.h"
#include "IGrainGenHandler.h"
#include "AudioBufferVis.h"
#include "FileListener.h"
#include "ConnectionDragMouseListener.h"
#include "IAudioFileHandler.h"
#include "IConnectionDragHandler.h"
#include "Utils.h"

class MainPanel :
	public juce::Component,
	public IGrainGenHandler,
	public IAudioFileHandler,
	public IConnectionDragHandler
{
public:
	MainPanel(juce::ValueTree state)
		:genTree(state.getChildWithName(Ids::genTree)),
		fileTree(state.getChildWithName(Ids::fileTree))
	{
		genListener.reset(new GenListener(this, genTree));
		fileListener.reset(new FileListener(this, fileTree));
		connectionDragMouseListener.reset(new ConnectionDragMouseListener(this));
		this->addMouseListener(connectionDragMouseListener.get(), true);
	}

	void paint(juce::Graphics& g) override
	{
		g.fillAll(juce::Colours::antiquewhite);
		if (grainGenIsConnectionDragging)
		{
			auto grainGenPosition = grainGenThatIsDragging->getPosition();
			auto line = new juce::Line<int>{ grainGenPosition, this->getMouseXYRelative() };
			g.drawArrow(line->toFloat(), 10.0f, 50.0f, 10.0f);
		}
	}

	void resized() override
	{
		for (auto grainVis : generatorVis) {
			grainVis->setBounds(grainVis->calculateBounds().reduced(1).toNearestInt());
		}
	}

	void createGrainGenerator(juce::ValueTree generatorValueTree) override
	{
		generatorVis.add(new GrainGeneratorVis(generatorValueTree));
		addAndMakeVisible(generatorVis.getLast());
		generatorVis.getLast()->setBounds(generatorVis.getLast()->calculateBounds().toNearestInt());
	}

	void removeGrainGenerator(int indexToRemove) override
	{
		generatorVis.remove(indexToRemove);
	}

	void addSoundToGrainGenerator(int grainGenIndex, int audioFileIndex, int audioBufferIndex) override {};
	void removeSoundFromGrainGenerator(int grainGenIndex, int audioFileIndex, int audioBufferIndex) override {};

	void addAudioFile(juce::ValueTree newAudioSource) override {};

	void addAudioBuffer(juce::ValueTree audioSource, juce::ValueTree childOfSource) override
	{
		auto audioBuffer = audioFileVis.add(new AudioBufferVis(childOfSource));
		addAndMakeVisible(audioBuffer);
		audioBuffer->setBounds(audioBuffer->calculateBounds().toNearestInt());
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

	void sendChangeMessageOnValueTree()
	{
		for (auto child : genTree) {
			genListener->valueTreeChildAdded(genTree, child);
			for (int i = 0; i < child.getNumProperties(); i++) {
				child.sendPropertyChangeMessage(child.getPropertyName(i));
			}
		}
		for (auto child : fileTree)
		{
			fileListener->valueTreeChildAdded(fileTree, child);
			for (auto buffer : child)
			{
				fileListener->valueTreeChildAdded(child, buffer);
				for (int i = 0; i < buffer.getNumProperties(); i++) {
					buffer.sendPropertyChangeMessage(buffer.getPropertyName(i));
				}
			}
		}
	}

	void startConnectionDrag(GrainGeneratorVis* grainGenVis) override
	{
		grainGenIsConnectionDragging = true;
		grainGenThatIsDragging = grainGenVis;
	};

	void connectionDrag() override
	{
		if (grainGenIsConnectionDragging) 
		{
			repaint();
		}
	};

	void endConnectionDrag(const juce::MouseEvent& event) override
	{
		if (grainGenIsConnectionDragging) 
		{
			GrainGeneratorVis* clickedGrainGen = dynamic_cast <GrainGeneratorVis*> (event.eventComponent);
			grainGenIsConnectionDragging = false;
			repaint();
		}
	};

private:
	juce::ValueTree createGeneratorValueTree(float x, float y)
	{
		juce::ValueTree newTree{ Ids::generator };
		newTree
			.setProperty(Ids::active, true, nullptr)
			.setProperty(Ids::numVoices, 20, nullptr)
			.setProperty(Ids::frequency, 100.0, nullptr)
			.setProperty(Ids::x, x, nullptr)
			.setProperty(Ids::y, y, nullptr);
		genTree.addChild(newTree, -1, nullptr);
		return newTree;
	}

	juce::OwnedArray<GrainGeneratorVis> generatorVis;
	juce::OwnedArray<AudioBufferVis> audioFileVis;
	juce::ValueTree genTree, fileTree;
	std::unique_ptr<GenListener> genListener;
	std::unique_ptr<FileListener> fileListener;
	std::unique_ptr<ConnectionDragMouseListener> connectionDragMouseListener;
	bool grainGenIsConnectionDragging{ false };
	GrainGeneratorVis* grainGenThatIsDragging;
};
