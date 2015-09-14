#include "ErrorManager.h"
#include <iostream>

void ErrorManager::Warning(const std::string& message, const std::string& message2) {
	std::cout << "Warning: " + message << std::endl;
}

void ErrorManager::Error(const std::string& message, const std::string& message2, U32 error_number) {
	std::cout << "Error: " + message + message2  << std::endl;
	exit(error_number);
}
