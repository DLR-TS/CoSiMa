cmake_minimum_required(VERSION 3.12)
include(FetchContent)

FetchContent_Declare(
  gRPC
  GIT_REPOSITORY https://github.com/grpc/grpc
  GIT_TAG        v1.31.1
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE
  # all submodules except third_party/re2, because it fails to clone
  GIT_SUBMODULES third_party/zlib third_party/protobuf third_party/gflags third_party/googletest third_party/benchmark third_party/boringssl-with-bazel third_party/cares/cares third_party/bloaty third_party/abseil-cpp third_party/envoy-api third_party/googleapis third_party/protoc-gen-validate third_party/udpa third_party/libuv
  LOG_CONFIGURE TRUE
  LOG_BUILD TRUE
  LOG_INSTALL TRUE
)

function(fetch_gRPC_and_non_conan_dependencies)
	set(FETCHCONTENT_QUIET OFF)
	# not available before CMake version 3.14 - Using FetchContent_Populate instead
	#FetchContent_MakeAvailable(gRPC)

	FetchContent_GetProperties(gRPC)
	if(NOT grpc_POPULATED)
		FetchContent_Populate(gRPC)

		if(TARGET zlib OR TARGET CONAN_PKG::zlib)
			set(gRPC_ZLIB_PROVIDER "package" CACHE STRING "Provider of zlib library")
			message(VERBOSE "Using existing zlib provider")
		else()
			set(gRPC_ZLIB_PROVIDER "module" CACHE STRING "Provider of zlib library")
		endif()

		if(TARGET protobuf OR TARGET CONAN_PKG::protobuf)
			set(gRPC_PROTOBUF_PROVIDER "package" CACHE STRING "Provider of protobuf library")
			message(VERBOSE "Using existing protobuf provider")
		else()
			set(gRPC_PROTOBUF_PROVIDER "module" CACHE STRING "Provider of protobuf library")
		endif()

		# submodule is broken/cannot be cloned using DLR VPN
		#if(TARGET re2 OR TARGET CONAN_PKG::re2)
		#	set(gRPC_RE2_PROVIDER "package" CACHE STRING "Provider of re2 library")
		#	message(VERBOSE "Using existing re2 provider")
		#else()
		#	set(gRPC_RE2_PROVIDER "module" CACHE STRING "Provider of re2 library")
		#endif()
		set(RE2_ROOT_DIR ${CONAN_RE2_ROOT})
		message("RE2_ROOT_DIR: ${CONAN_RE2_ROOT}")

		# deactivate abseil-cpp option for building tests
		set(BUILD_TESTING FALSE)

		message(STATUS "Fetched gRPC to ${grpc_SOURCE_DIR}. Will put build results into ${grpc_BINARY_DIR}.")
		add_subdirectory(${grpc_SOURCE_DIR} ${grpc_BINARY_DIR} EXCLUDE_FROM_ALL)
	endif()
endfunction()

fetch_gRPC_and_non_conan_dependencies()