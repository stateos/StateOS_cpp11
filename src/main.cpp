#include "stm32f4_discovery.h"
#include <thread>
#include <mutex>
#include <chrono>

using namespace std::chrono_literals;

thread_local auto delay = 1000ms;

void test()
{
	std::once_flag flg;
	std::jthread jt([&](std::stop_token st)
	{
		do std::call_once(flg, []{ delay = 1ms; });
		while (!st.stop_requested());
	});
}

int main()
{
	device::Led led;
	delay = 100ms;
	for (;;)
	{
		test();
		std::this_thread::sleep_for(delay);
		std::thread([&]{ led.tick(); }).detach();
	}
}
