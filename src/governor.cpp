#include "governor.h"

bool governor::is_closing_command_true() {
	return interface_object.glfw_window_should_close();
}

void governor::governor_init() {

}

void governor::app_init() {
	interface_object.init();
	init_core_api_components();
}

void governor::init_core_api_components() {
	glfw_input_wrapper::init(interface_object.get_display_window_ptr());

	core_api_objects.input_tracker_object.init();
	core_api_objects.ref_str_object.init();
	core_api_objects.input_checker_object.init(&core_api_objects.input_tracker_object, &core_api_objects.ref_str_object);
	core_api_objects.perf_analyzer_object.init();
}

void governor::app_update() {
	interface_object.update();
	update_core_api_components();
}

void governor::update_core_api_components() {
	core_api_objects.input_tracker_object.record_user_input();
	core_api_objects.input_checker_object.update();
}

void governor::app_shutdown() {
	interface_object.shutdown();
	shutdown_core_api_components();
}

void governor::shutdown_core_api_components() {

}

void governor::governor_shutdown() {
	
}