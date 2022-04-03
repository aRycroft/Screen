/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once
constexpr auto NUM_NODES = 16;
constexpr auto DUMMYSAMPLERATE = 48000;
constexpr auto CPGSAMPLERATE = 10000;

#include <JuceHeader.h>
#include <vector>
#include "Grain.h"
#include "GrainGenerator.h"
#include "AudioBuffer.h"
#include "AudioFile.h"
#include "IGrainGenHandler.h"
#include "IAudioFileHandler.h"
#include "IConnectionHandler.h"
#include "GenListener.h"
#include "FileListener.h"
#include "PositionListener.h"
#include "ConnectionListener.h"
#include "ConnectionChangeListener.h"
#include "CPGNetwork.h"

class ScreenAudioProcessor : public juce::AudioProcessor,
							 public IGrainGenHandler,
							 public IAudioFileHandler,
							 public IConnectionHandler
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

	void createGrainGeneratorValueTree(float x, float y);
	void removeGrainGeneratorValueTree(int indexToRemove);
	
	void createGrainGenerator(juce::ValueTree generatorValueTree) override;
	void removeGrainGenerator(int indexToRemove) override;

	void addSoundToGrainGenerator(int grainGenIndex, int audioFileIndex, int audioBufferIndex) override;
	void removeSoundFromGrainGenerator(int grainGenIndex, int audioFileIndex, int audioBufferIndex) override;

	void createConnectionValueTree(int from, int to);
	void removeConnectionValueTree(int from, int to);

	void connectionCreated(int from, int to) override;
	void connectionRemoved(int from, int to) override;

	void createAudioFileValueTree(juce::File audioFile);
	void addAudioFile(juce::ValueTree newAudioSource) override;

	void createAudioBufferValueTree(float x, float y, int lowSample, int highSample, int maxSample, int audioFileTreeId);

	void addAudioBuffer(juce::ValueTree audioSource, juce::ValueTree childOfSource) override;

	void connectionWeightChanged(int from, int to, float weight) override;
private:


	void copyValueTreesFromXmlString();
	void fillValueTreesFromXmlElement(const juce::XmlElement& xmlElement);

	juce::ValueTree paramTree;
	juce::ValueTree fileTree;
	juce::ValueTree genTree;
	juce::ValueTree connectionTree;

	CPGNetwork cpgNetwork;
	juce::OwnedArray<GrainGenerator> generators;
	juce::OwnedArray<AudioFile> fileBuffers;

	std::unique_ptr<FileListener> fileListener;
	std::unique_ptr<GenListener> genListener;
	std::unique_ptr<PositionListener> positionListener;
	std::unique_ptr<ConnectionListener> connectionListener;
	std::unique_ptr<ConnectionChangeListener> connectionChangeListener;

	juce::AudioFormatManager formatManager;
	unsigned counter = 0;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScreenAudioProcessor)
};
