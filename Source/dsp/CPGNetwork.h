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
        //this->setNodeFrequency(0, 1000, false);
        generatorTree.addListener(this);
    }

    void stepAndCheckForTriggeredNodes() 
    {
        for (auto node : this->getNodeList()) 
        {
            if (this->getNode(node).getSignalState() == MatsuNode::signalState::zeroXup)
            {
                triggeredNodes.push_back(node);
            }
        }
        this->step();
    }

    void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
    {
        auto nodeId = generatorTree.indexOf(treeWhosePropertyHasChanged);
        if (property == Ids::frequency) 
        {
            this->setNodeFrequency(nodeId, treeWhosePropertyHasChanged[property], true);
        }
    }

    void addNode(int id) 
    {
        this->CPG::addNode(id);
    }

    std::vector<unsigned> triggeredNodes;
private:
    juce::ValueTree generatorTree;
};
