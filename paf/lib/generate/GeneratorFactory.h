/**
 * @file
 * @brief     Factory to make generator.
 * @author    Igor Lesik 2022
 * @copyright Igor Lesik 2022
 */
#pragma once

#include "paf/lib/generate/Generator.h"
#include "paf/lib/generate/SinGenerator.h"
#include "paf/lib/generate/WhiteNoiseGenerator.h"

namespace paf {

struct GeneratorFactory
{
    enum Type {
        NONE, SIN, WHITE_NOISE
    };

    static
    std::unique_ptr<GeneratorSource> make(Type type) {
        switch (type) {
            case SIN: return std::make_unique<SinGenerator>();
            case WHITE_NOISE: return std::make_unique<WhiteNoiseGenerator>();
            case NONE: break;
            default: break;
        }

        return std::unique_ptr<GeneratorSource>(nullptr);
    }

};


} // namespace paf
