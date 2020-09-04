/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JunebugSamplerAudioProcessorEditor::JunebugSamplerAudioProcessorEditor (JunebugSamplerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    loadButton.onClick = [&]() {audioProcessor.loadFile(); };
    addAndMakeVisible(loadButton);
    setSize(600, 200);
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
    
    if (needRepaint) {
        //our Path object used to draw the waveform
        juce::Path p;
        drawPoints.clear();
        //we must scale the drawing of the waveform to our width and height of the window:
        //each sample should be drawn with the width of sampleLength/getWidth() -- maybe do this later to allow for zooming?
        //for now, we "downsample" by the scaleFactor and only draw every sample s.t. sample%scaleFactor == 0

        auto waveForm = audioProcessor.getWaveForm();
        auto scaleFactor = waveForm.getNumSamples() / getWidth();

        auto buffer = waveForm.getReadPointer(0);

        for (int sample = 0; sample < waveForm.getNumSamples(); sample += scaleFactor)
        {
            //only stretch the points we want to draw
            auto scaled = juce::jmap<float>(buffer[sample], -1.0f, 1.0f, static_cast<float>(getHeight()), 0.0f);
            drawPoints.push_back(scaled);
        }

        p.startNewSubPath(0, getHeight() / 2);

        for (int sample = 0; sample < drawPoints.size(); sample++)
        {
            //create a line using several X,Y points 
            //does not draw it! 
            p.lineTo(sample, drawPoints[sample]);
        }

        //draws the path described above
        g.strokePath(p, juce::PathStrokeType(2));
    }
    needRepaint = false;
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
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    //loadButton.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 50, 100, 100);
}

bool JunebugSamplerAudioProcessorEditor::isInterestedInFileDrag(const juce::StringArray& files)
{
    //we need to check if all files in array are valid audio file to sample
    bool valid = true;
    for (auto file: files)
    {
        if (!(file.contains(".wav") || file.contains(".mp3") || file.contains(".aif") || file.contains(".flac")))
        {
            valid = false;
        }
    }
    return valid;
}


void JunebugSamplerAudioProcessorEditor::filesDropped(const juce::StringArray& files, int x, int y)
{
    for (auto file : files)
    {
        if (isInterestedInFileDrag(files))
        {
            //load the file from an absolute path
            needRepaint = true;
            audioProcessor.loadFile(file);
            

        }
        
    }
    audioProcessor.samplenames = files;
    repaint();

}
