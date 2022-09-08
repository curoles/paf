/**
 * @file
 * @brief     Factory to make generator.
 * @author    Igor Lesik 2022
 * @copyright Igor Lesik 2022
 */
#pragma once

#include "paf/lib/generate/Generator.h"
#include "paf/lib/generate/SinGenerator.h"

namespace paf {

struct GeneratorFactory
{
    enum Type {
        NONE, SIN
    };

    static
    std::unique_ptr<GeneratorSource> make(Type type) {
        switch (type) {
            case SIN: return std::make_unique<SinGenerator>();
            case NONE: break;
            default: break;
        }

        return std::unique_ptr<GeneratorSource>(nullptr);
    }

};


} // namespace paf
