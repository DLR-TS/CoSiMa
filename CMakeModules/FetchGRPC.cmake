cmake_minimum_required(VERSION 3.12)
include(FetchContent)

FetchContent_Declare(
  gRPC
  GIT_REPOSITORY https://github.com/grpc/grpc
  GIT_TAG        v1.31.0
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE
)
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

	# deactivate abseil-cpp option for building tests
	set(BUILD_TESTING FALSE)

	message(STATUS "Fetched gRPC to ${grpc_SOURCE_DIR}. Will put build results into ${grpc_BINARY_DIR}.")
	add_subdirectory(${grpc_SOURCE_DIR} ${grpc_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()