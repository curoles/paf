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
    std::atomic<float> amplitude_{1.0};

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

    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override
    {
        for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
        {
            auto* buffer = getBufferWrPtrStart(channel, bufferToFill);

            // Fill the required number of samples with noise
            for (auto sample = 0; sample < bufferToFill.numSamples; ++sample) {
                buffer[sample] = random_.nextFloat() * amplitude_ * 2 - amplitude_;
            }
        }
    }

    void setAmplitude(float newAmplitude) override {
        amplitude_ = newAmplitude;
    }

    void setFrequency(double /*newFrequencyHz*/) override {
    }

};


} // namespace paf
