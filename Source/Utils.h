/*
  ==============================================================================

    Utils.h
    Created: 11 Sep 2020 9:13:03pm
    Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
constexpr int DEFAULTSIZE = 60;

enum TreeChildren {
    fileTree,
    genTree,
    connectionTree
};

namespace Ids
{
    /*AUDIO SOURCE*/
    static juce::Identifier audioFile("audioFile");
    static juce::Identifier audioBuffer("audioBuffer");
    static juce::Identifier lowSample("lowSample");
    static juce::Identifier highSample("highSample");
    static juce::Identifier maxSample("maxSample");
    /*FILE*/
    static juce::Identifier fileTree("fileTree");
    static juce::Identifier relativePath("relativePath");
    /*GENERATOR*/
    static juce::Identifier genTree("genTree");
    static juce::Identifier generator("generator");
    static juce::Identifier frequency("frequency");
    static juce::Identifier numVoices("numVoices");
    static juce::Identifier distance("distance");
    static juce::Identifier jitter("jitter");
    /*CONNECTION*/
    static juce::Identifier connectionTree("connectionTree");
    static juce::Identifier connection("connection");
    static juce::Identifier from("from");
    static juce::Identifier to("to");
    static juce::Identifier weight("weight");
    /*SHARED*/
    static juce::Identifier active("active");
    static juce::Identifier x("x");
    static juce::Identifier y("y");
}

