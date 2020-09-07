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
    envComp.setBoundsRelative(0.0f, 0.75f, 1.0f, 0.25f);

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
