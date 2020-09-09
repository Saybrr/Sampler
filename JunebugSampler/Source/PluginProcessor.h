/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class JunebugSamplerAudioProcessor : public juce::AudioProcessor,
                                     public juce ::ValueTree::Listener
{
public:
    //==============================================================================
    JunebugSamplerAudioProcessor();
    ~JunebugSamplerAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    
    void loadFile();
    void loadFile(const juce::String& path);

    juce::StringArray samplenames{ juce::StringArray() };

    int getNumSamplerSounds() { return sampler.getNumSounds(); }

    //Accessor method for our private audio buffer
    juce::AudioBuffer<float>& getWaveForm() { return waveForm; }

    //ADSR Envelope
    void updateADSR();
    juce::ADSR::Parameters& getADSRParams() { return adsrParams; }
    
    
    juce::AudioProcessorValueTreeState& getAPVTS() { return myAPVST; }

    std::atomic<bool>& getIsNotePlayed() { return isNotePlayed; }
    std::atomic<int>& getSamplePlayedCount() { return samplePlayedCount; }

private:
    juce::Synthesiser sampler;
    const int numVoices{ 3 };

    juce::ADSR::Parameters adsrParams; 

    //buffer we use to draw a visual representation of our sample
    //TODO: use this for amplitude splicing? 
    juce::AudioBuffer<float> waveForm;

    juce::AudioFormatManager formatManager;
    //each file gets its own reader, so we want to just store a pointer
    //to the appropriate reader instead 
    juce::AudioFormatReader* formatReader{ nullptr }; 

    juce::AudioProcessorValueTreeState myAPVST;

    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();

    void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property);
    
    std::atomic<bool> shouldUpdate{ false };
    std::atomic<bool> isNotePlayed{ false }; 
    std::atomic<int> samplePlayedCount{ 0 };
   
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JunebugSamplerAudioProcessor)
};
