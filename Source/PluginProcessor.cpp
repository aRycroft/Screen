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
	, paramTree("ParamTree")
	, fileTree(Ids::fileTree)
	, genTree(Ids::genTree)
	, connectionTree(Ids::connectionTree)
	, cpgNetwork(genTree, CPGSAMPLERATE)
{
	formatManager.registerBasicFormats();

	paramTree.addChild(fileTree, TreeChildren::fileTree, nullptr);
	paramTree.addChild(genTree, TreeChildren::genTree, nullptr);
	paramTree.addChild(connectionTree, TreeChildren::connectionTree, nullptr);

	genListener = std::make_unique<GenListener>(this, genTree);
	fileListener = std::make_unique<FileListener>(this, fileTree);
	connectionListener = std::make_unique<ConnectionListener>(this, connectionTree);

	cpgNetwork.generators.ensureStorageAllocated(NUM_NODES);

	//copyValueTreesFromXmlString();
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
		if (counter++ == (int)DUMMYSAMPLERATE / CPGSAMPLERATE)
		{
			cpgNetwork.stepAndCheckForTriggeredNodes();
			counter = 0;
		}
	}

	for (auto files : audioFiles)
	{
		for (auto sound : files->allSounds)
		{
			sound->fillNextBuffer(&buffer);
		}
	}
}

//==============================================================================
bool ScreenAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ScreenAudioProcessor::createEditor()
{
	return new ScreenAudioProcessorEditor(*this, paramTree);
}

//==============================================================================
void ScreenAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	std::unique_ptr<juce::XmlElement> xml(paramTree.createXml());
	copyXmlToBinary(*xml, destData);
}

void ScreenAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	//std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
	//if (xmlState.get() != nullptr)
	//	if (xmlState->hasTagName(paramTree.getType()))
	//		fillValueTreesFromXmlElement(*xmlState);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new ScreenAudioProcessor();
}

void ScreenAudioProcessor::createAudioFileValueTree(juce::File audioFile)
{
	if (!fileTree.getChildWithProperty(Ids::relativePath, audioFile.getFullPathName()).isValid())
	{
		juce::ValueTree newFileValueTree(Ids::audioFile);
		newFileValueTree.setProperty(Ids::relativePath, audioFile.getFullPathName(), nullptr);
		fileTree.addChild(newFileValueTree, fileTree.getNumChildren(), nullptr);
	}
}

