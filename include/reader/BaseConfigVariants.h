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

#if defined(_WIN32) && (_MSC_VER >= 1910) || defined(__linux__) && __cplusplus >= 201703L
	typedef std::variant<CARLAInterfaceConfig, DummyInterfaceConfig> baseConfigVariants_t;
#elif defined(_WIN32) && (_MSC_VER >= 1600) || defined(__linux__) && __cplusplus >= 201103L
	typedef boost::variant<CARLAInterfaceConfig, DummyInterfaceConfig> baseConfigVariants_t;
#endif

#endif //!BASECONFIGVARIANTS_H
