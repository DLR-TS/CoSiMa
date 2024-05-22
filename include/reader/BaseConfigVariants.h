/**
@authors German Aerospace Center: Björn Bahn, Nils Wendorff, Danny Behnecke
*/

#ifndef BASECONFIGVARIANTS_H
#define BASECONFIGVARIANTS_H

#if defined(_WIN32) && (_MSC_VER >= 1910) || defined(__linux__) && __cplusplus >= 201703L
	#include <variant>
#elif defined(_WIN32) && (_MSC_VER >= 1600) || defined(__linux__) && __cplusplus >= 201103L
	#include "boost/variant.hpp"
#endif
#include "reader/StandardYAMLConfig.h"

#if defined(_WIN32) && (_MSC_VER >= 1910) || defined(__linux__) && __cplusplus >= 201703L
	typedef std::variant<CARLAInterfaceConfig, DummyInterfaceConfig> baseConfigVariants_t;
#elif defined(_WIN32) && (_MSC_VER >= 1600) || defined(__linux__) && __cplusplus >= 201103L
	typedef boost::variant<CARLAInterfaceConfig, DummyInterfaceConfig> baseConfigVariants_t;
#endif

#endif //!BASECONFIGVARIANTS_H
