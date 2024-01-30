/**
@authors German Aerospace Center: Björn Bahn, Nils Wendorff, Danny Behnecke
*/

#ifndef BASECONFIGVARIANTS_H
#define BASECONFIGVARIANTS_H

#if __has_include(<variant>)
	#include <variant>
#elif __has_include("boost/variant.hpp")
	#include "boost/variant.hpp"
#else
	error "Missing the <variant> header."
#endif
#include "reader/StandardYAMLConfig.h"

#if __has_include(<variant>)
	typedef std::variant<CARLAInterfaceConfig, DummyInterfaceConfig> baseConfigVariants_t;
#elif __has_include("boost/variant.hpp")
	typedef boost::variant<CARLAInterfaceConfig, DummyInterfaceConfig> baseConfigVariants_t;
#endif

#endif //!BASECONFIGVARIANTS_H
