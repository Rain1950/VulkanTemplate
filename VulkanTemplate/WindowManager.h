#pragma once
#include <GLFW/glfw3.h>

class WindowManager {
	public:
		~WindowManager();
		GLFWwindow* window;
		void InitWindow();
	private:
		const uint32_t WIDTH = 800;
		const uint32_t HEIGHT = 600;
};