#include <thread>
#include <functional>

namespace Retro3D
{
	class Thread
	{
	private:
		std::function<void()> mFunction;
		std::thread mThread;

	public:
		void Execute();
		void WaitForThread();

		template <typename Functor>
		Thread(Functor arg_function)
		{
			mFunction = arg_function;
		}


	};
}
