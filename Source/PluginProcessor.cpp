/**
 *  ElephantDSP.com Hall Reverb
 *
 *  Copyright (C) 2022 Christian Voigt
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "PluginProcessor.h"

ReverbAudioProcessor::ReverbAudioProcessor()
        :
#ifndef JucePlugin_PreferredChannelConfigurations
          AudioProcessor(
              BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                  .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                  .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
                  ),
#endif
          parameters(*this, &undo, "parameters", createParameterLayout())
{
    // add a listener for each parameter
    parameters.addParameterListener("dryLevel", this);
    parameters.addParameterListener("earlyLevel", this);
    parameters.addParameterListener("earlySendLevel", this);
    parameters.addParameterListener("lateLevel", this);
    parameters.addParameterListener("earlyOutputHPF", this);
    parameters.addParameterListener("earlyOutputLPF", this);
    parameters.addParameterListener("earlyRoomSize", this);
    parameters.addParameterListener("earlyStereoWidth", this);
    parameters.addParameterListener("lateApFeedback", this);
    parameters.addParameterListener("lateCrossOverFreqHigh", this);
    parameters.addParameterListener("lateCrossOverFreqLow", this);
    parameters.addParameterListener("lateDecay", this);
    parameters.addParameterListener("lateDecayFactorHigh", this);
    parameters.addParameterListener("lateDecayFactorLow", this);
    parameters.addParameterListener("lateDiffusion", this);
    parameters.addParameterListener("lateLFO1Freq", this);
    parameters.addParameterListener("lateLFO2Freq", this);
    parameters.addParameterListener("lateLFOFactor", this);
    parameters.addParameterListener("lateOutputHPF", this);
    parameters.addParameterListener("lateOutputLPF", this);
    parameters.addParameterListener("latePredelay", this);
    parameters.addParameterListener("lateRoomSize", this);
    parameters.addParameterListener("lateSpin", this);
    parameters.addParameterListener("lateSpinFactor", this);
    parameters.addParameterListener("lateStereoWidth", this);
    parameters.addParameterListener("lateWander", this);
}

ReverbAudioProcessor::~ReverbAudioProcessor()
{
}

//==============================================================================
const juce::String ReverbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ReverbAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool ReverbAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool ReverbAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double ReverbAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ReverbAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are
              // 0 programs, so this should be at least 1, even if you're not
              // really implementing programs.
}

int ReverbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ReverbAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String ReverbAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return "None";
}

void ReverbAudioProcessor::changeProgramName(int index,
                                             const juce::String& newName)
{
    juce::ignoreUnused(index);
    juce::ignoreUnused(newName);
}

//==============================================================================
void ReverbAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused(samplesPerBlock);
    reverb.setSampleRate(sampleRate);
}

void ReverbAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    reverb.mute();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ReverbAudioProcessor::isBusesLayoutSupported(
    const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
        layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

        // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void ReverbAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                        juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    if (totalNumInputChannels == 1 && totalNumOutputChannels == 2)
    {
        // mono in, stereo out
        reverb.process(buffer.getReadPointer(0), buffer.getReadPointer(0),
                       buffer.getWritePointer(0), buffer.getWritePointer(1),
                       buffer.getNumSamples());
    }
    else if (totalNumInputChannels == 2 && totalNumOutputChannels == 2)
    {
        // stereo in, stereo out
        reverb.process(buffer.getReadPointer(0), buffer.getReadPointer(1),
                       buffer.getWritePointer(0), buffer.getWritePointer(1),
                       buffer.getNumSamples());
    }
    else
    {
        jassertfalse; // channel layout not supported
    }
}

//==============================================================================
bool ReverbAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ReverbAudioProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void ReverbAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void ReverbAudioProcessor::setStateInformation(const void* data,
                                               int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory
    // block, whose contents will have been created by the getStateInformation()
    // call.
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReverbAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout
ReverbAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout params;
    using Range = juce::NormalisableRange<float>;

    params.add(std::make_unique<juce::AudioParameterFloat>("dryLevel", "dryLevel", Range{0.0f, 1.0f, 0.01f}, 0.8f, ""));
    params.add(std::make_unique<juce::AudioParameterFloat>("earlyLevel", "earlyLevel", Range{0.0f, 1.0f, 0.01f}, 0.1f, ""));
    params.add(std::make_unique<juce::AudioParameterFloat>("earlySendLevel", "earlySendLevel", Range{0.0f, 1.0f, 0.01f}, 0.2f, ""));
    params.add(std::make_unique<juce::AudioParameterFloat>("lateLevel", "lateLevel", Range{0.0f, 1.0f, 0.01f}, 0.2f, ""));

    params.add(std::make_unique<juce::AudioParameterFloat>("earlyOutputHPF", "earlyOutputHPF", Range{0.0f, 16000.0f, 1.0f}, 4.0f, " Hz"));
    params.add(std::make_unique<juce::AudioParameterFloat>("earlyOutputLPF", "earlyOutputLPF", Range{0.0f, 16000.0f, 1.0f}, 16000.0f, " Hz"));
    params.add(std::make_unique<juce::AudioParameterFloat>("earlyRoomSize", "earlyRoomSize", Range{0.4f, 3.6f, 0.1f}, 0.5f, ""));
    params.add(std::make_unique<juce::AudioParameterFloat>("earlyStereoWidth", "earlyStereoWidth", Range{-1.0f, 1.0f, 0.01f}, 1.0f, ""));

    params.add(std::make_unique<juce::AudioParameterFloat>("lateApFeedback", "lateApFeedback", Range{-1.0f, 1.0f, 0.01f}, 0.63f, ""));
    params.add(std::make_unique<juce::AudioParameterFloat>("lateCrossOverFreqHigh", "lateCrossOverFreqHigh", Range{0.0f, 16000.0f, 1.0f}, 3600.0f, " Hz"));
    params.add(std::make_unique<juce::AudioParameterFloat>("lateCrossOverFreqLow", "lateCrossOverFreqLow", Range{0.0f, 16000.0f, 1.0f}, 500.0f, " Hz"));
    params.add(std::make_unique<juce::AudioParameterFloat>("lateDecay", "lateDecay", Range{0.1f, 30.0f, 0.01f}, 0.4f, " s"));
    params.add(std::make_unique<juce::AudioParameterFloat>("lateDecayFactorHigh", "lateDecayFactorHigh", Range{0.1f, 5.0f, 0.1f}, 0.3f, ""));
    params.add(std::make_unique<juce::AudioParameterFloat>("lateDecayFactorLow", "lateDecayFactorLow", Range{0.1f, 5.0f, 0.1f}, 1.3f, ""));
    params.add(std::make_unique<juce::AudioParameterFloat>("lateDiffusion", "lateDiffusion", Range{-1.0f, 1.0f, 0.01f}, 0.82f, ""));
    params.add(std::make_unique<juce::AudioParameterFloat>("lateLFO1Freq", "lateLFO1Freq", Range{0.0f, 5.0f, 0.1f}, 0.9f, " Hz"));
    params.add(std::make_unique<juce::AudioParameterFloat>("lateLFO2Freq", "lateLFO2Freq", Range{0.0f, 5.0f, 0.1f}, 1.3f, " Hz"));
    params.add(std::make_unique<juce::AudioParameterFloat>("lateLFOFactor", "lateLFOFactor", Range{0.0f, 1.0f, 0.01f}, 0.31f, ""));
    params.add(std::make_unique<juce::AudioParameterFloat>("lateOutputHPF", "lateOutputHPF", Range{0.0f, 16000.0f, 1.0f}, 4.0f, " Hz"));
    params.add(std::make_unique<juce::AudioParameterFloat>("lateOutputLPF", "lateOutputLPF", Range{0.0f, 16000.0f, 1.0f}, 16000.0f, " Hz"));
    params.add(std::make_unique<juce::AudioParameterFloat>("latePredelay", "latePredelay", Range{0.0f, 200.0f, 0.1f}, 8.0f, " ms"));
    params.add(std::make_unique<juce::AudioParameterFloat>("lateRoomSize", "lateRoomSize", Range{0.4f, 3.6f, 0.1f}, 0.5f, ""));
    params.add(std::make_unique<juce::AudioParameterFloat>("lateSpin", "lateSpin", Range{0.0f, 50.0f, 0.1f}, 2.4f, " Hz"));
    params.add(std::make_unique<juce::AudioParameterFloat>("lateSpinFactor", "lateSpinFactor", Range{0.0f, 1.0f, 0.01f}, 0.3f, ""));
    params.add(std::make_unique<juce::AudioParameterFloat>("lateStereoWidth", "lateStereoWidth", Range{-1.0f, 1.0f, 0.01f}, 1.0f, ""));
    params.add(std::make_unique<juce::AudioParameterFloat>("lateWander", "lateWander", Range{0.0f, 100.0f, 1.0f}, 22.0f, " ms"));

    return params;
}

void ReverbAudioProcessor::parameterChanged(const juce::String& parameter, float newValue)
{
    if (parameter == "dryLevel")
    {
        reverb.setDryLevel(newValue);
    }
    else if (parameter == "earlyLevel")
    {
        reverb.setEarlyLevel(newValue);
    }
    else if (parameter == "earlySendLevel")
    {
        reverb.setEarlySendLevel(newValue);
    }
    else if (parameter == "lateLevel")
    {
        reverb.setLateLevel(newValue);
    }
    else if (parameter == "earlyOutputHPF")
    {
        reverb.setEarlyOutputHPF(newValue);
    }
    else if (parameter == "earlyOutputLPF")
    {
        reverb.setEarlyOutputLPF(newValue);
    }
    else if (parameter == "earlyRoomSize")
    {
        reverb.setEarlyRoomSize(newValue);
    }
    else if (parameter == "earlyStereoWidth")
    {
        reverb.setEarlyStereoWidth(newValue);
    }
    else if (parameter == "lateApFeedback")
    {
        reverb.setLateApFeedback(newValue);
    }
    else if (parameter == "lateCrossOverFreqHigh")
    {
        reverb.setLateCrossOverFreqHigh(newValue);
    }
    else if (parameter == "lateCrossOverFreqLow")
    {
        reverb.setLateCrossOverFreqLow(newValue);
    }
    else if (parameter == "lateDecay")
    {
        reverb.setLateDecay(newValue);
    }
    else if (parameter == "lateDecayFactorHigh")
    {
        reverb.setLateDecayFactorHigh(newValue);
    }
    else if (parameter == "lateDecayFactorLow")
    {
        reverb.setLateDecayFactorLow(newValue);
    }
    else if (parameter == "lateDiffusion")
    {
        reverb.setLateDiffusion(newValue);
    }
    else if (parameter == "lateLFO1Freq")
    {
        reverb.setLateLFO1Freq(newValue);
    }
    else if (parameter == "lateLFO2Freq")
    {
        reverb.setLateLFO2Freq(newValue);
    }
    else if (parameter == "lateLFOFactor")
    {
        reverb.setLateLFOFactor(newValue);
    }
    else if (parameter == "lateOutputHPF")
    {
        reverb.setLateOutputHPF(newValue);
    }
    else if (parameter == "lateOutputLPF")
    {
        reverb.setLateOutputLPF(newValue);
    }
    else if (parameter == "latePredelay")
    {
        reverb.setLatePredelay(newValue);
    }
    else if (parameter == "lateRoomSize")
    {
        reverb.setLateRoomSize(newValue);
    }
    else if (parameter == "lateSpin")
    {
        reverb.setLateSpin(newValue);
    }
    else if (parameter == "lateSpinFactor")
    {
        reverb.setLateSpinFactor(newValue);
    }
    else if (parameter == "lateStereoWidth")
    {
        reverb.setLateStereoWidth(newValue);
    }
    else if (parameter == "lateWander")
    {
        reverb.setLateWander(newValue);
    }
    else
    {
        jassertfalse; // unknown parameter ...
    }
}
