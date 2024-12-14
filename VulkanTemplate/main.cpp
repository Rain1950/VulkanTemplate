

#include <vulkan/vulkan.h>
#include <iostream>


class App {
public:
	void Run() {
		InitVulkan();
		MainLoop();
		Cleanup();
	}
private:
	void InitVulkan() {


	}


	void MainLoop() {
		
		
	}

	void Cleanup() {
		
	}

};



int main() {
	App app;

	try {
		app.Run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}