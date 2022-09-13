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

    setSize(600, 400);
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
    changeState(STARTING);
}

void
MainComponent::
buttonStopClicked()
{
    changeState(STOPPING);
}

void
MainComponent::
uiChangeState(TransportState newState)
{
    switch (newState)
    {
        case STOPPED:
            buttonStop_.setEnabled(false);
            buttonPlay_.setEnabled(true);
            break;

        case STARTING:
            buttonPlay_.setEnabled(false);
            break;

        case PLAYING:
            buttonStop_.setEnabled(true);
            break;

        case STOPPING:
            break;
    }
}
