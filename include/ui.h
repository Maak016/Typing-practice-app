#pragma once

#include "includes.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


class interface {
public:
	class defaults {
	public:
		static int window_height, window_width;
		static ImVec4 color_wrong, color_correct, color_neutral;

		static float spacing_between_chars, spacing_between_words;
	};

	void init();

	void load_element_render_func(void(*func)(void));

	void update();

	bool glfw_window_should_close();

	void shutdown();

	GLFWwindow* get_display_window_ptr();
private:
	GLFWwindow* display_window = nullptr;
	void (*gui_element_func)(void) = nullptr;

	void opengl_glfw_init();
	void imgui_init();

	void opengl_glfw_update();

	void imgui_update();

	void menu_bar();

	void main_content_space_setup();
	void main_content_space_end();

	void render();

	void opengl_glfw_shutdown();
	void imgui_shutdown();
};