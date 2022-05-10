/*
  ==============================================================================

    Utils.h
    Created: 11 Sep 2020 9:13:03pm
    Author:  Alex

  ==============================================================================
*/

#pragma once
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
    static juce::Identifier jitter("jitter");
    static juce::Identifier numVoices("numVoices");
    /*FILE*/
    static juce::Identifier fileTree("fileTree");
    static juce::Identifier relativePath("relativePath");
    /*GENERATOR*/
    static juce::Identifier genTree("genTree");
    static juce::Identifier generator("generator");
    static juce::Identifier frequency("frequency");
    static juce::Identifier distance("distance");
    /*CONNECTION*/
    static juce::Identifier connectionTree("connectionTree");
    static juce::Identifier connection("connection");
    static juce::Identifier from("from");
    static juce::Identifier to("to");
    static juce::Identifier weight("weight");
    /*SHARED*/
    static juce::Identifier x("x");
    static juce::Identifier y("y");
}

namespace Helpers
{
    template <class T>
    static int getNextFreeIndex(juce::OwnedArray<T>& arr, int size)
    {
        
        for (int i{ 0 }; i < size; i++)
        {
            if (arr[i] == nullptr)
            {
                return i;
            }
        }
        return -1;
    }

    static bool isInRange(float distance, float x1, float x2, float y1, float y2)
    {
        return std::abs(x1 - x2) < distance && std::abs(y1 - y2) < distance;
    }
}

namespace ValueTreeHelpers
{
    static juce::ValueTree createAudioBufferValueTree(float x, float y, int lowSample, int highSample, int maxSample, int numVoices, int jitter)
    {
        juce::ValueTree newTree{ Ids::audioBuffer };
        newTree
            .setProperty(Ids::x, x, nullptr)
            .setProperty(Ids::y, y, nullptr)
            .setProperty(Ids::lowSample, lowSample, nullptr)
            .setProperty(Ids::highSample, highSample, nullptr)
            .setProperty(Ids::jitter, jitter, nullptr)
            .setProperty(Ids::numVoices, numVoices, nullptr)
            .setProperty(Ids::maxSample, maxSample, nullptr);
        return newTree;
    }

    static juce::ValueTree createGrainGeneratorValueTree(float x, float y, float frequency, float distance)
    {
        juce::ValueTree newTree{ Ids::generator };
        newTree
            .setProperty(Ids::frequency, frequency, nullptr)
            .setProperty(Ids::x, x, nullptr)
            .setProperty(Ids::y, y, nullptr)
            .setProperty(Ids::distance, distance, nullptr);
        return newTree;
    }

    static juce::ValueTree createConnectionValueTree(int from, int to, float weight)
    {
        juce::ValueTree newTree{ Ids::connection };
        newTree
            .setProperty(Ids::from, from, nullptr)
            .setProperty(Ids::to, to, nullptr)
            .setProperty(Ids::weight, weight, nullptr);
        return newTree;
    }
}
