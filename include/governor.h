#pragma once

#include "includes.h"
#include "typingapp.h"
#include "ui.h"

struct core_api_components {
	user_key_tracker input_tracker_object;
	reference_string ref_str_object;
	input_validator input_checker_object;
	performance_analyzer perf_analyzer_object;
};

class governor {
private:
	interface interface_object;
	
	core_api_components core_api_objects;

	void init_core_api_components();
	void update_core_api_components();
	void shutdown_core_api_components();

	void(*ui_element_func)(void) = nullptr;
public:
	bool is_closing_command_true();

	void governor_init();

	void app_init();
	void app_update();
	void app_shutdown();

	void governor_shutdown();
};