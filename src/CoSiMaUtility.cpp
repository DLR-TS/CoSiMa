#include "CoSiMaUtility.h"

std::unique_ptr<grpc::ClientContext> CoSiMa::Utility::CreateDeadlinedClientContext(double transactionTimeout) {
	if (0 == transactionTimeout) {
		//prevent too short deadlines
		return std::make_unique<grpc::ClientContext>();
	}
	// double to integer conversion
	std::chrono::system_clock::duration timeout = std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::duration<double>(transactionTimeout));
	return CoSiMa::Utility::CreateDeadlinedClientContext(timeout);
}

std::unique_ptr<grpc::ClientContext> CoSiMa::Utility::CreateDeadlinedClientContext(uint32_t transactionTimeout) {
	if (0 == transactionTimeout) {
		//prevent too short deadlines
		return std::make_unique<grpc::ClientContext>();
	}
	//convert to systemclock time unit
	std::chrono::system_clock::duration timeout = std::chrono::milliseconds(transactionTimeout);
	// set deadline to transactionTimeout seconds from now
	return CoSiMa::Utility::CreateDeadlinedClientContext(timeout);
}

std::unique_ptr<grpc::ClientContext> CoSiMa::Utility::CreateDeadlinedClientContext(std::chrono::system_clock::duration transactionTimeout) {
	// context to handle a rpc call - cannot be reused
	auto context = std::make_unique<grpc::ClientContext>();
	//prevent too short deadlines
	if (std::chrono::system_clock::duration::min() < transactionTimeout) {
		// set deadline to transactionTimeout seconds from now
		context->set_deadline(std::chrono::system_clock::now() + transactionTimeout);
	}
	return context;
}