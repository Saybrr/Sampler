/*
  ==============================================================================

    EnvelopeComponent.h
    Created: 7 Sep 2020 2:44:32pm
    Author:  saybe

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
//==============================================================================
/*
*/
class EnvelopeComponent  : public juce::Component
{
public:
    EnvelopeComponent(JunebugSamplerAudioProcessor&);
    ~EnvelopeComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JunebugSamplerAudioProcessor& audioProcessor;

    //Sliders for ADSR
    juce::Slider attackSlider, decaySlider, sustainSlider, releaseSlider;
    juce::Label attackLabel, decayLabel, sustainLabel, releaseLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvelopeComponent)
};
