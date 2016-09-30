#include "Socket.h"
#include "Log.h"
#include <sstream>
#include <codecvt>


namespace Ryno {

	void Socket::print(std::string s) {
		Log::println(s);
	}
	void Socket::print_error(std::string s) {
		Log::println(s + get_error());
	}

	U32 Socket::compress_ip(std::string s) {
		
			std::vector<U32> split;
			std::stringstream ss(s); // Turn the string into a stream.
			std::string tok;

			while (std::getline(ss, tok, '.')) {
				split.push_back(std::stoi(tok));
			}
			if (split.size() != 4) {
				print("Ip address not valid.");
				return 0;
			}
			return (split[0] << 24) + (split[1] << 16) + (split[2] << 8) + split[3];
			
	}
	
	std::string Socket::decompress_ip(U32 _ip) {
		std::string s;
		std::stringstream ss(s);
		
		U32 mod = (1 << 8);
		ss << (_ip >> 24);
		ss << ".";
		ss << (_ip >> 16)% mod;
		ss << ".";
		ss << (_ip >> 8) % mod;
		ss << ".";
		ss << _ip % mod;

		ss >> s;

		return s;
	}

	std::string Socket::get_server_ip() {
		return decompress_ip(server_ip);
	}

	void Socket::set_server_ip(std::string s) {
		server_ip = compress_ip(s);
	}

	U32 Socket::get_server_port() {
		return server_port;
	}
	void Socket::set_server_port(U32 _port) {
		server_port = _port;
	}

	std::string Socket::get_error() {
		wchar_t *s = NULL;
		FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, WSAGetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&s, 0, NULL);
		std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
		std::string str = std::string(myconv.to_bytes(s));
		return str.substr(0, str.size() - 1);
	}
}