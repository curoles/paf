#pragma once

#include "JuceHeader.h"

#include "AudioComponent.h"

//#include <juce_gui_extra/juce_gui_extra.h>

/** Top level (root) component for all controls and content.
 *
 * Lives inside the main window.
 */
class MainComponent : public AudioComponent
{
    juce::TextButton buttonOpen_;
    juce::TextButton buttonPlay_;
    juce::TextButton buttonStop_;

    std::unique_ptr<juce::FileChooser> fileChooser_;

public:
    MainComponent();

    void paint(juce::Graphics&) override;
    void resized() override;

private:

    void uiChangeState(TransportState newState) override;

    void buttonOpenClicked();
    void buttonPlayClicked();
    void buttonStopClicked();


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
