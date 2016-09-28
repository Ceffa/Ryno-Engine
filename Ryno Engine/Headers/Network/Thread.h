#pragma once
#include <thread>

namespace Ryno{

	class Thread {
	private:

		std::thread* thread;

	public:

		template<class _Fn, class _Arg>
			void create(_Fn&& _Fx, _Arg&& _Ax) {
			thread = new std::thread(_Fx, _Ax);
		}

		void join() {
			thread->join();
		}
		~Thread() {
			delete thread;
		}

	};
}