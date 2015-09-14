#pragma once
#include <string>
#include "Global.h"

class ErrorManager
{
public:
	/**
	Print a warning message, but the excecution of the program is not stopped.
	\Param The warning message.
	*/
	static void Warning(const std::string& message, const std::string& message2 = "");
	/**
	Print an error message, and exit with error code.
	@Param The error message.
	@Param The error number, 1 by default.
	*/
	static void Error(const std::string&message, const std::string& message2 = "", U32 error_number = 1);
};

