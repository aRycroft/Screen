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

class MainPanel : public juce::Component, public juce::ValueTree::Listener, public juce::MouseListener
{
public:
	MainPanel(juce::ValueTree state)
		:genTree(state.getChildWithName(Ids::genTree)),
		fileTree(state.getChildWithName(Ids::fileTree))
	{
		genListener.reset(new GenListener(this, genTree));
		for (auto generator : genTree)
		{
			addGeneratorVis(generator);
		}
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

	void addGeneratorVis(juce::ValueTree valueTree)
	{
		generatorVis.add(new GrainGeneratorVis(valueTree));
		addAndMakeVisible(generatorVis.getLast());
		generatorVis.getLast()->setBounds(generatorVis.getLast()->calculateBounds().toNearestInt());
		generatorVis.getLast()->addMouseListener(this, false);
	}

	void removeGeneratorVis(int indexFromWhichChildWasRemoved)
	{
		generatorVis.remove(indexFromWhichChildWasRemoved);
		//removeChildComponent(indexFromWhichChildWasRemoved);
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

	class GenListener : public juce::ValueTree::Listener
	{
	public:
		GenListener(MainPanel* panel, juce::ValueTree tree)
			: vTree(tree)
		{
			mainPanel = panel;
			vTree.addListener(this);
		};

		void valueTreeChildAdded(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenAdded) override
		{
			mainPanel->addGeneratorVis(childWhichHasBeenAdded);
		};

		void valueTreeChildRemoved(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override
		{
			mainPanel->removeGeneratorVis(indexFromWhichChildWasRemoved);
		};
	private:
		MainPanel* mainPanel;
		juce::ValueTree vTree;
	};

	class FileListener : public juce::ValueTree::Listener
	{
	public:
		FileListener(MainPanel* panel, juce::ValueTree tree)
			: vTree(tree)
		{
			mainPanel = panel;
			vTree.addListener(this);
		};

		void valueTreeChildAdded(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenAdded) override
		{
			DBG("Hell");
		};
	private:
		MainPanel* mainPanel;
		juce::ValueTree vTree;
	};

	juce::OwnedArray<GrainGeneratorVis> generatorVis;
	juce::ValueTree genTree, fileTree;
	std::unique_ptr<GenListener> genListener;
	std::unique_ptr<FileListener> fileListener;
};
