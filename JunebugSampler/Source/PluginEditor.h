/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "WaveThumbnail.h"
#include "EnvelopeComponent.h"
//==============================================================================
/**
*/
class JunebugSamplerAudioProcessorEditor : public juce::AudioProcessorEditor 
                                     
                                           //public juce::Slider::Listener
{
public:
    JunebugSamplerAudioProcessorEditor (JunebugSamplerAudioProcessor&);
    ~JunebugSamplerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    //FileDragAndDropTarget 
  /*  bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;*/

    //pure virtual from Slider Listener *DEPRECATED -- USING VALUE TREE STATE INSTEAD*
    //void sliderValueChanged(juce::Slider* slider) override;

private:
 
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    JunebugSamplerAudioProcessor& audioProcessor;

    WaveThumbnail waveThumb;
    EnvelopeComponent envComp;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JunebugSamplerAudioProcessorEditor)
};
