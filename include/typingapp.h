#pragma once

#include "includes.h"
#include "util.h"

class glfw_input_wrapper {
private:
	static int latest_keystroke;
	static float time_of_last_stroke;

	static std::map<char, int> key_char_association;

public:
	static void init(GLFWwindow* window_for_callback);

	static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static int get_latest_recorded_keystroke();
	static float get_time_of_last_recorded_key();

	static int get_keycode_from_char(char c);
};

struct input_data_package {
	int num_recorded_words;
	int num_char_in_curr_word;
	int latest_input_in_word;
};

class user_key_tracker {
private:
	std::vector<int> recorded_keystrokes;

	std::vector<std::vector<int>> recorded_words;

	std::vector<int> current_word;
	
	bool latest_input_accessed = false;

	float time_of_last_stroke = 0;

	bool is_new_key_recorded();

	input_data_package gen_input_data_package();

public:
	void init();

	void record_user_input();
	void acknowledge_keystroke(int recorded_keystroke);
	void input_to_current_word(int recorded_keystroke);
	void record_current_word();

	void clear_key_access_record();

	bool get_latest_new_input(input_data_package* output);

private:
	static bool is_stroke_valid(int keystroke);

	void fall_back_to_prev_word();
	void fall_back_to_prev_char();
};

class reference_string {
private:
	std::string ref_string;

	std::vector<int> word_starting_indices;

	void parse_word_starting_indices(int start_idx);
public:
	void init();

	int get_total_num_words();
	char get_char_at_index(int current_tracking_index);
	int get_next_word_index(int current_tracking_index);
	int get_total_num_chars();

	int get_index_of_nth_word(int nth);
	int get_num_char_of_nth_word(int nth);
};

class input_validator {
private:
	enum action_type { NEW_KEY, KEY_DEL, WORD_JUMP, NONE };

	user_key_tracker* user_input_tracker = nullptr;
	reference_string* ref_str = nullptr;

	std::vector<std::vector<bool>> input_validity;
	std::vector<bool> curr_word_validity;

	bool is_new_input_received = false;

	int curr_num_recorded_words = 0;
	int offset_of_current_word = 0;
	int current_char_in_word = 0;
	int latest_new_char = -1;

	input_data_package prev_pkg = {};
	input_data_package curr_pkg = {};

	bool prev_pkg_valid = false;

	action_type detected_action = NONE;

public:
	void init(user_key_tracker* user_input_tracker, reference_string* ref_str);

	void update();

	bool is_input_at_char_correct(int word_index, int char_index);
private:
	bool is_typing_done();

	void reset_input_data_buf();

	void parse_latest_new_input();

	bool no_new_input();

	void parse_action_type_from_input();

	void detect_new_char();
	void detect_word_jump();
	void detect_key_del();

	void update_ref_input_data();
	void interpret_input_data();

	void handle_input_data();
	void handle_new_key();

	bool is_new_key_correct();
	bool input_within_curr_word();

	void handle_key_del();
	void handle_word_jump();
};

class performance_analyzer {
public:
	void init();

	void record_mistake();
	void record_letter();
	void record_word();

	void parse_summary();

	void parse_wpm();
	void parse_accuracy();

	float get_wpm();
	float get_accuracy();

private:
	enum performance_variable {
		START_TIME = 0,
		FINISH_TIME,
		NUM_WORDS,
		NUM_LETTERS,
		NUM_MISTAKES,
		NUM_PERF_PARAM
	};

	enum performance_output_data {
		WPM,
		ACCURACY,
		NUM_PERF_DATA
	};

	int performance_variables[NUM_PERF_PARAM];
	float performance_results[NUM_PERF_DATA];

	float get_session_duration_in_min();
};