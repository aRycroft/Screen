/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once
#define NUM_NODES 4
#define DUMMYSAMPLERATE 48000
#define CPGSAMPLERATE 10000

#include <JuceHeader.h>
#include <vector>
#include "Grain.h"
#include "GrainGenerator.h"
#include "Utils.h"
#include "AudioBuffer.h"
#include "AudioFile.h"
#include "IGrainGenHandler.h"
#include "IAudioFileHandler.h"
#include "GenListener.h"
#include "FileListener.h"
#include "PositionListener.h"
#include "CPGNetwork.h"

class ScreenAudioProcessor : public juce::AudioProcessor,
							 public IGrainGenHandler,
							 public IAudioFileHandler
{
public:
	ScreenAudioProcessor();
	~ScreenAudioProcessor() override;
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

	void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

	juce::AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;
	const juce::String getName() const override;
	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	double getTailLengthSeconds() const override;
	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const juce::String getProgramName(int index) override;
	void changeProgramName(int index, const juce::String& newName) override;
	void getStateInformation(juce::MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

	void createGrainGenerator(juce::ValueTree generatorValueTree) override;
	void removeGrainGenerator(int indexToRemove) override;
	void addSoundToGrainGenerator(int grainGenIndex, int audioFileIndex, int audioBufferIndex) override;
	void removeSoundFromGrainGenerator(int grainGenIndex, int audioFileIndex, int audioBufferIndex) override;
private:
	void addAudioFile(juce::ValueTree newAudioSource);
	void addAudioBuffer(juce::ValueTree audioSource, juce::ValueTree childOfSource);
	void removeAudioFile(juce::File newFile);

	CPGNetwork cpgNetwork;
	juce::ValueTree vTree;
	juce::ValueTree fileTree;
	juce::ValueTree genTree;
	juce::OwnedArray<GrainGenerator> generators;
	juce::OwnedArray<MyAudioBuffer> allSounds;
	juce::OwnedArray<AudioFile> fileBuffers;
	std::unique_ptr<FileListener> fileListener;
	std::unique_ptr<GenListener> genListener;
	std::unique_ptr<PositionListener> positionListener;
	juce::AudioFormatManager formatManager;
	unsigned counter = 0;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScreenAudioProcessor)
};
