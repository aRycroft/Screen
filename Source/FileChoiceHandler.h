/*
  ==============================================================================

    FileChoiceHandler.h
    Created: 1 Oct 2020 7:45:10pm
    Author:  Alex

  ==============================================================================
*/
#include <JuceHeader.h>
#include "Utils.h"
#pragma once

class FileChoiceHandler
{
public:
    FileChoiceHandler(juce::ValueTree fileTree)
        : vTree(fileTree) { }

    void loadSoundFileToMemory(const char* fileTitle, const char* relativeFilePath)
    {
        juce::ValueTree newFileValueTree(juce::Identifier{ fileTitle });
        newFileValueTree.setProperty(Ids::relativePath, relativeFilePath, nullptr);
        vTree.addChild(newFileValueTree, vTree.getNumChildren(), nullptr);
    }
private:
    juce::ValueTree vTree;
};
