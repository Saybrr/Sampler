/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
JunebugSamplerAudioProcessorEditor::JunebugSamplerAudioProcessorEditor (JunebugSamplerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), waveThumb(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    loadButton.onClick = [&]() {audioProcessor.loadFile(); };
    addAndMakeVisible(loadButton);
    
    //----------------SLIDERS AND LABELS------------------------------------
    //atk slider
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
    attackLabel.attachToComponent(&attackSlider,false);

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
    decayLabel.setText("Decay",juce::NotificationType::dontSendNotification);
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
    //-----------------------------------------------------------------
    //---------------------WAVE THUMBNAIL-----------------------------
    addAndMakeVisible(waveThumb);

    setSize(600, 400);
}

JunebugSamplerAudioProcessorEditor::~JunebugSamplerAudioProcessorEditor()
{
}

//==============================================================================
void JunebugSamplerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colours::white);
    
        //use jmap to scale on y axis: [-1,1] to [getHeight(),0]
        //
    
    //juce::jmap(waveForm, 0, getHeight());

    if (audioProcessor.getNumSamplerSounds() > 0)
    {
        //DRAW TEXT 

        g.setColour(juce::Colours::white);
        g.setFont(15.0f);
        //g.drawText("Sound(s) Loaded", getWidth() / 2 - 50, getHeight() / 2 - 10, getWidth(), 20, juce::Justification::left, false);
        ////list names of sample(s) loaded
        //int offset = 15;
        //for (auto sample : audioProcessor.samplenames)
        //{
        //    auto basename = sample.fromLastOccurrenceOf("\\", false, true);
        //    g.drawText(basename, getWidth() / 2 - 50, getHeight() / 2 - 10 + offset, getWidth(), 20, juce::Justification::left, false);
        //    offset += 15;
        //}

        //std::string base_filename = path.substr(path.find_last_of("/\\") + 1)
        //https://stackoverflow.com/questions/8520560/get-a-file-name-from-a-path


         

    }
    else
    {
        g.drawText("Load a Sound", getWidth() / 2 - 50, getHeight() / 2 - 10, 100, 20, juce::Justification::centred);
    }
    //g.setColour (juce::Colours::white);
    //g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);


}

void JunebugSamplerAudioProcessorEditor::resized()
{
    waveThumb.setBoundsRelative(0.0f, 0.25f, 1.0f, 0.5f);

    const auto startX = 0.6;
    const auto xOff = 0.1f;
    const auto startY = 0.8f;
    const auto width = 0.1f;
    const auto height = 0.2f;

    attackSlider.setBoundsRelative(startX,startY,width,height);
    decaySlider.setBoundsRelative(startX + xOff, startY, width, height);
    sustainSlider.setBoundsRelative(startX + 2 * xOff, startY, width, height);
    releaseSlider.setBoundsRelative(startX + 3 * xOff, startY, width, height);
}



//void JunebugSamplerAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
//{
//    //a slider value has been changed! Which one is it? and what do we do with the changes? 
//    if (slider == &attackSlider)
//    {
//        audioProcessor.getADSRParams().attack = attackSlider.getValue();
//    }
//    else if (slider == &decaySlider)
//    {
//        audioProcessor.getADSRParams().decay = decaySlider.getValue();
//    }
//    else if (slider == &sustainSlider)
//    {
//        audioProcessor.getADSRParams().sustain = sustainSlider.getValue();
//    }
//    else if(slider == &releaseSlider)
//    {
//        audioProcessor.getADSRParams().release = releaseSlider.getValue();
//    }
//
//    audioProcessor.updateADSR();
//}
