#pragma once
#ifndef COSIMAUTILITY_H
#define COSIMAUTILITY_H


#include <chrono>

#include <grpcpp/client_context.h>

namespace CoSiMa::Utility {
	/**
	create a grpc client context for a rpc that times out transactionTimeout seconds from now.

	client contexts can not be reused. They don't support timeouts, so the timeout is set as a deadline transactionTimeout seconds from now.
	\var transactionTimout amount of time in seconds to set before reaching the deadline (deadline = time.now + transactionTimeout)
	*/
	std::unique_ptr<grpc::ClientContext> CreateDeadlinedClientContext(double transactionTimeout);
	/**
	create a grpc client context for a rpc that times out transactionTimeout seconds from now.

	client contexts can not be reused. They don't support timeouts, so the timeout is set as a deadline transactionTimeout seconds from now.
	\var transactionTimout amount of time in milliseconds to set before reaching the deadline (deadline = time.now + transactionTimeout)
	*/
	std::unique_ptr<grpc::ClientContext> CreateDeadlinedClientContext(long long transactionTimeout);
	/**
	create a grpc client context for a rpc that times out transactionTimeout seconds from now.

	client contexts can not be reused. They don't support timeouts, so the timeout is set as a deadline transactionTimeout seconds from now.
	\var transactionTimout amount of time to set before reaching the deadline (deadline = time.now + transactionTimeout)
	*/
	std::unique_ptr<grpc::ClientContext> CreateDeadlinedClientContext(std::chrono::system_clock::duration transactionTimeout);
};
#endif // !COSIMAUTILITY_H