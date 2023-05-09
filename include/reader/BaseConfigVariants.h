/**
@authors German Aerospace Center: Björn Bahn, Nils Wendorff, Danny Behnecke
*/

#ifndef BASECONFIGVARIANTS_H
#define BASECONFIGVARIANTS_H

#include <variant>
#include "reader/StandardYAMLConfig.h"

typedef std::variant<CARLAInterfaceConfig, DummyInterfaceConfig> baseConfigVariants_t;

#endif //!BASECONFIGVARIANTS_H