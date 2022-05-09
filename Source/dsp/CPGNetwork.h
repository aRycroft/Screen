/*
  ==============================================================================

	CPGNetworkHandler.h
	Created: 2 Mar 2021 8:56:52pm
	Author:  Alex

  ==============================================================================
*/

#pragma once
#include "CPG.h"
#include <vector>
class CPGNetwork : public CPG, juce::ValueTree::Listener
{
public:
	CPGNetwork(juce::ValueTree grainGenTree, int sampleRate)
		: CPG(sampleRate),
		generatorTree(grainGenTree)
	{
		generatorTree.addListener(this);
	}

	void stepAndCheckForTriggeredNodes()
	{
		for (auto node : this->getNodeList())
		{
			if (this->getNode(node).getSignalState() == MatsuNode::signalState::zeroXup)
			{
				auto generator = generators[node];
				if (generator != nullptr)
				{
					for (auto sound : generator->getActiveSounds())
					{
						sound->playGrain();
					}
				}
			}
		}
		this->step();
	}

	void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
	{
		auto nodeId = generatorTree.indexOf(treeWhosePropertyHasChanged);
		if (property == Ids::frequency)
		{
			this->setNodeFrequency(nodeId, (int)treeWhosePropertyHasChanged[property], true);
		}
	}

	void addNode(juce::ValueTree generatorValueTree)
	{
		int id = generatorTree.indexOf(generatorValueTree);
		auto generator = generators.insert(id, std::make_unique<GrainGenerator>(generatorValueTree));
		CPG::addNode(id);
		setNodeFrequency(id, 1.0, false);
		generator->initParamTreeValues();
	}
	juce::OwnedArray<GrainGenerator> generators;
private:
	juce::ValueTree generatorTree;
};
