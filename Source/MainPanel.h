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
#include "SelectedOption.h"

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
		: genTree(state.getChild(TreeChildren::genTree)),
		fileTree(state.getChild(TreeChildren::fileTree)),
		connectionTree(state.getChild(TreeChildren::connectionTree))
	{
		genListener = std::make_unique<GenListener>(this, genTree);
		fileListener = std::make_unique<FileListener>(this, fileTree);
		connectionDragMouseListener = std::make_unique<ConnectionDragMouseListener>(this);
		groupDragMouseListener = std::make_unique<GroupDragMouseListener>();
		selectedOption = std::make_unique<SelectedOption>();

		this->addMouseListener(connectionDragMouseListener.get(), true);
		this->addAndMakeVisible(lasso);
		this->setInterceptsMouseClicks(true, false);
		groupDragMouseListener->draggableItemSet.addChangeListener(this);
		genTree.addListener(this);
		fileTree.addListener(this);
	}

	void paint(juce::Graphics& g) override
	{
		g.fillAll(juce::Colours::antiquewhite);

		for (auto tree : connectionTree)
		{
			auto fromGrainGen = generatorVis[tree[Ids::from]];
			auto toGrainGen = generatorVis[tree[Ids::to]];
			auto line = calculateConnectionLine(*fromGrainGen, *toGrainGen).toFloat();
			g.drawArrow(line, 10.0f, 50.0f, 10.0f);
		}

		if (grainGenIsConnectionDragging)
		{
			auto grainGenPosition = grainGenThatIsDragging->getPointOnEdge() + grainGenThatIsDragging->getPosition().toFloat();
			auto line = juce::Line<float>{ grainGenPosition, this->getMouseXYRelative().toFloat() };
			g.drawArrow(line, 10.0f, 50.0f, 10.0f);
		}
	}

	void resized() override
	{
		for (auto grainVis : generatorVis)
		{
			auto bounds = grainVis->calculateBounds();
			grainVis->setBounds(bounds
				.withWidth(bounds.getWidth() + grainVis->getValueTreeProperty(Ids::distance) * 1000)
				.withHeight(bounds.getHeight() + grainVis->getValueTreeProperty(Ids::distance) * 1000)
				.toNearestInt());
		}
		for (auto fileVis : audioBufferVis)
		{
			fileVis->setBounds(fileVis->calculateBounds().toNearestInt());
		}
	}

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

		if (clickedGrainGen != 0) 
		{
			genTree.removeChild(clickedGrainGen->getValueTree(), nullptr);
		}
		else
		{
			genTree.addChild(createGeneratorValueTree((float)event.getMouseDownX() / getWidth(), (float)event.getMouseDownY() / getHeight()), -1, nullptr);
		}
	}

	void createGrainGenerator(juce::ValueTree generatorValueTree) override
	{
		auto genVis = generatorVis.add(std::make_unique<GrainGeneratorVis>(generatorValueTree));
		addAndMakeVisible(genVis);
		genVis->setBounds(genVis->calculateBounds().toNearestInt());
		genVis->addMouseListener(groupDragMouseListener.get(), false);
		genVis->setAlwaysOnTop(true);
	}

	void removeGrainGenerator(int indexToRemove) override
	{
		generatorVis.remove(indexToRemove);
	}

	void addAudioBuffer(juce::ValueTree audioSource, juce::ValueTree childOfSource) override
	{
		auto audioBuffer = audioBufferVis.add(std::make_unique<AudioBufferVis>(childOfSource));
		addAndMakeVisible(audioBuffer);
		audioBuffer->setBounds(audioBuffer->calculateBounds().toNearestInt());
		audioBuffer->addMouseListener(groupDragMouseListener.get(), false);
	};

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
			if (clickedGrainGen != 0 && grainGenThatIsDragging != clickedGrainGen)
			{
				createConnectionValueTree(generatorVis.indexOf(grainGenThatIsDragging), generatorVis.indexOf(clickedGrainGen));
			}
			grainGenIsConnectionDragging = false;
			repaint();
		}
	};

	void findLassoItemsInArea(juce::Array<DraggableComponent*>& itemsFound, const juce::Rectangle<int>& area) override
	{
		for (GrainGeneratorVis* genVis : generatorVis)
		{
			auto bounds = genVis->getBounds();
			auto nearestPoint = area.getConstrainedPoint(bounds.getCentre());
			if (genVis->hitTest(nearestPoint.getX() - bounds.getX(), nearestPoint.getY() - bounds.getY()))
			{
				itemsFound.add(genVis);
			}
		}

		for (auto& bufferVis : audioBufferVis)
		{
			if (area.intersects(bufferVis->getBounds()))
			{
				itemsFound.add(bufferVis);
			}
		}
	}

	juce::SelectedItemSet<DraggableComponent*>& getLassoSelection() override
	{
		return groupDragMouseListener->draggableItemSet;
	}

	void changeListenerCallback(juce::ChangeBroadcaster* source) override
	{
		bool menuHasBeenSelected = false;
		auto numSelected = groupDragMouseListener->draggableItemSet.getNumSelected();
		if (numSelected == 0 || numSelected > 1)
		{
			selectedOption->setMenu(MenuOption::Sample);
			menuHasBeenSelected = true;
		}

		int i = 0;
		for (auto gen : generatorVis)
		{
			if (groupDragMouseListener->draggableItemSet.isSelected(gen))
			{
				gen->selected = true;
				if (!menuHasBeenSelected)
				{
					selectedOption->selectedMenuId = i;
					selectedOption->setMenu(MenuOption::GrainMenu);
				}
			}
			else
			{
				gen->selected = false;
			}
			gen->repaint();
			i++;
		}

		for (auto buffer : audioBufferVis)
		{
			if (groupDragMouseListener->draggableItemSet.isSelected(buffer))
			{
				buffer->selected = true;
				if (!menuHasBeenSelected)
				{
					selectedOption->valueTree = buffer->getValueTree();
					selectedOption->setMenu(MenuOption::Buffer);
				}
			}
			else
			{
				buffer->selected = false;
			}
			buffer->repaint();
			i++;
		}
	}
	std::unique_ptr<SelectedOption> selectedOption;
	void addSoundToGrainGenerator(int grainGenIndex, int audioFileIndex, int audioBufferIndex) override {};
	void removeSoundFromGrainGenerator(int grainGenIndex, int audioFileIndex, int audioBufferIndex) override {};
	void addAudioFile(juce::ValueTree newAudioSource) override {};
