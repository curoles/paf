#pragma once

#include "JuceHeader.h"

class AudioComponent : public juce::AudioAppComponent,
                       public juce::ChangeListener
{
protected:
    // AudioFormatManager can create suitable objects for reading
    // audio data from different formats such as WAV, MP3 and so on.
    juce::AudioFormatManager formatManager_;

    // Handles the low-level file reading operations on the audio file.
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource_;

    // AudioTransportSource controls the playback of hte AudioFormatReaderSource object,
    // including starting and stopping the playback of the AudioFormatReaderSource object.
    // It can also perform sample rate conversion and it can buffer audio ahead of time if we wish.
    juce::AudioTransportSource transportSource_;

    enum TransportState
    {
        STOPPED,
        STARTING,
        PLAYING,
        PAUSING,
        PAUSED,
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
        if (readerSource_.get() == nullptr) // clear buffer if we do not have working reader
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

    // When changes in the transport are reported, the changeListenerCallback()
    // function will be called. This will be called asynchronously on the message thread.
    void changeListenerCallback(juce::ChangeBroadcaster* source) override
    {
        if (source == &transportSource_)
        {
            if (transportSource_.isPlaying())
                changeState(PLAYING);
            else if (state_ == STOPPING or state_ == PLAYING)
                changeState(STOPPED);
            else if (state_ == PAUSING)
                changeState(PAUSED);
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
            case PAUSING:
                transportSource_.stop();
                break;
            case PAUSED:
                break;
            case STOPPING:
                transportSource_.stop();
                break;
        }

        uiChangeState(newState);
    }

    virtual void uiChangeState(TransportState newState) = 0;
};
