/**
@authors German Aerospace Center: Bj?rn Bahn, Nils Wendorff, Danny Behnecke
*/

#ifndef BASECONFIGVARIANTS_H
#define BASECONFIGVARIANTS_H

#include <variant>
#include <string>
#include "reader/StandardYAMLConfig.h"

typedef std::variant<CARLAInterfaceConfig> baseConfigVariants_t;

#endif //!BASECONFIGVARIANTS_H