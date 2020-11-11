#include "CoSiMaUtility.h"

std::unique_ptr<grpc::ClientContext> CoSiMa::Utility::CreateDeadlinedClientContext(double transactionTimeout) {
	// double to integer conversion
	std::chrono::system_clock::duration timeout = std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::duration<double>(transactionTimeout));
	return CoSiMa::Utility::CreateDeadlinedClientContext(timeout);
}

std::unique_ptr<grpc::ClientContext> CoSiMa::Utility::CreateDeadlinedClientContext(uint32_t transactionTimeout) {
	//convert to systemclock time unit
	std::chrono::system_clock::duration timeout = std::chrono::milliseconds(transactionTimeout);
	// set deadline to transactionTimeout seconds from now
	return CoSiMa::Utility::CreateDeadlinedClientContext(timeout);
}

std::unique_ptr<grpc::ClientContext> CoSiMa::Utility::CreateDeadlinedClientContext(std::chrono::system_clock::duration transactionTimeout) {
	// context to handle a rpc call - cannot be reused
	auto context = std::make_unique<grpc::ClientContext>();
	// set deadline to transactionTimeout seconds from now
	context->set_deadline(std::chrono::system_clock::now() + transactionTimeout);
	return context;
}