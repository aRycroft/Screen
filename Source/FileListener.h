/*
  ==============================================================================

    FileListener.h
    Created: 6 Dec 2020 4:31:31pm
    Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "IAudioFileHandler.h"
#include "AudioFile.h"
class FileListener : public juce::ValueTree::Listener
{
public:
	FileListener(IAudioFileHandler* processor, juce::ValueTree tree)
		: vTree(tree)
	{
		proc = processor;
		vTree.addListener(this);
		formatManager.registerBasicFormats();
	};

	void valueTreeChildAdded(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenAdded) override
	{
		if (parentTree == vTree) {
			auto filePath = childWhichHasBeenAdded.getProperty(Ids::relativePath);
			juce::File newAudioFile{ filePath };
			if (newAudioFile.existsAsFile()) {
				std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(newAudioFile));
				juce::AudioBuffer<float> newBuffer;
				if (reader.get() != nullptr) {
					auto duration = (float)reader->lengthInSamples / reader->sampleRate;
					newBuffer.setSize((int)reader->numChannels, (int)reader->lengthInSamples);
					reader->read(&newBuffer,
						0,
						(int)reader->lengthInSamples,
						0,
						true,
						true);
				}
				proc->addAudioBuffer(newBuffer);
			}
		}
		/*AudioFile Child tree added*/
		else if (parentTree.isAChildOf(vTree)) {
			int bufferIndex = vTree.indexOf(parentTree);
			auto* buffer = proc->getAudioSampleBuffer(bufferIndex);
			if (buffer != nullptr) {
				AudioFile newAudioFile{ buffer, childWhichHasBeenAdded[Ids::lowSample], childWhichHasBeenAdded[Ids::highSample] };
				proc->addAudioFile(newAudioFile);
			}
		}
	};
private:
	juce::AudioFormatManager formatManager;
	IAudioFileHandler* proc;
	juce::ValueTree vTree;
};
