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

#ifndef DISTRHO_PLUGIN_MONOSYNTH_HPP_INCLUDED
#define DISTRHO_PLUGIN_MONOSYNTH_HPP_INCLUDED

#include "DistrhoPlugin.hpp"
#include "LFO.hpp"
#include "ADSR.hpp"
#include "WaveUtils.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class DistrhoPluginMonoSynth : public Plugin
{
public:
    enum Parameters {
        paramVolume = 0,
        paramCount
    };

    DistrhoPluginMonoSynth();

protected:
    // -------------------------------------------------------------------
    // Information

    const char* getLabel() const noexcept override {
        return "MonoSynth";
    }

    const char* getDescription() const override {
        return "Simple virtual analog mono synth.";
    }

    const char* getMaker() const noexcept override {
        return "Chris Arndt";
    }

    const char* getHomePage() const override {
        return "http://chrisarndt.de/plugins/MonoSynth";
    }

    const char* getLicense() const noexcept override {
        return "MIT";
    }

    uint32_t getVersion() const noexcept override {
        return d_version(1, 0, 0);
    }

    int64_t getUniqueId() const noexcept override {
        return d_cconst('C', 'A', 'M', 'S');
    }

    // -------------------------------------------------------------------
    // Init

    void initParameter(uint32_t index, Parameter& parameter) override;

    // -------------------------------------------------------------------
    // Internal data

    float getParameterValue(uint32_t index) const override;
    void  setParameterValue(uint32_t index, float value) override;

    // -------------------------------------------------------------------
    // Process

    void activate() override;
    void run(const float**, float** outputs, uint32_t frames,
             const MidiEvent* midiEvents, uint32_t midiEventCount) override;

    // -------------------------------------------------------------------

private:
    float    fVolume;
    double   fSampleRate;
    bool     noteState[128];
    WaveTableOsc    *osc1;
    LFO             *lfo1;
    ADSR            *env1;

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DistrhoPluginMonoSynth)
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // DISTRHO_PLUGIN_MONOSYNTH_HPP_INCLUDED
