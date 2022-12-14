#pragma once

#include "JuceHeader.h"

#include "AudioComponent.h"
#include "PlayerControlPanel.h"

//#include <juce_gui_extra/juce_gui_extra.h>

/** Top level (root) component for all controls and content.
 *
 * Lives inside the main window.
 */
class MainComponent : public AudioComponent,
                      public juce::Timer
{
    juce::TextButton buttonOpen_;
    juce::TextButton buttonPlay_;
    juce::TextButton buttonStop_;

    juce::ToggleButton toggleLooping_;
    juce::Label currentPositionLabel_;

    paf::PlayerControlPanel playerControl_;

    std::unique_ptr<juce::FileChooser> fileChooser_;

public:
    MainComponent();

    void paint(juce::Graphics&) override;
    void resized() override;

    void timerCallback() override;

private:

    void uiChangeState(TransportState newState) override;

    void buttonOpenClicked();
    void buttonPlayClicked();
    void buttonStopClicked();
    void toggleLoopChanged();

    void updateLoopState(bool shouldLoop);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
