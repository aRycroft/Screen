/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ScreenAudioProcessor::ScreenAudioProcessor()
	:
#ifndef JucePlugin_PreferredChannelConfigurations
	AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	)
#endif
	, vTree("ParamTree")
	, fileTree(Ids::fileTree)
	, genTree(Ids::genTree)
	, connectionTree(Ids::connectionTree)
	,cpgNetwork(genTree, CPGSAMPLERATE)
{
	formatManager.registerBasicFormats();

	vTree.addChild(fileTree, TreeChildren::fileTree, nullptr);
	vTree.addChild(genTree, TreeChildren::genTree, nullptr);
	vTree.addChild(connectionTree, TreeChildren::connectionTree, nullptr);

	fileListener.reset(new FileListener(this, fileTree));
	genListener.reset(new GenListener(this, genTree));
	positionListener.reset(new PositionListener(this, genTree, fileTree));
	connectionListener.reset(new ConnectionListener(this, connectionTree));
}

ScreenAudioProcessor::~ScreenAudioProcessor()
{
}

//==============================================================================
const juce::String ScreenAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool ScreenAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool ScreenAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool ScreenAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double ScreenAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int ScreenAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
				// so this should be at least 1, even if you're not really implementing programs.
}

int ScreenAudioProcessor::getCurrentProgram()
{
	return 0;
}

void ScreenAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String ScreenAudioProcessor::getProgramName(int index)
{
	return {};
}

void ScreenAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void ScreenAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
}

void ScreenAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ScreenAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	juce::ignoreUnused(layouts);
	return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
		return false;

	// This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}
#endif

void ScreenAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	buffer.clear();
	for (int i{ 0 }; i < buffer.getNumSamples(); i++) 
	{
		if (counter++ % CPGSAMPLERATE == 0) {
			cpgNetwork.stepAndCheckForTriggeredNodes();
		}
	}

	for (unsigned triggeredNode : cpgNetwork.triggeredNodes) 
	{
		auto generator = generators[triggeredNode];
		if(generator != nullptr)
		{
			generator->playGrain();
		}
	}

	for (auto grainGen : generators) 
	{
		grainGen->fillNextBuffer(&buffer);
	}

	cpgNetwork.triggeredNodes.clear();
	/*for (auto grainGen : generators)
	{
		if (grainGen->shouldPlayGrain()) {
			grainGen->playGrain();
		}
		grainGen->fillNextBuffer(&buffer);
	}
	if (counter % 500 == 0) {
		DBG(vTree.toXmlString());
	}*/
}

//==============================================================================
bool ScreenAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ScreenAudioProcessor::createEditor()
{
	return new ScreenAudioProcessorEditor(*this, vTree);
}

//==============================================================================
void ScreenAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void ScreenAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new ScreenAudioProcessor();
}

void ScreenAudioProcessor::addAudioFile(juce::ValueTree newAudioSource)
{
	auto filePath = newAudioSource.getProperty(Ids::relativePath);
	juce::File newAudioFile{ filePath };
	if (newAudioFile.existsAsFile()) 
	{
		std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(newAudioFile));
		auto* newBuffer = fileBuffers.add(new AudioFile());

		if (reader.get() != nullptr) {
			newBuffer->setSize((int)reader->numChannels, (int)reader->lengthInSamples);
			reader->read(newBuffer,
				0,
				(int)reader->lengthInSamples,
				0,
				true,
				true);
		}
	}
}

void ScreenAudioProcessor::addAudioBuffer(juce::ValueTree audioSource, juce::ValueTree childOfSource)
{
	int bufferIndex = fileTree.indexOf(audioSource);
	auto* buffer = fileBuffers[bufferIndex];
	if (buffer != nullptr) {
		buffer->allSounds.add(new MyAudioBuffer( buffer, childOfSource[Ids::lowSample], childOfSource[Ids::highSample]));
	}
}

void ScreenAudioProcessor::createGrainGenerator(juce::ValueTree generatorValueTree) 
{
	auto generator = generators.add(new GrainGenerator{ DUMMYSAMPLERATE, generatorValueTree });
	generator->initParamTreeValues();
	cpgNetwork.addNode(generators.size());
	cpgNetwork.setNodeFrequency(generators.size(), 1000.0, false);
}

void ScreenAudioProcessor::removeGrainGenerator(int indexToRemove) 
{
	generators.remove(indexToRemove);
}

void ScreenAudioProcessor::addSoundToGrainGenerator(int grainGenIndex, int audioFileIndex, int audioBufferIndex)
{
	generators[grainGenIndex]->addActiveSound(fileBuffers[audioFileIndex]->allSounds[audioBufferIndex]);
}

void ScreenAudioProcessor::removeSoundFromGrainGenerator(int grainGenIndex, int audioFileIndex, int audioBufferIndex)
{
	generators[grainGenIndex]->removeSound(fileBuffers[audioFileIndex]->allSounds[audioBufferIndex]);
}

void ScreenAudioProcessor::connectionCreated(int from, int to) 
{
	DBG("OIOIO");
}
void ScreenAudioProcessor::connectionRemoved(int from, int to) 
{
	DBG("OIOIO");
}

