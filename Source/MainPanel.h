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
#include "GroupDragMouseListener.h"
#include "Utils.h"

class MainPanel :
	public juce::Component,
	public IGrainGenHandler,
	public IAudioFileHandler,
	public IConnectionDragHandler,
	public juce::LassoSource<DraggableComponent*>,
	public juce::ChangeListener,
	public juce::ValueTree::Listener
{
public:
	MainPanel(juce::ValueTree state)
		:genTree(state.getChild(TreeChildren::genTree)),
		fileTree(state.getChild(TreeChildren::fileTree)),
		connectionTree(state.getChild(TreeChildren::connectionTree))
	{
		genListener.reset(new GenListener(this, genTree));
		fileListener.reset(new FileListener(this, fileTree));
		connectionDragMouseListener.reset(new ConnectionDragMouseListener(this));
		groupDragMouseListener.reset(new GroupDragMouseListener());
		this->addMouseListener(connectionDragMouseListener.get(), true);
		this->addAndMakeVisible(lasso);
		this->setInterceptsMouseClicks(true, false);
		groupDragMouseListener->draggableItemSet.addChangeListener(this);
		genTree.addListener(this);
	}

	void paint(juce::Graphics& g) override
	{
		g.fillAll(juce::Colours::antiquewhite);

		for (auto tree : connectionTree)
		{
			auto fromGrainGen = generatorVis[tree[Ids::from]];
			auto toGrainGen = generatorVis[tree[Ids::to]];
			auto line = new juce::Line<int>{ fromGrainGen->getPosition(), toGrainGen->getPosition() };
			g.drawArrow(line->toFloat(), 10.0f, 50.0f, 10.0f);
		}

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
		auto genVis = generatorVis.add(new GrainGeneratorVis(generatorValueTree));
		addAndMakeVisible(genVis);
		genVis->setBounds(genVis->calculateBounds().toNearestInt());
		genVis->addMouseListener(groupDragMouseListener.get(), false);
		genVis->setAlwaysOnTop(true);
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
		audioBuffer->addMouseListener(groupDragMouseListener.get(), false);
	};

	void mouseDown(const juce::MouseEvent& event)
	{
		groupDragMouseListener->draggableItemSet.deselectAll();
		lasso.beginLasso(event, this);
	}

	void mouseDrag(const juce::MouseEvent& event)
	{
		lasso.dragLasso(event);
	}

	void mouseUp(const juce::MouseEvent& event) 
	{
		lasso.endLasso();
	}

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

	void sendChangeMessagesOnValueTree()
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
			GrainGeneratorVis* clickedGrainGen = nullptr;
			for (auto genVis : generatorVis) 
			{
				if (genVis->getBoundsInParent().contains(this->getMouseXYRelative())) 
				{
					clickedGrainGen = genVis;
				}
			}
			if (clickedGrainGen != 0) 
			{
				createConnectionValueTree(generatorVis.indexOf(grainGenThatIsDragging), generatorVis.indexOf(clickedGrainGen));
			}
			grainGenIsConnectionDragging = false;
			repaint();
		}
	};

	void findLassoItemsInArea(juce::Array<DraggableComponent*>& itemsFound, const juce::Rectangle<int>& area) override 
	{
		for (auto& genVis : generatorVis)
		{
			if (area.intersects(genVis->getBounds()))
			{
				itemsFound.add(genVis);
			}
		}
		for (auto& fileVis : audioFileVis)
		{
			if (area.intersects(fileVis->getBounds()))
			{
				itemsFound.add(fileVis);
			}
		}
	}

	juce::SelectedItemSet<DraggableComponent*>& getLassoSelection() override
	{
		return groupDragMouseListener->draggableItemSet;
	}

	void changeListenerCallback(juce::ChangeBroadcaster* source) override
	{
		for (auto gen : generatorVis) 
		{
			if (groupDragMouseListener->draggableItemSet.isSelected(gen)) 
			{
				gen->selected = true;
			}
			else 
			{
				gen->selected = false;
			}
			gen->repaint();
		}
		for (auto gen : audioFileVis)
		{
			if (groupDragMouseListener->draggableItemSet.isSelected(gen))
			{
				gen->selected = true;
			}
			else
			{
				gen->selected = false;
			}
			gen->repaint();
		}
	}
private:
	juce::ValueTree createGeneratorValueTree(float x, float y)
	{
		juce::ValueTree newTree{ Ids::generator };
		newTree
			.setProperty(Ids::active, true, nullptr)
			.setProperty(Ids::numVoices, 100, nullptr)
			.setProperty(Ids::frequency, 1000.0, nullptr)
			.setProperty(Ids::x, x, nullptr)
			.setProperty(Ids::y, y, nullptr);
		genTree.addChild(newTree, -1, nullptr);
		return newTree;
	}

	void createConnectionValueTree(int from , int to)
	{
		for (auto tree : connectionTree)
		{
			if ((int) tree[Ids::from] == from && (int) tree[Ids::to] == to)
			{
				return;
			}
		}
		juce::ValueTree newTree{ Ids::connection };
		newTree
			.setProperty(Ids::from, from, nullptr)
			.setProperty(Ids::to, to, nullptr)
			.setProperty(Ids::weight, 1, nullptr);
		connectionTree.addChild(newTree, -1, nullptr);
	}

	void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property) override
	{
		if (property == Ids::x)
		{
			int indexOfGenVis = genTree.indexOf(treeWhosePropertyHasChanged);
			for (auto child : connectionTree) 
			{
				if (indexOfGenVis == (int) child[Ids::from] || indexOfGenVis == (int) child[Ids::to])
				{
					child.sendPropertyChangeMessage(Ids::weight);
				}
			}
		}
	}

	juce::OwnedArray<GrainGeneratorVis> generatorVis;
	juce::OwnedArray<AudioBufferVis> audioFileVis;
	juce::ValueTree genTree, fileTree, connectionTree;
	std::unique_ptr<GenListener> genListener;
	std::unique_ptr<FileListener> fileListener;
	std::unique_ptr<ConnectionDragMouseListener> connectionDragMouseListener;
	std::unique_ptr<GroupDragMouseListener> groupDragMouseListener;
	bool grainGenIsConnectionDragging{ false };
	GrainGeneratorVis* grainGenThatIsDragging{ nullptr };
	juce::LassoComponent<DraggableComponent*> lasso;
};
