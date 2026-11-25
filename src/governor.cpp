#include "governor.h"

governor* governor::def_gov_obj_for_gui_func;

bool governor::is_closing_command_true() {
	return interface_object.glfw_window_should_close();
}

void governor::governor_init() {
	def_gov_obj_for_gui_func = this;
}

void governor::app_init() {
	interface_object.init();
	interface_object.load_element_render_func(this->gui_element_func);

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

void governor::gui_element_func() {
	int total_num_words = def_gov_obj_for_gui_func->core_api_objects.ref_str_object.get_total_num_words();

	for (int i = 0; i < total_num_words; i++) {
		def_gov_obj_for_gui_func->gui_render_word(i);

		ImGui::SameLine(0.0f, 15.0f);
	}
}

void governor::gui_render_word(int word_index) {
	int num_chars_in_word = core_api_objects.ref_str_object.get_num_char_of_nth_word(word_index);
	int word_start_idx = core_api_objects.ref_str_object.get_index_of_nth_word(word_index);

	for (int i = 0; i < num_chars_in_word; i++) {
		int correctness = core_api_objects.input_checker_object.is_input_at_char_correct(word_index, i);
		pick_char_color(correctness);

		std::string text_content = get_text_content(word_start_idx, i);
		ImGui::Text(text_content.c_str());

		ImGui::PopStyleColor();

		ImGui::SameLine(0.0f, 1.0f);
	}
}

void governor::pick_char_color(int correctness) {
	if (correctness) {
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
	}
}

std::string governor::get_text_content(int word_offset, int char_idx) {
	std::string result = "";

	result += core_api_objects.ref_str_object.get_char_at_index(word_offset + char_idx);

	return result;
}