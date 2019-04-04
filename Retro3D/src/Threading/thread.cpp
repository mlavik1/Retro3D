#include "thread.h"

namespace Retro3D
{
	void Thread::Execute()
	{
		auto lambda = [=](void)  -> void
		{
			mFunction();
		};

		mThread = std::thread(lambda);

	}

	void Thread::WaitForThread()
	{
		mThread.join();
	}
}
