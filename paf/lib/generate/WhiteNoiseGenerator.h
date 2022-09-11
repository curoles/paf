/**
 * @file
 * @brief     Sine wave generator.
 * @author    Igor Lesik 2022
 * @copyright Igor Lesik 2022
 *
 */
#pragma once

#include "paf/lib/generate/Generator.h"

namespace paf {


class WhiteNoiseGenerator : public paf::GeneratorSource
{
    juce::Random random_;

public:
    WhiteNoiseGenerator():
        GeneratorSource()
    {
    }

    void prepareToPlay(int /*samplesPerBlockExpected*/, double /*sampleRate*/) override {
    }

    void releaseResources() override {
    }

    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override {
        (void)bufferToFill;
    }

    void setAmplitude(float /*newAmplitude*/) override {
    }

    void setFrequency(double /*newFrequencyHz*/) override {
    }

};


} // namespace paf
