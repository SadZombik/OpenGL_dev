#define STB_IMAGE_IMPLEMENTATION


#include <thread>
#include <chrono>
#include "Renderer.h"

int main()
{
	LOG("main thread " << std::this_thread::get_id());
	
	std::thread render_thread(Renderer::Loop);
	LOG("render thread " << render_thread.get_id());
	render_thread.join();

	return 0;
}