/**
 * @file
 * @brief     PAF Application.
 * @author    Igor Lesik 2022
 * @copyright Igor Lesik 2022
 */
#include "PafApp.h"

#include "paf/lib/generate/Generator.h"
#include "paf/lib/generate/GeneratorFactory.h"

using namespace paf;

void
paf::Application::
play(const juce::ArgumentList& /*args*/)
{
    printf("play...\n");
}

void
paf::Application::
generate(const juce::ArgumentList& /*args*/)
{
    printf("generate...\n");

    auto src = paf::GeneratorFactory::make(paf::GeneratorFactory::SIN);
    if (src.get() == nullptr) {
        printf("failed to create generator\n");
        return;
    }

    paf::Generator generator(std::move(src));
}
