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

    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;

    int& getStartPos() { return startPos; }
    int& getEndPos() { return endPos; }


private:
    juce::String fileName{ "" };
    JunebugSamplerAudioProcessor& audioProcessor;
    bool needRepaint{ false };
    std::vector<float> drawPoints;
    
    //sample selection start and end, where mouse is pressed down and up
    int startPos{ 0 };
    int endPos{ 0 };
    //flag to determine when to redraw selection rectangle
    bool reDrawRect{ false };
    bool finalRect{ false };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveThumbnail)
};
