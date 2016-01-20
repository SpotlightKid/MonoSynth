/*
 * DISTRHO MonoSynth Plugin
 * Copyright (C) 2016 Christopher Arndt <chris@chrisarndt.de>
 *
 * Permission to use, copy, modify, and/or distribute this software for any purpose with
 * or without fee is hereby granted, provided that the above copyright notice and this
 * permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <math.h>
#include "DistrhoPluginMonoSynth.hpp"
#include "LFO.hpp"
#include "ADSR.hpp"
#include "WaveUtils.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

DistrhoPluginMonoSynth::DistrhoPluginMonoSynth()
    : Plugin(paramCount, 0, 0), // 0 programs, 0 states
      fVolume(0.1f),
      fSampleRate(getSampleRate())
{
    osc1 = sawOsc();
    lfo1 = new LFO(fSampleRate);
    env1 = new ADSR();
}

// -----------------------------------------------------------------------
// Init

void DistrhoPluginMonoSynth::initParameter(uint32_t index, Parameter& parameter) {
    if (index != 0)
        return;

    parameter.hints      = kParameterIsAutomable | kParameterIsLogarithmic;
    parameter.name       = "Volume";
    parameter.symbol     = "Volume";
    parameter.ranges.def = 0.1f;
    parameter.ranges.min = 0.0f;
    parameter.ranges.max = 1.0f;
}

// -----------------------------------------------------------------------
// Internal data

float DistrhoPluginMonoSynth::getParameterValue(uint32_t index) const {
    if (index != 0)
        return 0.0f;

    return fVolume;
}

void DistrhoPluginMonoSynth::setParameterValue(uint32_t index, float value) {
    if (index != 0)
        return;

    fVolume = value;
}

// -----------------------------------------------------------------------
// Process

void DistrhoPluginMonoSynth::activate() {
    env1->setAttackRate(0.1f * fSampleRate);
    env1->setDecayRate(0.3f * fSampleRate);
    env1->setReleaseRate(2.0f * fSampleRate);
    env1->setSustainLevel(0.8f);

    for (int i=0; i < 128; i++) {
        noteState[i] = false;
    }
}

void DistrhoPluginMonoSynth::run(const float**, float** outputs,
                                 uint32_t frames, const MidiEvent* midiEvents,
                                 uint32_t midiEventCount) {
    uint8_t note, velo;
    float* out = outputs[0];
    float freq;

    for (uint32_t count, pos=0, curEventIndex=0; pos<frames;) {
        for (;curEventIndex < midiEventCount && pos >= midiEvents[curEventIndex].frame; ++curEventIndex) {
            if (midiEvents[curEventIndex].size > MidiEvent::kDataSize)
                continue;

            const uint8_t* data = midiEvents[curEventIndex].data;
            const uint8_t status = data[0] & 0xF0;

            switch (status) {
                case 0x90:
                    note = data[1];
                    velo = data[2];
                    DISTRHO_SAFE_ASSERT_BREAK(note < 128);

                    if (noteState[note]) {
                        if (velo == 0) {
                            noteState[note] = false;
                            env1->gate(false);
                        }
                    }
                    else if (velo > 0) {
                        freq = 440.0f * powf(2.0f, (note - 69.0f) / 12.0f);
                        osc1->setFrequency(freq / fSampleRate);
                        noteState[note] = true;
                        env1->gate(true);
                        break;
                    }
                    break;
                case 0x80:
                    note = data[1];
                    DISTRHO_SAFE_ASSERT_BREAK(note < 128);

                    if (noteState[note]) {
                        noteState[note] = false;
                        env1->gate(false);
                    }
                    break;
            }
        }

        if (curEventIndex < midiEventCount && midiEvents[curEventIndex].frame < frames)
            count = midiEvents[curEventIndex].frame - pos;
        else
            count = frames - pos;

        for (uint32_t i=0; i<count; ++i) {
            out[pos + i] = osc1->getOutput() * env1->process() * fVolume;
            osc1->updatePhase();
        }

        pos += count;
    }
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new DistrhoPluginMonoSynth();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
