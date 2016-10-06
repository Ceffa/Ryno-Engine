#pragma once
#include "Global.h"
#include <sstream>
#include <WinSock2.h>
#include <codecvt>
#include "Log.h"


namespace Ryno{

	static class NetUtil {
	
	public:

		static U32 compress_ip(std::string s);

		static std::string decompress_ip(U32 _ip);

		static std::string get_error();

		static void print(std::string s);

		static void print_error(std::string s);

	};
}