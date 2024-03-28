#include "CoSiMaUtility.h"

std::unique_ptr<grpc::ClientContext> createDefaultContext() {
#if defined(_WIN32) && (_MSC_VER >= 1910) || defined(__linux__) && __cplusplus >= 201703L
	return std::make_unique<grpc::ClientContext>();
#elif defined(_WIN32) && (_MSC_VER >= 1600) || defined(__linux__) && __cplusplus >= 201103L
	return std::unique_ptr<grpc::ClientContext>(new grpc::ClientContext);
#endif
}

std::unique_ptr<grpc::ClientContext> CoSiMa::Utility::CreateDeadlinedClientContext(double transactionTimeout) {
	if (0 == transactionTimeout) {
		//prevent too short deadlines
		return createDefaultContext();
	}
	// double to integer conversion
	std::chrono::system_clock::duration timeout = std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::duration<double>(transactionTimeout));
	return CoSiMa::Utility::CreateDeadlinedClientContext(timeout);
}

std::unique_ptr<grpc::ClientContext> CoSiMa::Utility::CreateDeadlinedClientContext(uint32_t transactionTimeout) {
	if (0 == transactionTimeout) {
		//prevent too short deadlines
		return createDefaultContext();
	}
	//convert to systemclock time unit
	std::chrono::system_clock::duration timeout = std::chrono::milliseconds(transactionTimeout);
	// set deadline to transactionTimeout seconds from now
	return CoSiMa::Utility::CreateDeadlinedClientContext(timeout);
}

std::unique_ptr<grpc::ClientContext> CoSiMa::Utility::CreateDeadlinedClientContext(std::chrono::system_clock::duration transactionTimeout) {
	// context to handle a rpc call - cannot be reused
	std::unique_ptr<grpc::ClientContext> context = createDefaultContext();
	//prevent too short deadlines
	if (std::chrono::system_clock::duration::min() < transactionTimeout) {
		// set deadline to transactionTimeout seconds from now
		context->set_deadline(std::chrono::system_clock::now() + transactionTimeout);
	}
	return context;
}
