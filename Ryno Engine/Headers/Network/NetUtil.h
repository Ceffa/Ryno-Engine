#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Global.h"
#include <sstream>
#include <WinSock2.h>
#include <codecvt>
#include "Log.h"



namespace Ryno{

	//Static utility class for several network functions
	static class NetUtil {
	
	public:

		//Pack and unpack an ip in a U32
		static U32 compress_ip(std::string s);
		static std::string decompress_ip(U32 _ip);

		//Print an error string from winsock
		static std::string get_error();

		//Logging functions.
		static void print(std::string s);
		static void print(I32 n);
		static void print(F32 n);
		static void print_error(std::string s);
		static I32 error_limit;	//Avoid flood of errors

	};
}