private:
	juce::ValueTree createGeneratorValueTree(float x, float y)
	{
		juce::ValueTree newTree{ Ids::generator };
		newTree
			.setProperty(Ids::active, true, nullptr)
			.setProperty(Ids::numVoices, 100, nullptr)
			.setProperty(Ids::frequency, 1000.0, nullptr)
			.setProperty(Ids::x, x, nullptr)
			.setProperty(Ids::y, y, nullptr)
			.setProperty(Ids::distance, 0.1, nullptr)
			.setProperty(Ids::jitter, 50000, nullptr);
		genTree.addChild(newTree, -1, nullptr);
		return newTree;
	}

	void createConnectionValueTree(int from, int to)
	{
		juce::ValueTree newTree{ Ids::connection };
		newTree
			.setProperty(Ids::from, from, nullptr)
			.setProperty(Ids::to, to, nullptr)
			.setProperty(Ids::weight, 1, nullptr);
		connectionTree.addChild(newTree, -1, nullptr);
	}

	void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property) override
	{
		if (treeWhosePropertyHasChanged == genTree) 
		{
			if (property == Ids::x || property == Ids::y)
			{
				int indexOfGenVis = genTree.indexOf(treeWhosePropertyHasChanged);
				for (auto child : connectionTree)
				{
					if (indexOfGenVis == (int)child[Ids::from] || indexOfGenVis == (int)child[Ids::to])
					{
						child.sendPropertyChangeMessage(Ids::weight);
					}
				}
			}
			else if (property == Ids::distance)
			{
				repaint();
			}
		}
	}

	juce::Line<int> calculateConnectionLine(const GrainGeneratorVis& from, const GrainGeneratorVis& to)
	{
		auto fromCentre = from.getBounds().getCentre();
		auto toCentre = to.getBounds().getCentre();
		auto fromAngle = fromCentre.getAngleToPoint(toCentre);
		auto toAngle = fromAngle - juce::MathConstants<float>::pi;
		auto fromCentreOnEdge = juce::Line<double>::fromStartAndAngle(fromCentre.toDouble(), 25, fromAngle).getEnd();
		auto toCentreOnEdge = juce::Line<double>::fromStartAndAngle(toCentre.toDouble(), 25, toAngle).getEnd();
		return juce::Line<int>{ fromCentreOnEdge.toInt(), toCentreOnEdge.toInt()};
	}

	juce::OwnedArray<GrainGeneratorVis> generatorVis;
	juce::OwnedArray<AudioBufferVis> audioBufferVis;
	juce::ValueTree genTree, fileTree, connectionTree;
	std::unique_ptr<GenListener> genListener;
	std::unique_ptr<FileListener> fileListener;
	std::unique_ptr<ConnectionDragMouseListener> connectionDragMouseListener;
	std::unique_ptr<GroupDragMouseListener> groupDragMouseListener;
	bool grainGenIsConnectionDragging{ false };
	GrainGeneratorVis* grainGenThatIsDragging{ nullptr };
	juce::LassoComponent<DraggableComponent*> lasso;
};