void ScreenAudioProcessor::addAudioFile(juce::ValueTree newAudioSource)
{
	auto& filePath = newAudioSource.getProperty(Ids::relativePath);
	juce::File newAudioFile{ filePath };
	if (newAudioFile.existsAsFile())
	{
		std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(newAudioFile));
		auto* newBuffer = audioFiles.add(std::make_unique<AudioFile>());

		if (reader.get() != nullptr)
		{
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

void ScreenAudioProcessor::createAudioBufferValueTree(float x, float y, int lowSample, int highSample, int maxSample, int audioFileTreeId)
{
	auto newTree = ValueTreeHelpers::createAudioBufferValueTree(x, y, lowSample, highSample, maxSample, 100, 100);
	auto parentTree = fileTree.getChild(audioFileTreeId);
	parentTree.addChild(newTree, -1, nullptr);
}

void ScreenAudioProcessor::addAudioBuffer(juce::ValueTree audioSource, juce::ValueTree childOfSource)
{

	auto bufferIndex = fileTree.indexOf(audioSource);
	auto* buffer = audioFiles[bufferIndex];
	if (buffer != nullptr)
	{
		auto audioBuffer = buffer->allSounds.add(std::make_unique<MyAudioBuffer>(childOfSource,
			buffer, childOfSource.getPropertyAsValue(Ids::lowSample, nullptr), childOfSource.getPropertyAsValue(Ids::highSample, nullptr)));
		audioBuffer->initParamTreeValues();
	}
}

void ScreenAudioProcessor::createGrainGeneratorValueTree(float x, float y)
{
	int nextId = Helpers::getNextFreeIndex(cpgNetwork.generators, NUM_NODES);
	if (nextId >= 0)
	{
		auto newTree = ValueTreeHelpers::createGrainGeneratorValueTree(x, y, 1.0, 0.5);
		genTree.addChild(newTree, nextId, nullptr);
	}
}

void ScreenAudioProcessor::createGrainGenerator(juce::ValueTree generatorValueTree)
{
	cpgNetwork.addNode(generatorValueTree);
}

void ScreenAudioProcessor::removeGrainGeneratorValueTree(int indexToRemove)
{
	genTree.removeChild(indexToRemove, nullptr);
}

void ScreenAudioProcessor::removeGrainGenerator(int indexToRemove)
{
	cpgNetwork.generators.remove(indexToRemove);
}

void ScreenAudioProcessor::generatorMoved(juce::ValueTree generatorThatMoved)
{
	for (auto file : fileTree)
	{
		for (auto buffer : file) {
			if (Helpers::isInRange(generatorThatMoved[Ids::distance],
				generatorThatMoved[Ids::x], buffer[Ids::x],
				generatorThatMoved[Ids::y], buffer[Ids::y]))
			{
				addSoundToGrainGenerator(genTree.indexOf(generatorThatMoved),
					fileTree.indexOf(file),
					file.indexOf(buffer));
			}
			else
			{
				removeSoundFromGrainGenerator(genTree.indexOf(generatorThatMoved),
					fileTree.indexOf(file),
					file.indexOf(buffer));
			}
		}
	}
}

void ScreenAudioProcessor::audioBufferMoved(juce::ValueTree bufferThatMoved)
{
	for (auto generator : genTree)
	{
		auto audioFileTree = bufferThatMoved.getParent();
		if (Helpers::isInRange(generator[Ids::distance],
			bufferThatMoved[Ids::x], generator[Ids::x],
			bufferThatMoved[Ids::y], generator[Ids::y]))
		{
			addSoundToGrainGenerator(genTree.indexOf(generator),
				fileTree.indexOf(audioFileTree),
				audioFileTree.indexOf(bufferThatMoved));
		}
		else
		{
			removeSoundFromGrainGenerator(genTree.indexOf(generator),
				fileTree.indexOf(audioFileTree),
				audioFileTree.indexOf(bufferThatMoved));
		}
	}
}

void ScreenAudioProcessor::addSoundToGrainGenerator(int grainGenIndex, int audioFileIndex, int audioBufferIndex)
{
	cpgNetwork.generators[grainGenIndex]->addActiveSound(audioFiles[audioFileIndex]->allSounds[audioBufferIndex]);
}

void ScreenAudioProcessor::removeSoundFromGrainGenerator(int grainGenIndex, int audioFileIndex, int audioBufferIndex)
{
	if (cpgNetwork.generators[grainGenIndex] != nullptr)
	{
		cpgNetwork.generators[grainGenIndex]->removeSound(audioFiles[audioFileIndex]->allSounds[audioBufferIndex]);
	}
}

void ScreenAudioProcessor::createConnectionValueTree(int from, int to)
{
	connectionTree.addChild(ValueTreeHelpers::createConnectionValueTree(from, to, 1.0f), -1, nullptr);
}

void ScreenAudioProcessor::removeConnectionValueTree(int from, int to)
{
	for (auto child : connectionTree)
	{
		if ((int)child[Ids::from] == from && (int)child[Ids::to] == to)
		{
			connectionTree.removeChild(child, nullptr);
			return;
		}
	}
}

void ScreenAudioProcessor::connectionCreated(int from, int to)
{
	if (from != to)
	{
		cpgNetwork.setConnection(from, to, 1);
	}
}

void ScreenAudioProcessor::connectionRemoved(int from, int to)
{
	if (from != to)
	{
		cpgNetwork.removeConnection(from, to);
	}
}

void ScreenAudioProcessor::setFrequency(int nodeId, float frequency)
{
	cpgNetwork.setNodeFrequency(nodeId, frequency, false);
}

void ScreenAudioProcessor::setConnectionWeights(int generatorThatMoved)
{
	for (auto connection : connectionTree)
	{
		if ((int)connection[Ids::from] == generatorThatMoved || (int)connection[Ids::to] == generatorThatMoved)
		{
			double mult = connection[Ids::weight];

			double fromX = genTree.getChild(connection[Ids::from])[Ids::x];
			double fromY = genTree.getChild(connection[Ids::from])[Ids::y];
			double toX = genTree.getChild(connection[Ids::to])[Ids::y];
			double toY = genTree.getChild(connection[Ids::to])[Ids::y];

			double weight = sqrt(pow(toX - fromX, 2) + pow(toY - fromY, 2) * 1.0f);
			cpgNetwork.setConnection((int) connection[Ids::from], (int) connection[Ids::to], weight);
		}
	}
}

void ScreenAudioProcessor::copyValueTreesFromXmlString()
{
	juce::File testTree("C:\\Users\\Alex\\Documents\\GitHub\\Screen\\Source\\testTree.txt");
	if (testTree.existsAsFile())
	{
		fillValueTreesFromXmlElement(*juce::parseXML(testTree));
	}
}

void ScreenAudioProcessor::fillValueTreesFromXmlElement(const juce::XmlElement& xmlElement)
{
	auto newTree = paramTree.fromXml(xmlElement.toString());

	for (auto child : newTree.getChild(TreeChildren::genTree))
	{
		genTree.addChild(child.createCopy(), genTree.getNumChildren(), nullptr);
	}

	for (auto child : newTree.getChild(TreeChildren::fileTree))
	{
		auto fileChildWithoutChildren = child.createCopy();
		fileChildWithoutChildren.removeAllChildren(nullptr);
		fileTree.addChild(fileChildWithoutChildren.createCopy(), fileTree.getNumChildren(), nullptr);
		for (auto buffer : child)
		{
			fileTree.getChild(newTree.getChild(TreeChildren::fileTree).indexOf(child)).addChild(buffer.createCopy(), -1, nullptr);
		}
	}

	for (auto child : newTree.getChild(TreeChildren::connectionTree))
	{
		connectionTree.addChild(child.createCopy(), connectionTree.getNumChildren(), nullptr);
	}
}

