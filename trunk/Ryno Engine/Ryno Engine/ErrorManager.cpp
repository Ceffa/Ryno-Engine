#include "ErrorManager.h"
#include <iostream>

void ErrorManager::Warning(const std::string& message) {
	std::cout << "Warning: " + message << std::endl;
}

void ErrorManager::Error(const std::string& message, U32 error_number) {
	std::cout << "Error: " + message << std::endl;
	exit(error_number);
}
