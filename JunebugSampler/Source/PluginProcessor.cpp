/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JunebugSamplerAudioProcessor::JunebugSamplerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), myAPVST(*this,nullptr, "PARAMETERS", createParameters())
#endif
{
    //Constructor for the sampler class
    formatManager.registerBasicFormats();
    myAPVST.state.addListener(this);

    for (int i = 0; i < numVoices; i++)
    {
        //we create numVoices voices 
        sampler.addVoice(new juce::SamplerVoice());
    }
}

JunebugSamplerAudioProcessor::~JunebugSamplerAudioProcessor()
{
    //destructor 
    formatReader = nullptr;

}

//==============================================================================
const juce::String JunebugSamplerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JunebugSamplerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool JunebugSamplerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool JunebugSamplerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double JunebugSamplerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JunebugSamplerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int JunebugSamplerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void JunebugSamplerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String JunebugSamplerAudioProcessor::getProgramName (int index)
{
    return {};
}

void JunebugSamplerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void JunebugSamplerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    sampler.setCurrentPlaybackSampleRate(sampleRate);
}

void JunebugSamplerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool JunebugSamplerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void JunebugSamplerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    if (shouldUpdate)
    {
        updateADSR();
    }
    

    sampler.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

//    
//    for (int channel = 0; channel < totalNumInputChannels; ++channel)
//    {
//        auto* channelData = buffer.getWritePointer (channel);
//
//        // ..do something to the data...
//    }
}

//==============================================================================
bool JunebugSamplerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* JunebugSamplerAudioProcessor::createEditor()
{
    return new JunebugSamplerAudioProcessorEditor (*this);
}

//==============================================================================
void JunebugSamplerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void JunebugSamplerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void JunebugSamplerAudioProcessor::loadFile()
{
    //uses a system Explorer GUI to select file(s)
    sampler.clearSounds();
    juce::FileChooser chooser{ "Please Select a Sample"};
    juce::BigInteger range;
    range.setRange(0, 128, true);

    if (chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        formatReader = formatManager.createReaderFor(file);
        sampler.addSound(new juce::SamplerSound("Sample", *formatReader, range, 60, 0.1, 0.1, 10.0));
    }    
}

void JunebugSamplerAudioProcessor::loadFile(const juce::String& path)
{
    //called when using the drag and drop method where a path to a file has been 
    //already supplied by user
    sampler.clearSounds();
    juce::BigInteger range;
    range.setRange(0, 128, true);

    auto file = juce::File(path);
    formatReader = formatManager.createReaderFor(file);

    //need to set size of buffer before writing to it
    auto numSamples = static_cast<int>(formatReader->lengthInSamples);
    waveForm.setSize(2, numSamples);
    formatReader->read(&waveForm, 0,numSamples,0, true, true);

    //iterate through the audio buffer via a pointer
    auto buffer = waveForm.getReadPointer(0);


    //for (int sample = 0; sample < waveForm.getNumSamples(); sample++)
    //{
    //    //print out the float value (amplitude) at a given sample
    //    DBG(buffer[sample]);
    //}


    sampler.addSound(new juce::SamplerSound("Sample", *formatReader, range, 60, 0.1, 0.1, 10.0));
}

void JunebugSamplerAudioProcessor::updateADSR()
{
    //read the params from the ValueTree
    adsrParams.attack = myAPVST.getRawParameterValue("ATTACK")->load();
    adsrParams.decay = myAPVST.getRawParameterValue("DECAY")->load();
    adsrParams.sustain = myAPVST.getRawParameterValue("SUSTAIN")->load();
    adsrParams.release = myAPVST.getRawParameterValue("RELEASE")->load();

    //theres a built in ADSR object in SamplerSounds! How do we access it to write to it? 
    //iterate through the sounds of our Synthesiser (the samples we want to play) 
    //then build / write ADSR envelopes to the SamplerSound objects using setEnvelopeParameters
    for (int i = 0; i < sampler.getNumSounds(); i++)
    {
        //need to validate that a given sound is a SamplerSound (not SynthesiserSound)
        auto sound = dynamic_cast<juce::SamplerSound*>(sampler.getSound(i).get());
        if (sound)
        {
            //SamplerSound.setEnvelopeParameters
            //apply params set by sliders in Editor.sliderValueChanged(1)
            sound->setEnvelopeParameters(adsrParams);
        }

    }
    //DBG(attack << decay << sustain << release);
}

juce::AudioProcessorValueTreeState::ParameterLayout JunebugSamplerAudioProcessor::createParameters()
{
    //creates the parameters for our ValueTree, so far it is ADSR that we want to read from sliders
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", 0.0f, 5.0f, 0.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", 0.0f, 5.0f, 2.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", 0.0f, 1.0f, 1.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", 0.0f, 5.0f, 2.0f));
    return { parameters.begin(), parameters.end() };
}

void JunebugSamplerAudioProcessor::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
{
    //called when something is changed in the value tree 
    shouldUpdate = true;
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JunebugSamplerAudioProcessor();
}
