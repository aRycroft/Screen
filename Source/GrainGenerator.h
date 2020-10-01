/*
  ==============================================================================

    GrainGenerator.h
    Created: 12 Sep 2020 5:35:53pm
    Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Grain.h"
#include <vector>

class GrainGenerator
{
public:
    GrainGenerator(int sampleRate, std::atomic<float>* grainSizeMS)
        :activeParamListener(this),
        grainVoiceParamListener(this)
    {
        this->grainSize = grainSizeMS;
        this->sampleRate = sampleRate;
    }

    void fillNextBuffer(juce::AudioBuffer<float>* outputBuffer)
    {
        for (auto grain : grains) {
            if (grain->isPlaying()) {
                grain->fillNextBuffer(outputBuffer);
            }
        }
    }

    void playGrain()
    {
        for (auto grain : grains) {
            if (!grain->isPlaying()) {
                grain->startPlaying(0, (int)(*grainSize / 1000 * sampleRate));
                return;
            }
        }
    }

    /*Based on parameters set in valuetree*/
    void addGrainVoice()
    {
        grains.push_back(new Grain{ &activeSounds, 0, 0 });
        numVoices++;
    }

    void removeGrainVoice()
    {
        if (!grains.empty()) 
        {
            grains.pop_back();
            numVoices--;
        }
    }

    void addActiveSound(AudioFile* newAudioFile)
    {
        if (newAudioFile != nullptr) {
            activeSounds.add(newAudioFile);
        }
    }

    void removeSound(AudioFile* soundToRemove)
    {
        if (soundToRemove != nullptr) {
            activeSounds.removeAllInstancesOf(soundToRemove);
        }
    }

    int getNumVoices() 
    {
        return numVoices;
    }

    juce::AudioProcessorValueTreeState::Listener* getActiveParamListener()
    {
        return &activeParamListener;
    }

    juce::AudioProcessorValueTreeState::Listener* getGrainVoiceParamListener()
    {
        return &grainVoiceParamListener;
    }

    bool isActive{ false };

private:
    class ActiveParamListener : public juce::AudioProcessorValueTreeState::Listener
    {
    public:
        ActiveParamListener(GrainGenerator* processor)
        {
            proc = processor;
        };

        void parameterChanged(const juce::String& parameterID, float newValue) override
        {
            proc->isActive = newValue;
        }

    private:
        GrainGenerator* proc;
    };

    class GrainVoiceParamListener : public juce::AudioProcessorValueTreeState::Listener
    {
    public:
        GrainVoiceParamListener(GrainGenerator* gen)
        {
            generator = gen;
        };

        void parameterChanged(const juce::String& parameterID, float newValue) override
        {
            int targetNumberofVoices = (int)newValue;
            int currentNumberofVoices = generator->getNumVoices();

            while (currentNumberofVoices < targetNumberofVoices)
            {
                generator->addGrainVoice();
                currentNumberofVoices++;
            }
            while (currentNumberofVoices > targetNumberofVoices)
            {
                generator->removeGrainVoice();
                currentNumberofVoices--;
            }
        }
    private:
        GrainGenerator* generator;
    };

    juce::Array<AudioFile*> activeSounds;
    std::vector<Grain*> grains;
    std::atomic<float>* grainSize;
    ActiveParamListener activeParamListener;
    GrainVoiceParamListener grainVoiceParamListener;
    int sampleRate, numVoices = 0;
};

