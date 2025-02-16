/*
  ==============================================================================

    WaveThumbnail.cpp
    Created: 7 Sep 2020 12:58:20pm
    Author:  saybe

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveThumbnail.h"

//==============================================================================
WaveThumbnail::WaveThumbnail(JunebugSamplerAudioProcessor& p) : audioProcessor(p)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

WaveThumbnail::~WaveThumbnail()
{
}

void WaveThumbnail::paint (juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    if (audioProcessor.getWaveForm().getNumSamples() > 0) {
        //our Path object used to draw the waveform
        juce::Path p;
        drawPoints.clear();
        //we must scale the drawing of the waveform to our width and height of the window:
        //waveform should take up top half of window
        //each sample should be drawn with the width of sampleLength/getWidth() -- maybe do this later to allow for zooming?
        //for now, we "downsample" by the scaleFactor and only draw every sample s.t. sample%scaleFactor == 0

        auto waveForm = audioProcessor.getWaveForm();
        auto scaleFactor = waveForm.getNumSamples() / getWidth();

        auto buffer = waveForm.getReadPointer(0);

        for (int sample = 0; sample < waveForm.getNumSamples(); sample += scaleFactor)
        {
            //only stretch the points we want to draw - 
            auto scaled = juce::jmap<float>(buffer[sample], -1.0f, 1.0f, static_cast<float>(getHeight()), 0.0f);
            drawPoints.push_back(scaled);
        }

        p.startNewSubPath(0, getHeight()/2);

        for (int sample = 0; sample < drawPoints.size(); sample++)
        {
            //create a line using several X,Y points 
            //does not draw it! 
            p.lineTo(sample, drawPoints[sample]);
        }

        //draws the path described above
        g.strokePath(p, juce::PathStrokeType(2));

        //Draw Sample Fileame Above Wavefrom
        g.setColour(juce::Colours::white);
        g.setFont(15.0f);

        auto nameBounds = getLocalBounds().reduced(10, 10);
        g.drawFittedText(fileName, nameBounds, juce::Justification::topRight, 1);

        //----------------------SELECTION RECTANGLE--------------------------------
        //TODO: IF reDrawRect, draw from start to current mouse x pos 
        if (reDrawRect)
        {
            g.setColour(juce::Colours::lightgreen);
            g.setOpacity(0.4f);
            int width = endPos - startPos;
            g.drawRect(startPos, 0, (width > 0) ? width : 1, getHeight());
            reDrawRect = false;
        }
        if (finalRect)
        {
            int width = endPos - startPos;
            g.drawRect(startPos, 0, (width > 0) ? width : 1, getHeight());
        }   
        g.drawFittedText(static_cast<juce::String>(startPos), getLocalBounds().reduced(20, 50), juce::Justification::topRight, 1);
        g.drawFittedText(static_cast<juce::String>(audioProcessor.getStartPos()), getLocalBounds().reduced(20, 70), juce::Justification::topRight, 1);
        g.drawFittedText(static_cast<juce::String>(endPos), getLocalBounds().reduced(20, 20), juce::Justification::topRight, 1);
        g.drawFittedText(static_cast<juce::String>(audioProcessor.getStartPos()), getLocalBounds().reduced(20, 70), juce::Justification::topRight, 1);
        //-------------------------------------------------------------------------
        //----------------------PLAYHEAD DRAWING------------------------------------
        //map num samples to the width of our screen via jmap
        //draw a line at the current sample we are playing
        if (audioProcessor.getSamplePlayedCount() > 0) 
        {
           // DBG(audioProcessor.getSamplePlayedCount());
            auto playHeadPosition = juce::jmap<int>(audioProcessor.getSamplePlayedCount(), 0, audioProcessor.getWaveForm().getNumSamples(), 0, getWidth());
            DBG("Playhead Pos: "<< playHeadPosition<< " Width: " << getWidth() << " TotalSamples: " << audioProcessor.getWaveForm().getNumSamples());
            g.setColour(juce::Colours::white);
            //note: locations are relative to component as we are in a child component of editor
            g.drawLine(playHeadPosition, 0, playHeadPosition, getHeight(), 2.0f);
        }

        //------------------------------------------------------------------------
        //needRepaint = false;
    }
    else
    {
        g.setColour(juce::Colours::white);
        g.drawFittedText("Drag and drop a Sample!", getLocalBounds(), juce::Justification::centred, 1);
    }
    
}

void WaveThumbnail::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
}


bool WaveThumbnail::isInterestedInFileDrag(const juce::StringArray & files)
{
    //we need to check if all files in array are valid audio file to sample
    bool valid = true;
    for (auto file : files)
    {
        if (!(file.contains(".wav") || file.contains(".mp3") || file.contains(".aif") || file.contains(".flac")))
        {
            valid = false;
        }
    }
    return valid;
}


void WaveThumbnail::filesDropped(const juce::StringArray & files, int x, int y)
{
    for (auto file : files)
    {
        if (isInterestedInFileDrag(files))
        {
            //load the file from an absolute path
            //needRepaint = true;
            auto myFile = std::make_unique<juce::File>(file);
            fileName = myFile->getFileNameWithoutExtension();
            audioProcessor.loadFile(file);
                

        }

    }
    audioProcessor.samplenames = files;
    repaint();

}

void WaveThumbnail::mouseDown(const juce::MouseEvent& event)
{
    //start pos = xpos
    startPos = getMouseXYRelative().getX();
    
    auto mappedToSample = juce::jmap(startPos, 0, getWidth(), 0, audioProcessor.getWaveForm().getNumSamples());
    audioProcessor.setStartPos(mappedToSample);
    reDrawRect = true;
    DBG("START POS: " << startPos << "START SAMPLE: " << mappedToSample);
    repaint();
}

void WaveThumbnail::mouseDrag(const juce::MouseEvent& event)
{
    endPos = getMouseXYRelative().getX();
    reDrawRect = true;
    repaint();
    DBG("DRAGGED TO: " << endPos);
}

void WaveThumbnail::mouseUp(const juce::MouseEvent& event)
{
    //end pos = xpos
    endPos = getMouseXYRelative().getX();
    auto mappedToSample = juce::jmap(endPos, 0, getWidth(), 0, audioProcessor.getWaveForm().getNumSamples());
    audioProcessor.setEndPos(mappedToSample);
    finalRect = true;
    reDrawRect = false;
    DBG("ENDED AT: " << endPos << "END SAMPLE: " << mappedToSample);
    repaint();
}

