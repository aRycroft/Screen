/*
  ==============================================================================

    CPGNetworkHandler.h
    Created: 2 Mar 2021 8:56:52pm
    Author:  Alex

  ==============================================================================
*/

#pragma once
#include "CPGLib-master/CPG.h"
#include <vector>
class CPGNetwork : public CPG, juce::ValueTree::Listener
{
public:
    CPGNetwork(juce::ValueTree grainGenTree, int sampleRate)
        : CPG(sampleRate)
        , generatorTree(grainGenTree)
    {
        this->setNodeFrequency(0, 1000, false);
        previousNodeValue = 0.0;
    }

    void stepAndCheckForTriggeredNodes() 
    {
        if (previousNodeValue < 0.0 && this->getNode(0).getOutput() > 0.0) 
        {
            triggeredNodes.push_back(0);
        }
        previousNodeValue = this->getNode(0).getOutput();
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

    std::vector<unsigned> triggeredNodes;
private:
    double previousNodeValue;
    juce::ValueTree generatorTree;
};
