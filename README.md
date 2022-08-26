# Hall Reverb Prototype
Hall Reverb is a mono/stereo to stereo algorithmic reverb audio plugin. It uses the implementation of Moorer's early reflection model and a simple FDN reverb based on zita-rev1 from the Freeverb3 signal processing library.

## Disclaimer
- ⚠️ This plugin is only a prototype! ⚠️
- It may be unstable or generate noise at certain parameter values.
- The current parameter ranges have been chosen more or less randomly and may not make sense sometimes, but can of course be changed.

## Plugin Parameters
| Parameter | Freeverb Name | Description |
| --- | --- | --- |
| dryLevel | DRY | The level of the dry signal. |
| earlyLevel | EWET | The level of the early reflection signal. |
| earlySendLevel | ESEN | The level of the early reflection signal which was send to the late reverberation. |
| lateLevel | LWET | The level of the late reverberation signal. |
| earlyOutputHPF | EHPF | The cutoff frequency of the high pass filter of the early reflection signal. |
| earlyOutputLPF | ELPF | The cutoff frequency of the low pass filter of the early reflection signal. |
| earlyRoomSize | EFAC | The room size of the early reflection. |
| earlyStereoWidth | EWID | The stereo width of the early reflection. |
| lateApFeedback | ADIF | The strength of the allpass diffusor in the FDN loop. |
| lateCrossOverFreqHigh | XOH | The high crossover frequency for the late reverb time. |
| lateCrossOverFreqLow | XOL | The low crossover frequency for the late reverb time. |
| lateDecay | RT60 | The reverb time. |
| lateDecayFactorHigh | RTHi | The high frequency gain for the late reverb time. |
| lateDecayFactorLow | RTLo | The low frequency gain for the late reverb time. |
| lateDiffusion | IDIF | The strength of the input allpass diffusor. |
| lateLFO1Freq | LFO1 | The first frequency of the LFO in the FDN loop. |
| lateLFO2Freq | LFO2 | The second frequency of the LFO in the FDN loop. |
| lateLFOFactor | LFOF | The strength of the LFO in the FDN loop. |
| lateOutputHPF | LHPF | The cutoff frequency of the high pass filter of the late reverb signal. |
| lateOutputLPF | LLPF | The cutoff frequency of the low pass filter of the late reverb signal. |
| latePredelay | IDEL | The length of the initial delay of the late reverb wet signal in ms. |
| lateRoomSize | SIZE | The late reverb's room size. |
| lateSpin | SPN | The frequency of the output chorus. |
| lateSpinFactor | SPNF | The strength of the output chorus. |
| lateStereoWidth | LWID | The stereo width of the late reverberation. |
| lateWander | WAN | The length of the output chorus. |

## Next steps
- [ ] Decide which parameters should be visible in the final GUI.
- [ ] Decide what to do with parameters that are not visible in the final GUI (set to a fixed value or set depending on other parameters?).
- [ ] Decide on the final parameter ranges.
- [ ] Decide on parameter range transformations (0-1 -> 0-100% etc.)
- [ ] Create presets for different halls.
- [ ] Implement final GUI.

## Download
The latest prototype is available [here](https://github.com/cvde/HallReverb/releases).

## Building Hall Reverb

Hall Reverb uses the JUCE C++ framework. If you are on Linux, you need to install [these dependencies](https://github.com/juce-framework/JUCE/blob/master/docs/Linux%20Dependencies.md) first.
```bash
# Get the repository including submodules
git clone --recurse-submodules https://github.com/cvde/HallReverb.git
cd HallReverb

# Build plugin with CMake
cmake -DCMAKE_BUILD_TYPE=Release -B build
cmake --build build
```
After a successful build, the plugin binaries can be found in `build/HallReverb_artefacts`.

## References
- [Freeverb3 signal processing library](https://www.nongnu.org/freeverb3/)
- [Freeverb3VST](https://freeverb3vst.osdn.jp/)
- [Freeverb3 Hibiki Reverb Parameter reference](https://freeverb3vst.osdn.jp/tutorial/tutorial_vst_hibiki.shtml)

## Licenses
Hall Reverb is free software and distributed under the [GNU General Public License Version 3](LICENSE).

Freeverb3 is free software and distributed under the [GNU General Public License Version 2](Freeverb3/COPYING).

JUCE is dual licenced under both the JUCE licence and the GNU General Public License Version 3.

<img src="Assets/VST_Compatible_Logo_Steinberg_with_TM_negative.svg" alt="Steinberg VST compatible logo" width="250" height="232"/>

All other trademarks, registered trademarks and logos or other manufacturers written or shown herein are the property of their respective owners. Use of them does not imply any affiliation with or endorsement by them.