#include "Log.h"
#include <iostream>


namespace Ryno{

	void Log::Message(const std::string& message) {
		std::cout<< message << std::endl;
	}

	void Log::Warning(const std::string& message, const std::string& message2) {
		std::cout << "Warning: " + message << std::endl;
	}

	void Log::Error(const std::string& message, const std::string& message2) {
		std::cout << "Error: " + message << std::endl;
	}

	void Log::FatalError(const std::string& message, const std::string& message2, U32 error_number) {
		std::cout << "Fatal Error: " + message + message2 << std::endl;
		exit(error_number);
	}
}