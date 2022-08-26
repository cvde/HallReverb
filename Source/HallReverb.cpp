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

#include "HallReverb.h"

HallReverb::HallReverb()
{
    // initialize unused freeverb parameters
    early.setdryr(0.0f);
    early.setwetr(1.0f);
    early.loadPresetReflection(0);
    early.setDiffusionApFreq(150.0f, 4.0f);
    early.setLRCrossApFreq(750.0f, 4.0f);
    early.setLRDelay(0.3f);
    early.setMuteOnChange(false);
    early.setoutputhpf(4.0f);
    early.setPreDelay(0.0f);
    late.setdryr(0.0f);
    late.setwetr(1.0f);
    late.setMuteOnChange(false);
    late.setdccutfreq(2.5);

    // initialize everything else
    setSampleRate(44100.0f);
    setDryLevel(0.8f);
    setEarlyLevel(0.1f);
    setEarlySendLevel(0.2f);
    setLateLevel(0.2f);

    setEarlyOutputHPF(4.0f);
    setEarlyOutputLPF(16000.0f);
    setEarlyRoomSize(0.5f);
    setEarlyStereoWidth(1.0f);

    setLateApFeedback(0.63f);
    setLateCrossOverFreqHigh(3600.0f);
    setLateCrossOverFreqLow(500.0f);
    setLateDecay(0.4f);
    setLateDecayFactorHigh(0.3f);
    setLateDecayFactorLow(1.3f);
    setLateDiffusion(0.82f);
    setLateLFO1Freq(0.9f);
    setLateLFO2Freq(1.3f);
    setLateLFOFactor(0.31f);
    setLateOutputHPF(4.0f);
    setLateOutputLPF(16000.0f);
    setLatePredelay(8.0f);
    setLateRoomSize(0.5f);
    setLateSpin(2.4f);
    setLateSpinFactor(0.3f);
    setLateStereoWidth(1.0f);
    setLateWander(22.0f);
}

void HallReverb::setSampleRate(float newSampleRate)
{
    early.setSampleRate(newSampleRate);
    late.setSampleRate(newSampleRate);
}

void HallReverb::process(const float* leftChannelIn,
                         const float* rightChannelIn, float* leftChannelOut,
                         float* rightChannelOut, int numSamples)
{
    // updating these parameters only once per buffer prevents segmention faults
    if (earlyRoomSizeNeedsUpdate)
    {
        early.setRSFactor(earlyRoomSize);
        earlyRoomSizeNeedsUpdate = false;
    }
    if (lateRoomSizeNeedsUpdate)
    {
        late.setRSFactor(lateRoomSize);
        lateRoomSizeNeedsUpdate = false;
    }
    if (latePredelayNeedsUpdate)
    {
        late.setPreDelay(latePredelay);
        latePredelayNeedsUpdate = false;
    }

    // split the buffer into fixed size chunks
    for (int offset = 0; offset < numSamples; offset += bufferSize)
    {
        int numSamplesInBuffer = numSamples - offset < bufferSize ? numSamples - offset : bufferSize;

        for (int i = 0; i < numSamplesInBuffer; ++i)
        {
            leftBufferIn[i] = leftChannelIn[offset + i];
            rightBufferIn[i] = rightChannelIn[offset + i];
        }

        early.processreplace(leftBufferIn,
                             rightBufferIn,
                             leftEarlyOut,
                             rightEarlyOut,
                             numSamplesInBuffer);

        for (int i = 0; i < numSamplesInBuffer; ++i)
        {
            leftLateIn[i] = earlySendLevel * leftEarlyOut[i] + leftBufferIn[i];
            rightLateIn[i] = earlySendLevel * rightEarlyOut[i] + rightBufferIn[i];
        }

        late.processreplace(leftLateIn,
                            rightLateIn,
                            leftLateOut,
                            rightLateOut,
                            numSamplesInBuffer);

        for (int i = 0; i < numSamplesInBuffer; ++i)
        {
            leftChannelOut[offset + i] = dryLevel * leftBufferIn[i] +
                                         earlyLevel * leftEarlyOut[i] +
                                         lateLevel * leftLateOut[i];
            rightChannelOut[offset + i] = dryLevel * rightBufferIn[i] +
                                          earlyLevel * rightEarlyOut[i] +
                                          lateLevel * rightLateOut[i];
        }
    }
}

void HallReverb::mute()
{
    early.mute();
    late.mute();
}

void HallReverb::setDryLevel(float newDryLevel)
{
    // The level of the dry signal. (DRY)
    dryLevel = newDryLevel;
}

void HallReverb::setEarlyLevel(float newEarlyLevel)
{
    // The level of the early reflection signal. (EWET)
    earlyLevel = newEarlyLevel;
}

