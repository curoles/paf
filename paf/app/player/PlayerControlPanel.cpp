/**
 * @file
 * @brief     Panel with player control buttons.
 * @author    Igor Lesik 2022
 * @copyright Igor Lesik 2022
 */
#include "PlayerControlPanel.h"

paf::PlayerControlPanel::
PlayerControlPanel()
{
    buttonPlay_.setButtonText("PLAY");
    buttonPlay_.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
    addAndMakeVisible(&buttonPlay_);

    buttonStop_.setButtonText("STOP");
    buttonStop_.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    addAndMakeVisible(&buttonStop_);

    buttonSkipBack_.setButtonText("<< SKIP");
    buttonSkipBack_.setColour(juce::TextButton::buttonColourId, juce::Colours::cornflowerblue);
    addAndMakeVisible(&buttonSkipBack_);

    buttonSkipForward_.setButtonText("SKIP >>");
    buttonSkipForward_.setColour(juce::TextButton::buttonColourId, juce::Colours::cornflowerblue);
    addAndMakeVisible(&buttonSkipForward_);
}

void
paf::PlayerControlPanel::
resized()
{
    auto area = getLocalBounds();
    auto areaW = getWidth();
    auto areaH = getHeight();
    auto height = areaH;

    buttonSkipBack_.setSize(70, height);
    buttonSkipBack_.setTopLeftPosition(0, 0);
    buttonPlay_.setSize(50, height);
    buttonPlay_.setTopLeftPosition(80, 0);
    buttonStop_.setSize(50, height);
    buttonStop_.setTopLeftPosition(140, 0);
    buttonSkipForward_.setSize(70, height);
    buttonSkipForward_.setTopLeftPosition(200, 0);
}