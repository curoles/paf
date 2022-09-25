/**
 * @file
 * @brief     Panel with player control buttons.
 * @author    Igor Lesik 2022
 * @copyright Igor Lesik 2022
 */
#pragma once

#include "JuceHeader.h"

namespace paf {

class PlayerControlPanel : public juce::/*Group*/Component
{
	juce::TextButton buttonPlay_;
	juce::TextButton buttonStop_;

	juce::TextButton buttonSkipBack_;
	juce::TextButton buttonSkipForward_;

public:
	PlayerControlPanel();

	void resized() override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerControlPanel)
};

} // namespace paf