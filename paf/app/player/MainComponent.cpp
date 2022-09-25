#include "MainComponent.h"

MainComponent::MainComponent()
{
    addAndMakeVisible(&buttonOpen_);
    buttonOpen_.setButtonText("Open...");
    buttonOpen_.onClick = [this] { buttonOpenClicked(); };

    addAndMakeVisible(&buttonPlay_);
    buttonPlay_.setButtonText("Play");
    buttonPlay_.onClick = [this] { buttonPlayClicked(); };
    buttonPlay_.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
    buttonPlay_.setEnabled(false);

    addAndMakeVisible(&buttonStop_);
    buttonStop_.setButtonText("Stop");
    buttonStop_.onClick = [this] { buttonStopClicked(); };
    buttonStop_.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    buttonStop_.setEnabled(false);

    addAndMakeVisible(&toggleLooping_);
    toggleLooping_.setButtonText("Loop");
    toggleLooping_.onClick = [this] { toggleLoopChanged(); };

    addAndMakeVisible(&currentPositionLabel_);
    currentPositionLabel_.setText ("Stopped", juce::dontSendNotification);

    addAndMakeVisible(&playerControl_);

    setSize(600, 400);

    startTimer(20);
}

void
MainComponent::
paint(juce::Graphics& g)
{
    juce::AudioAppComponent::paint(g);
#if 0
    // Fill the background with a solid colour
    auto bgColor = getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId);
    g.fillAll(bgColor);

    g.setFont(juce::Font (16.0f));
    g.setColour(juce::Colours::white);
    g.drawText("Hello World!", getLocalBounds(), juce::Justification::centred, true);
#endif
}

void
MainComponent::
resized()
{
    buttonOpen_.setBounds(10, 10, getWidth() - 20, 20);
    buttonPlay_.setBounds(10, 40, getWidth() - 20, 20);
    buttonStop_.setBounds(10, 70, getWidth() - 20, 20);
    toggleLooping_.setBounds (10, 100, getWidth() - 20, 20);
    currentPositionLabel_.setBounds(10, 130, getWidth() - 20, 20);

    auto area = getLocalBounds();

    auto playerControlPanelHeight = 40;
    playerControl_.setBounds(area.removeFromBottom(playerControlPanelHeight));
}

void
MainComponent::
buttonOpenClicked()
{
    fileChooser_ = std::make_unique<juce::FileChooser> (
        "Select a file to play...",
        juce::File{},
        "*.wav,*.mp3");

    auto chooserFlags = juce::FileBrowserComponent::openMode
                      | juce::FileBrowserComponent::canSelectFiles;

    fileChooser_->launchAsync(chooserFlags, [this] (const FileChooser& fc)
    {
        auto file = fc.getResult();

        if (file != File{})
        {
            auto* reader = formatManager_.createReaderFor(file);

            if (reader != nullptr)
            {
                auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
                transportSource_.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
                buttonPlay_.setEnabled(true);
                readerSource_.reset(newSource.release());
            }
        }
    });

}

void
MainComponent::
buttonPlayClicked()
{
    if (state_ == STOPPED or state_ == PAUSED) {
        changeState(STARTING);
        updateLoopState(toggleLooping_.getToggleState());
    }
    else if (state_ == PLAYING)
        changeState(PAUSING);
}

void
MainComponent::
buttonStopClicked()
{
    if (state_ == PAUSED)
        changeState(STOPPED);
    else
        changeState(STOPPING);
}

void
MainComponent::
toggleLoopChanged()
{
    updateLoopState(toggleLooping_.getToggleState());
}

void
MainComponent::
uiChangeState(TransportState newState)
{
    switch (newState)
    {
        case STOPPED:
            buttonStop_.setEnabled(false);
            buttonStop_.setButtonText("Stop");
            buttonPlay_.setEnabled(true);
            buttonPlay_.setButtonText("Play");
            break;
        case STARTING:
            break;
        case PLAYING:
            buttonStop_.setEnabled(true);
            buttonStop_.setEnabled("Stop");
            buttonPlay_.setButtonText("Pause");
            break;
        case PAUSING:
            break;
        case PAUSED:
            buttonPlay_.setButtonText("Resume");
            buttonStop_.setButtonText("Return to start");
            break;
        case STOPPING:
            break;
    }
}

void
MainComponent::
timerCallback()
{
    if (transportSource_.isPlaying())
    {
        juce::RelativeTime position(transportSource_.getCurrentPosition());

        auto minutes = ((int) position.inMinutes()) % 60;
        auto seconds = ((int) position.inSeconds()) % 60;
        auto millis  = ((int) position.inMilliseconds()) % 1000;

        auto positionString = juce::String::formatted("%02d:%02d:%03d", minutes, seconds, millis);

        currentPositionLabel_.setText(positionString, juce::dontSendNotification);
    }
    else
    {
        currentPositionLabel_.setText("Stopped", juce::dontSendNotification);
    }
}

void
MainComponent::
updateLoopState(bool shouldLoop)
{
    if (readerSource_.get() != nullptr) {
        readerSource_->setLooping(shouldLoop);
    }
}

