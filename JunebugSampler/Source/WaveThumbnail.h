/*
  ==============================================================================

    WaveThumbnail.h
    Created: 7 Sep 2020 12:58:20pm
    Author:  saybe

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class WaveThumbnail : public juce::Component, public juce::FileDragAndDropTarget
{
public:
    WaveThumbnail(JunebugSamplerAudioProcessor&);
    ~WaveThumbnail() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    bool isInterestedInFileDrag(const juce::StringArray& files);
    void filesDropped(const juce::StringArray& files, int x, int y);

private:
    juce::String fileName{ "" };
    JunebugSamplerAudioProcessor& audioProcessor;
    bool needRepaint{ false };
    std::vector<float> drawPoints;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveThumbnail)
};
