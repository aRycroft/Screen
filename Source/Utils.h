/*
  ==============================================================================

    Utils.h
    Created: 11 Sep 2020 9:13:03pm
    Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
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
    /*FILE*/
    static juce::Identifier fileTree("fileTree");
    static juce::Identifier relativePath("relativePath");
    /*GENERATOR*/
    static juce::Identifier genTree("genTree");
    static juce::Identifier generator("generator");
    static juce::Identifier frequency("frequency");
    static juce::Identifier numVoices("numVoices");
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


namespace util 
{
    static juce::Point<float> getPointOnEdge(juce::Component& component)
    {
        auto mousePoint = component.getMouseXYRelative().toFloat();
        auto centre = component.getLocalBounds().getCentre().toFloat();
        auto radius = component.getLocalBounds().getWidth() / 2.0 - 7.5;
        auto angle = centre.getAngleToPoint(mousePoint);
        auto line = juce::Line<float>::fromStartAndAngle(centre, radius, angle);
        return line.getEnd();
    }
}

