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
class CPGNetwork : public CPG
{
public:
    CPGNetwork(int sampleRate)
        : CPG(sampleRate)
    {
        this->setNodeFrequency(0, 100, false);
        previousNodeValue.push_back(0.0);
    }

    void stepAndCheckForTriggeredNodes() 
    {
        if (previousNodeValue[0] < 0.0 && this->getNode(0).getOutput() > 0.0) 
        {
            triggeredNodes.push_back(0);
        }
        previousNodeValue[0] = this->getNode(0).getOutput();
        this->step();
    }

    std::vector<unsigned> triggeredNodes;
private:
    std::vector<double> previousNodeValue;
};
