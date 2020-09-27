/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once
#define NUM_NODES 4
#define DUMMYSAMPLERATE 44100

#include <JuceHeader.h>
#include <vector>
#include "Grain.h"
#include "GrainGenerator.h"
#include "Utils.h"
#include "AudioFile.h"

//==============================================================================
/**
*/
class ScreenAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    ScreenAudioProcessor();
    ~ScreenAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createStateLayout();

    class FileListener : public juce::ValueTree::Listener
    {
    public:
        FileListener(ScreenAudioProcessor* processor, juce::ValueTree tree) 
            : vTree(tree)
        {
            proc = processor;
        };

        void valueTreeChildAdded(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenAdded) override {

        };

        void valueTreeChildRemoved(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override {

        };
    private:
        ScreenAudioProcessor* proc;
        juce::ValueTree vTree;
    };

    class SourceListener : public juce::ValueTree::Listener
    {
    public:
        SourceListener(ScreenAudioProcessor* processor, juce::ValueTree tree)
        : vTree(tree)
        {
            proc = processor;
            vTree.addListener(this);
        };

        void valueTreeChildAdded(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenAdded) override {
        };

        void valueTreeChildRemoved(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override {
        };
    private:
        ScreenAudioProcessor* proc;
        juce::ValueTree vTree;
    };

    class ActiveParamListener : public juce::AudioProcessorValueTreeState::Listener
    {
    public:
        ActiveParamListener(ScreenAudioProcessor* processor)
        {
            proc = processor;
        };

        void parameterChanged(const juce::String& parameterID, float newValue) override {
            DBG("We Here");
        }
    private:
        ScreenAudioProcessor* proc;
    };



    juce::AudioProcessorValueTreeState apvts;
    juce::OwnedArray<GrainGenerator> generators;
    juce::OwnedArray<AudioFile> allSounds;
    juce::ValueTree sourceTree{ Ids::sourceTree }, fileTree;
    std::unique_ptr<SourceListener> sourceListener;
    std::unique_ptr<FileListener> fileListener;
    std::unique_ptr<ActiveParamListener> activeParamListener;

    









    /*TestData*/
    int counter{ 0 };


    class PluginProcessorTests : private juce::UnitTest
    {
    public:
        PluginProcessorTests(ScreenAudioProcessor* proc) :
            juce::UnitTest("Processor Testing")
        {
            processor = proc;
        }

        void runTest() override
        {
            beginTest("Basic");
            expectEquals<juce::String>(processor->getName(), "Screen", "Names are not equal");
            expectEquals<int>(processor->getNumOutputChannels(), 2, "Output is not stereo");
            expectEquals<int>(processor->getNumInputChannels(), 0, "System has no input channels");
        }
    private:
        ScreenAudioProcessor* processor;
    };

    class GrainTests : private juce::UnitTest
    {
    public:
        GrainTests() : juce::UnitTest("Grain Testing") {}

        void runTest() override
        {
            beginTest("Basic");
            int bufferSize = 100;
            float maxFloatError = 0.001f;
            AudioFile sampleBuffer{ juce::AudioBuffer<float>{ 2, bufferSize }, 0, 100 };
            juce::AudioBuffer<float> outputBuffer{ 2, bufferSize };
            sampleBuffer.fillBufferWithValue(1.0f);
            outputBuffer.clear();
            activeSounds.add(&sampleBuffer);
            std::unique_ptr<Grain> g{ new Grain{ &activeSounds, 0, 100 } };
            g->fillNextBuffer(&outputBuffer);
            for (int channel{ 0 }; channel < sampleBuffer.getNumChannels(); channel++) {
                for (int i{ 0 }; i < bufferSize; i++) {
                    expectWithinAbsoluteError(outputBuffer.getSample(channel, i), 1.0f, maxFloatError, "Basic Adding 1");
                }
            }
            outputBuffer.clear();
            std::unique_ptr<Grain> g2{ new Grain{ &activeSounds, 0, 50 } };
            g2->fillNextBuffer(&outputBuffer);
            for (int channel{ 0 }; channel < sampleBuffer.getNumChannels(); channel++) {
                for (int i{ 0 }; i < bufferSize; i++) {
                    if (i < 50) {
                        expectWithinAbsoluteError(outputBuffer.getSample(channel, i), 1.0f, maxFloatError, "Should only fill the first 50 with 1");
                    }
                    else {
                        expectWithinAbsoluteError(outputBuffer.getSample(channel, i), 0.0f, maxFloatError, "Should only fill the first 50 with 1");
                    }
                }
            }
            outputBuffer.clear();
            std::unique_ptr<Grain> g3{ new Grain{ &activeSounds, 0, 50 } };
            std::unique_ptr<Grain> g4{ new Grain{ &activeSounds, 0, 100 } };
            g3->fillNextBuffer(&outputBuffer);
            g4->fillNextBuffer(&outputBuffer);
            for (int channel{ 0 }; channel < sampleBuffer.getNumChannels(); channel++) {
                for (int i{ 0 }; i < bufferSize; i++) {
                    if (i < 50) {
                        expectWithinAbsoluteError(outputBuffer.getSample(channel, i), 2.0f, maxFloatError, "Values not copied");
                    }
                    else {
                        expectWithinAbsoluteError(outputBuffer.getSample(channel, i), 1.0f, maxFloatError, "Values not copied");
                    }
                }
            }
        }
        juce::Array<AudioFile*> activeSounds;
        std::vector<int> activeSoundIndices;
    };

    class GeneratorTests : private juce::UnitTest
    {
    public:
        GeneratorTests(juce::AudioProcessorValueTreeState* state) : juce::UnitTest("Generator Testing")
        {
            this->state = state;
        }
        void runTest() override 
        {
            beginTest("Basic");
            int bufferSize = 100;
            float maxFloatError = 0.001f;
            AudioFile sampleBuffer{ juce::AudioBuffer<float>{ 2, bufferSize }, 0, 100 };
            juce::AudioBuffer<float> outputBuffer{ 2, bufferSize };
            GrainGenerator gen{ DUMMYSAMPLERATE, 20, state->getRawParameterValue("size0")};
            sampleBuffer.fillBufferWithValue(1.0f);
            outputBuffer.clear();
            activeSounds.add(&sampleBuffer);
            gen.addActiveSound(&sampleBuffer);
            gen.playGrain();
            gen.fillNextBuffer(&outputBuffer);
            for (int channel{ 0 }; channel < sampleBuffer.getNumChannels(); channel++) {
                for (int i{ 0 }; i < bufferSize; i++) {
                    expectWithinAbsoluteError(outputBuffer.getSample(channel, i), 1.0f, maxFloatError, "Values not copied");
                }
            }
        }
    private:
        juce::Array<AudioFile*> activeSounds;
        juce::AudioProcessorValueTreeState* state;
    };
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScreenAudioProcessor)
};
