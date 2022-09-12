#pragma once

#include "JuceHeader.h"

//#include <juce_gui_extra/juce_gui_extra.h>

/** Top level (root) component for all controls and content.
 *
 * Lives inside the main window.
 */
class MainComponent : public juce::Component
{
public:
    MainComponent();

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
