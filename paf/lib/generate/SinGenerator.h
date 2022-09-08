/**
 * @file
 * @brief     Sine wave generator.
 * @author    Igor Lesik 2022
 * @copyright Igor Lesik 2022
 */
#pragma once

#include "paf/lib/generate/Generator.h"

namespace paf {


class SinGenerator : public paf::GeneratorSource
{
    juce::ToneGeneratorAudioSource toneGen_;

public:
    SinGenerator():
        GeneratorSource()
    {
    }

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override {
        toneGen_.prepareToPlay(samplesPerBlockExpected, sampleRate);
    }

    void releaseResources() override {
        toneGen_.releaseResources();
    }

    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override {
        toneGen_.getNextAudioBlock(bufferToFill);
    }
};


} // namespace paf
