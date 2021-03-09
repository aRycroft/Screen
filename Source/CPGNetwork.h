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
        : CPG(sampleRate),
        generatorTree(grainGenTree)
    {
        this->setNodeFrequency(0, 1000, false);
        previousNodeValues.push_back(0.0);
    }

    void stepAndCheckForTriggeredNodes() 
    {
        for (auto node : this->getNodeList()) 
        {
            if (previousNodeValues[node] < 0.0 && this->getNode(node).getOutput() > 0.0)
            {
                triggeredNodes.push_back(node);
            }
            previousNodeValues[node] = this->getNode(node).getOutput();
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
        previousNodeValues.push_back(0.0);
        this->CPG::addNode(id);
    }

    std::vector<unsigned> triggeredNodes;
private:
    std::vector<double> previousNodeValues;
    juce::ValueTree generatorTree;
};
