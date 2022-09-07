/**
 * @file
 * @brief     PAF Application.
 * @author    Igor Lesik 2022
 * @copyright Igor Lesik 2022
 */
#include "JuceHeader.h"

namespace paf {

class Application
{
public:
    void play(const juce::ArgumentList& args);
    void generate(const juce::ArgumentList& args);
};

} // namespace paf
