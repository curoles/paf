#include "MainComponent.h"

MainComponent::MainComponent()
{
    setSize(600, 400);
}

void MainComponent::paint (juce::Graphics& g)
{
    // Fill the background with a solid colour
    auto bgColor = getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId);
    g.fillAll(bgColor);

    g.setFont(juce::Font (16.0f));
    g.setColour(juce::Colours::white);
    g.drawText("Hello World!", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
