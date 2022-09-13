#pragma once

#include "JuceHeader.h"

class AudioComponent : public juce::AudioAppComponent,
                       public juce::ChangeListener
{
protected:
    juce::AudioFormatManager formatManager_;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource_;
    juce::AudioTransportSource transportSource_;

    enum TransportState
    {
        STOPPED,
        STARTING,
        PLAYING,
        STOPPING
    };

    TransportState state_{STOPPED};

public:
    AudioComponent():state_{STOPPED}
    {
        formatManager_.registerBasicFormats();
        transportSource_.addChangeListener(this);

        setAudioChannels(/*IN*/0, /*OUT*/2);
    }

    ~AudioComponent() override
    {
        shutdownAudio();
    }

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
    {
        transportSource_.prepareToPlay(samplesPerBlockExpected, sampleRate);
    }

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        if (readerSource_.get() == nullptr)
        {
            bufferToFill.clearActiveBufferRegion();
            return;
        }

        transportSource_.getNextAudioBlock(bufferToFill);
    }

    void releaseResources() override
    {
        transportSource_.releaseResources();
    }

    void changeListenerCallback(juce::ChangeBroadcaster* source) override
    {
        if (source == &transportSource_)
        {
            if (transportSource_.isPlaying())
                changeState(PLAYING);
            else
                changeState(STOPPED);
        }
    }

    void changeState(TransportState newState)
    {
        if (state_ == newState) return;

        state_ = newState;

        switch (state_)
        {
            case STOPPED:
                transportSource_.setPosition(0.0);
                break;

            case STARTING:
                transportSource_.start();
                break;

            case PLAYING:
                break;

            case STOPPING:
                transportSource_.stop();
                break;
        }

        uiChangeState(newState);
    }

    virtual void uiChangeState(TransportState newState) = 0;
};
