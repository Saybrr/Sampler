/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
JunebugSamplerAudioProcessorEditor::JunebugSamplerAudioProcessorEditor (JunebugSamplerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), waveThumb(p), envComp(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
   
    
    //-----------------------------------------------------------------
    //---------------------WAVE THUMBNAIL-----------------------------
    addAndMakeVisible(waveThumb);
    //-----------------------------------------------------------------
    //---------------------ENVELOPE SLIDERS---------------------------
    addAndMakeVisible(envComp);
    startTimerHz(60);
    setSize(600, 400);
}

JunebugSamplerAudioProcessorEditor::~JunebugSamplerAudioProcessorEditor()
{
    stopTimer();
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

    }
    else
    {
        g.drawText("Load a Sound", getWidth() / 2 - 50, getHeight() / 2 - 10, 100, 20, juce::Justification::centred);
    }

}

void JunebugSamplerAudioProcessorEditor::resized()
{
    waveThumb.setBoundsRelative(0.0f, 0.25f, 1.0f, 0.5f);
    envComp.setBoundsRelative(0.0f, 0.75f, 1.0f, 0.25f);

}

void JunebugSamplerAudioProcessorEditor::timerCallback()
{
    //we are using the timer to repaint at a set interval to draw the playhead
    repaint();
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
