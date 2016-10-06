#pragma once

namespace Ryno{

	class Connection {
	public:
		bool want_read();
		bool want_write();
		bool do_read();
		bool do_write();

	private:
	};

}