void HallReverb::setEarlySendLevel(float newEarlySend)
{
    // The level of the early reflection signal which was send to the late reverberation. (ESEN)
    earlySendLevel = newEarlySend;
}

void HallReverb::setLateLevel(float newLateLevel)
{
    // The level of the late reverberation signal. (LWET)
    lateLevel = newLateLevel;
}

void HallReverb::setEarlyOutputHPF(float newEarlyOutputHPF)
{
    // The cutoff frequency of the high pass filter of the early reflection signal. (EHPF)
    early.setoutputhpf(newEarlyOutputHPF);
}

void HallReverb::setEarlyOutputLPF(float newEarlyOutputLPF)
{
    // The cutoff frequency of the low pass filter of the early reflection signal. (ELPF)
    early.setoutputlpf(newEarlyOutputLPF);
}

void HallReverb::setEarlyRoomSize(float newEarlyRoomSize)
{
    // The room size of the early reflection. (EFAC)
    earlyRoomSize = newEarlyRoomSize;
    earlyRoomSizeNeedsUpdate = true;
}

void HallReverb::setEarlyStereoWidth(float newEarlyStereoWidth)
{
    // The stereo width of the early reflection. (EWID)
    early.setwidth(newEarlyStereoWidth);
}

void HallReverb::setLateApFeedback(float newLateApFeedback)
{
    // The strength of the allpass diffusor in the FDN loop. (ADIF)
    late.setapfeedback(newLateApFeedback);
}

void HallReverb::setLateCrossOverFreqHigh(float newLateCrossOverFreqHigh)
{
    // The high crossover frequency for the late reverb time. (XOH)
    late.setxover_high(newLateCrossOverFreqHigh);
}

void HallReverb::setLateCrossOverFreqLow(float newLateCrossOverFreqLow)
{
    // The low crossover frequency for the late reverb time. (XOL)
    late.setxover_low(newLateCrossOverFreqLow);
}

void HallReverb::setLateDecay(float newLateDecay)
{
    // The reverb time. (RT60)
    late.setrt60(newLateDecay);
}

void HallReverb::setLateDecayFactorHigh(float newLateDecayFactorHigh)
{
    // The high frequency gain for the late reverb time. (RTHi)
    late.setrt60_factor_high(newLateDecayFactorHigh);
}

void HallReverb::setLateDecayFactorLow(float newLateDecayFactorLow)
{
    // The low frequency gain for the late reverb time. (RTLo)
    late.setrt60_factor_low(newLateDecayFactorLow);
}

void HallReverb::setLateDiffusion(float newLateDiffusion)
{
    // The strength of the input allpass diffusor. (IDIF)
    late.setidiffusion1(newLateDiffusion);
}

void HallReverb::setLateLFO1Freq(float newLateLFO1Freq)
{
    // The first frequency of the LFO in the FDN loop. (LFO1)
    late.setlfo1freq(newLateLFO1Freq);
}

void HallReverb::setLateLFO2Freq(float newLateLFO2Freq)
{
    // The second frequency of the LFO in the FDN loop. (LFO2)
    late.setlfo2freq(newLateLFO2Freq);
}

void HallReverb::setLateLFOFactor(float newLateLFOFactor)
{
    // The strength of the LFO in the FDN loop. (LFOF)
    late.setlfofactor(newLateLFOFactor);
}

void HallReverb::setLateOutputHPF(float newLateOutputHPF)
{
    // The cutoff frequency of the high pass filter of the late reverb signal. (LHPF)
    late.setoutputhpf(newLateOutputHPF);
}

void HallReverb::setLateOutputLPF(float newLateOutputLPF)
{
    // The cutoff frequency of the low pass filter of the late reverb signal. (LLPF)
    late.setoutputlpf(newLateOutputLPF);
}

void HallReverb::setLatePredelay(float newLatePredelay)
{
    // The length of the initial delay of the late reverb wet signal in ms. (IDEL)
    latePredelay = newLatePredelay;
    latePredelayNeedsUpdate = true;
}

void HallReverb::setLateRoomSize(float newLateRoomSize)
{
    // The late reverb's room size. (SIZE)
    lateRoomSize = newLateRoomSize;
    lateRoomSizeNeedsUpdate = true;
}

void HallReverb::setLateSpin(float newLateSpin)
{
    // The frequency of the output chorus. (SPN)
    late.setspin(newLateSpin);
}

void HallReverb::setLateSpinFactor(float newLateSpinFactor)
{
    // The strength of the output chorus. (SPNF)
    late.setspinfactor(newLateSpinFactor);
}

void HallReverb::setLateStereoWidth(float newLateStereoWidth)
{
    // The stereo width of the late reverberation. (LWID)
    late.setwidth(newLateStereoWidth);
}

void HallReverb::setLateWander(float newLateWander)
{
    // The length of the output chorus. (WAN)
    late.setwander(newLateWander);
}
