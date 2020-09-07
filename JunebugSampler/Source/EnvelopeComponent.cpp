/*
  ==============================================================================

    EnvelopeComponent.cpp
    Created: 7 Sep 2020 2:44:32pm
    Author:  saybe

  ==============================================================================
*/

#include <JuceHeader.h>
#include "EnvelopeComponent.h"

//==============================================================================
EnvelopeComponent::EnvelopeComponent(JunebugSamplerAudioProcessor& p) : audioProcessor(p)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    attackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 20);
    attackSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::orange);
    addAndMakeVisible(attackSlider);
    //attackSlider.setRange(0.0f, 5.0f,0.01f);
    //attackSlider.addListener(this);
    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.getAPVTS(), "ATTACK", attackSlider);
    //atk label
    attackLabel.setFont(10.0f);
    attackLabel.setText("Attack", juce::NotificationType::dontSendNotification);
    attackLabel.setJustificationType(juce::Justification::centredTop);
    attackLabel.attachToComponent(&attackSlider, false);

    //dec slider
    decaySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    decaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 20);
    decaySlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::orange);
    addAndMakeVisible(decaySlider);
    //decaySlider.setRange(0.0f, 5.0f, 0.01f);
    //decaySlider.addListener(this);
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.getAPVTS(), "DECAY", decaySlider);
    //dec label
    decayLabel.setFont(10.0f);
    decayLabel.setText("Decay", juce::NotificationType::dontSendNotification);
    decayLabel.setJustificationType(juce::Justification::centredTop);
    decayLabel.attachToComponent(&decaySlider, false);

    //sus slider
    sustainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 20);
    sustainSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::orange);
    addAndMakeVisible(sustainSlider);
    //sustainSlider.addListener(this);
    //sustainSlider.setRange(0.0f, 1.0f, 0.01f);

    sustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.getAPVTS(), "SUSTAIN", sustainSlider);
    //sus label
    sustainLabel.setFont(10.0f);
    sustainLabel.setText("Sustain", juce::NotificationType::dontSendNotification);
    sustainLabel.setJustificationType(juce::Justification::centredTop);
    sustainLabel.attachToComponent(&sustainSlider, false);


    //rel slider
    releaseSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 20);
    releaseSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::orange);
    addAndMakeVisible(releaseSlider);
    //releaseSlider.addListener(this);
    //releaseSlider.setRange(0.0f, 1.0f, 0.01f);

    releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.getAPVTS(), "RELEASE", releaseSlider);
    //rel label
    releaseLabel.setFont(10.0f);
    releaseLabel.setText("Release", juce::NotificationType::dontSendNotification);
    releaseLabel.setJustificationType(juce::Justification::centredTop);
    releaseLabel.attachToComponent(&releaseSlider, false);

}

EnvelopeComponent::~EnvelopeComponent()
{
}

void EnvelopeComponent::paint (juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    //----------------SLIDERS AND LABELS------------------------------------
        //atk slider
    
}

void EnvelopeComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    const auto startX = 0.6;
    const auto xOff = 0.1f;
    const auto startY = 0.2f;
    const auto width = 0.1f;
    const auto height = 0.75f;

    attackSlider.setBoundsRelative(startX, startY, width, height);
    decaySlider.setBoundsRelative(startX + xOff, startY, width, height);
    sustainSlider.setBoundsRelative(startX + 2 * xOff, startY, width, height);
    releaseSlider.setBoundsRelative(startX + 3 * xOff, startY, width, height);

}
