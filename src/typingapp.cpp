#include "typingapp.h"

int glfw_input_wrapper::latest_keystroke = GLFW_KEY_UNKNOWN;
float glfw_input_wrapper::time_of_last_stroke = 0;
std::map<char, int> glfw_input_wrapper::key_char_association = 
{
		{ ' ',  GLFW_KEY_SPACE },
		{ '\'', GLFW_KEY_APOSTROPHE },
		{ ',',  GLFW_KEY_COMMA },
		{ '-',  GLFW_KEY_MINUS },
		{ '.',  GLFW_KEY_PERIOD },
		{ '/',  GLFW_KEY_SLASH },
		{ '0',  GLFW_KEY_0 },
		{ '1',  GLFW_KEY_1 },
		{ '2',  GLFW_KEY_2 },
		{ '3',  GLFW_KEY_3 },
		{ '4',  GLFW_KEY_4 },
		{ '5',  GLFW_KEY_5 },
		{ '6',  GLFW_KEY_6 },
		{ '7',  GLFW_KEY_7 },
		{ '8',  GLFW_KEY_8 },
		{ '9',  GLFW_KEY_9 },
		{ ';',  GLFW_KEY_SEMICOLON },
		{ '=',  GLFW_KEY_EQUAL },
		{ 'a',  GLFW_KEY_A },
		{ 'b',  GLFW_KEY_B },
		{ 'c',  GLFW_KEY_C },
		{ 'd',  GLFW_KEY_D },
		{ 'e',  GLFW_KEY_E },
		{ 'f',  GLFW_KEY_F },
		{ 'g',  GLFW_KEY_G },
		{ 'h',  GLFW_KEY_H },
		{ 'i',  GLFW_KEY_I },
		{ 'j',  GLFW_KEY_J },
		{ 'k',  GLFW_KEY_K },
		{ 'l',  GLFW_KEY_L },
		{ 'm',  GLFW_KEY_M },
		{ 'n',  GLFW_KEY_N },
		{ 'o',  GLFW_KEY_O },
		{ 'p',  GLFW_KEY_P },
		{ 'q',  GLFW_KEY_Q },
		{ 'r',  GLFW_KEY_R },
		{ 's',  GLFW_KEY_S },
		{ 't',  GLFW_KEY_T },
		{ 'u',  GLFW_KEY_U },
		{ 'v',  GLFW_KEY_V },
		{ 'w',  GLFW_KEY_W },
		{ 'x',  GLFW_KEY_X },
		{ 'y',  GLFW_KEY_Y },
		{ 'z',  GLFW_KEY_Z },
		{ '[',  GLFW_KEY_LEFT_BRACKET },
		{ '\\', GLFW_KEY_BACKSLASH },
		{ ']',  GLFW_KEY_RIGHT_BRACKET },
		{ '`',  GLFW_KEY_GRAVE_ACCENT },
};

void glfw_input_wrapper::glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	glfw_input_wrapper::latest_keystroke = key;
	glfw_input_wrapper::time_of_last_stroke = glfwGetTime();
}

void glfw_input_wrapper::init(GLFWwindow* window_for_callback) {
	glfwSetKeyCallback(window_for_callback, glfw_key_callback);
	time_of_last_stroke = glfwGetTime();
	latest_keystroke = GLFW_KEY_UNKNOWN;
}

int glfw_input_wrapper::get_latest_recorded_keystroke() {
	return glfw_input_wrapper::latest_keystroke;
}

float glfw_input_wrapper::get_time_of_last_recorded_key() {
	return glfw_input_wrapper::time_of_last_stroke;
}

int glfw_input_wrapper::get_keycode_from_char(char c) {
	return key_char_association.at(c);
}

void user_key_tracker::init() {
	recorded_keystrokes = {};
	latest_input_accessed = false;
	time_of_last_stroke = glfwGetTime();
}

void user_key_tracker::record_user_input() {
	if (!is_new_key_recorded()) return;

	int recorded_keystroke = glfw_input_wrapper::get_latest_recorded_keystroke();
	if (!user_key_tracker::is_stroke_valid(recorded_keystroke)) return;

	acknowledge_keystroke(recorded_keystroke);
	input_to_current_word(recorded_keystroke);

	clear_key_access_record();
}

void user_key_tracker::acknowledge_keystroke(int recorded_keystroke) {
	this->recorded_keystrokes.push_back(recorded_keystroke);
}

void user_key_tracker::input_to_current_word(int recorded_keystroke) {
	if (recorded_keystroke == GLFW_KEY_BACKSPACE) {
		if (current_word.empty()) {
			fall_back_to_prev_word();
		}
		else {
			fall_back_to_prev_char();
		}
		
		return;
	}

	if (recorded_keystroke == GLFW_KEY_SPACE) {
		record_current_word();
		
		return;
	}

	this->current_word.push_back(recorded_keystroke);
}

void user_key_tracker::fall_back_to_prev_word() {
	current_word = recorded_words[recorded_words.size() - 1];
	recorded_words.pop_back();
}

void user_key_tracker::fall_back_to_prev_char() {
	current_word.pop_back();
}

void user_key_tracker::record_current_word() {
	recorded_words.push_back(current_word);
	current_word.clear();
}

bool user_key_tracker::is_new_key_recorded() {
	if (recorded_keystrokes.empty()) return true;

	if (glfw_input_wrapper::get_time_of_last_recorded_key() - time_of_last_stroke <= 0.15f 
	&& glfw_input_wrapper::get_latest_recorded_keystroke() == recorded_keystrokes[recorded_keystrokes.size() - 1]) return false;
	return true;
}

void user_key_tracker::clear_key_access_record() {
	latest_input_accessed = false;
	time_of_last_stroke = glfw_input_wrapper::get_time_of_last_recorded_key();
}

bool user_key_tracker::is_stroke_valid(int keystroke) {
	if (keystroke >= GLFW_KEY_WORLD_1 && keystroke <= GLFW_KEY_TAB) return false;
	if (keystroke >= GLFW_KEY_INSERT) return false;

	return true;
}

bool user_key_tracker::get_latest_new_input(input_data_package* output) {
	if (latest_input_accessed) return false;

	*output = gen_input_data_package();

	latest_input_accessed = true;

	return true;
}

input_data_package user_key_tracker::gen_input_data_package() {
	input_data_package result;

	result.num_recorded_words = recorded_words.size();
	result.num_char_in_curr_word = current_word.size();
	result.latest_input_in_word = current_word.empty() ? -1 : current_word[current_word.size() - 1];

	return result;
}


void reference_string::init() {
	this->ref_string = "program under now because when we come man do place go a it plan"; //default string for system test for now. generator added later
	std::cout << ref_string << std::endl;

	parse_word_starting_indices(0);
}

void reference_string::parse_word_starting_indices(int start_idx) {
	word_starting_indices.push_back(start_idx);

	int next_word_index = get_next_word_index(start_idx);

	if (next_word_index == -1) return;

	parse_word_starting_indices(next_word_index);
}

int reference_string::get_total_num_words() {
	return word_starting_indices.size();
}

char reference_string::get_char_at_index(int current_tracking_index) {
	if (current_tracking_index >= this->ref_string.length()) return -1;

	return this->ref_string.at(current_tracking_index);
}

int reference_string::get_next_word_index(int current_tracking_index) {
	int next_space_index = string_util::read_until_char(this->ref_string, ' ', current_tracking_index);

	if (next_space_index == -1) return -1;

	return ++next_space_index;
}

int reference_string::get_total_num_chars() {
	return this->ref_string.length();
}

int reference_string::get_index_of_nth_word(int nth) {
	if (nth >= word_starting_indices.size()) return -1;

	return word_starting_indices.at(nth);
}

int reference_string::get_num_char_of_nth_word(int nth) {
	if (nth == word_starting_indices.size() - 1) {
		return ref_string.length() - get_index_of_nth_word(nth);
	}

	return get_index_of_nth_word(nth + 1) - get_index_of_nth_word(nth) - 1;
}

bool input_validator::is_input_at_char_correct(int word_index, int char_index) {
	if (word_index > input_validity.size()) return false;

	std::vector<bool> requested_word = word_index == input_validity.size() ? curr_word_validity : input_validity.at(word_index);

	if (char_index >= requested_word.size()) return false;
	return requested_word.at(char_index);
}

void input_validator::init(user_key_tracker* user_input_tracker, reference_string* ref_str) {
	this->user_input_tracker = user_input_tracker;
	this->ref_str = ref_str;

	this->is_new_input_received = false;
	this->detected_action = NONE;
	this->input_validity = {};
	this->offset_of_current_word = 0;
	this->current_char_in_word = 0;

}

void input_validator::update() {
	if (is_typing_done()) return;

	reset_input_data_buf();

	parse_latest_new_input();

	if (no_new_input()) return;

	parse_action_type_from_input();
	update_ref_input_data();
	interpret_input_data();

	handle_input_data();
}

bool input_validator::is_typing_done() {
	return false;
}

void input_validator::reset_input_data_buf() {
	detected_action = NONE;
	is_new_input_received = false;
}

void input_validator::parse_latest_new_input() {
	if (!this->user_input_tracker->get_latest_new_input(&curr_pkg)) {
		return;
	}

	is_new_input_received = true;
}

bool input_validator::no_new_input() {
	return !is_new_input_received;
}

void input_validator::parse_action_type_from_input() {
	if (!prev_pkg_valid) return;

	detect_new_char();
	detect_word_jump();
	detect_key_del();
}

void input_validator::detect_new_char() {
	if (curr_pkg.num_recorded_words != prev_pkg.num_recorded_words) return;

	if (curr_pkg.num_char_in_curr_word <= prev_pkg.num_char_in_curr_word) return;

	detected_action = NEW_KEY;
}

void input_validator::detect_word_jump() {
	if (curr_pkg.num_recorded_words == prev_pkg.num_recorded_words) return;

	detected_action = WORD_JUMP;
}

void input_validator::detect_key_del() {
	if (curr_pkg.num_recorded_words != prev_pkg.num_recorded_words) return;

	if (curr_pkg.num_char_in_curr_word >= prev_pkg.num_char_in_curr_word) return;

	detected_action = KEY_DEL;
}

void input_validator::update_ref_input_data() {
	prev_pkg_valid = true;
	prev_pkg = curr_pkg;
}

void input_validator::interpret_input_data() {
	this->latest_new_char = curr_pkg.latest_input_in_word;
	this->current_char_in_word = curr_pkg.num_char_in_curr_word - 1;
	this->offset_of_current_word = ref_str->get_index_of_nth_word(curr_pkg.num_recorded_words);
	this->curr_num_recorded_words = curr_pkg.num_recorded_words;

	std::cout << offset_of_current_word << ' ' << current_char_in_word << std::endl;
}

void input_validator::handle_input_data() {
	switch (detected_action) {
	case NEW_KEY: handle_new_key(); return;
	case KEY_DEL: handle_key_del(); return;
	case WORD_JUMP: handle_word_jump(); return;
	default: return;
	}
}

void input_validator::handle_new_key() {
	if (is_new_key_correct()) {
		curr_word_validity.push_back(true);

		std::cout << ' ' << 1 << std::endl;
	}
	else {
		curr_word_validity.push_back(false);

		std::cout << ' ' << 0 << std::endl;
	}
}

bool input_validator::is_new_key_correct() {
	if (!input_within_curr_word()) {
		return false;
	}

	int ref_char_index = offset_of_current_word + current_char_in_word;
	int expected_char_keycode = glfw_input_wrapper::get_keycode_from_char(ref_str->get_char_at_index(ref_char_index));

	if (latest_new_char == expected_char_keycode) {
		return true;
	}

	return false;
}
bool input_validator::input_within_curr_word() {
	if (current_char_in_word < ref_str->get_num_char_of_nth_word(curr_num_recorded_words)) {
		return true;
	}

	return false;
}

void input_validator::handle_key_del() {
	curr_word_validity.pop_back();
}

void input_validator::handle_word_jump() {
	if (curr_num_recorded_words > input_validity.size()) {
		input_validity.push_back(curr_word_validity);

		curr_word_validity.clear();
	}
	else if (curr_num_recorded_words < input_validity.size()) {
		curr_word_validity = input_validity[input_validity.size() - 1];

		input_validity.pop_back();
	}
}


void performance_analyzer::init() {
	for (int i = 0; i < NUM_PERF_DATA; i++) {
		performance_results[i] = 0;
	}

	for (int i = 0; i < NUM_PERF_PARAM; i++) {
		performance_variables[i] = 0;
	}

	performance_variables[START_TIME] = time(nullptr);
	performance_variables[FINISH_TIME] = time(nullptr);
}

void performance_analyzer::record_mistake() {
	performance_variables[NUM_MISTAKES]++;
}

void performance_analyzer::record_letter() {
	performance_variables[NUM_LETTERS]++;
}

void performance_analyzer::record_word() {
	performance_variables[NUM_WORDS]++;
}

void performance_analyzer::parse_summary() {
	parse_wpm();
	parse_accuracy();
}

void performance_analyzer::parse_wpm() {
	float num_minutes_in_session = get_session_duration_in_min();

	performance_results[WPM] = performance_variables[NUM_WORDS] / num_minutes_in_session;
}

void performance_analyzer::parse_accuracy() {
	float num_mistakes_float = static_cast<float>(performance_variables[NUM_MISTAKES]);
	float num_letters_float = static_cast<float>(performance_variables[NUM_LETTERS]);

	performance_results[ACCURACY] = 1 - (num_mistakes_float / num_letters_float);
}

float performance_analyzer::get_wpm() {
	return performance_results[WPM];
}

float performance_analyzer::get_accuracy() {
	return performance_results[ACCURACY];
}

float performance_analyzer::get_session_duration_in_min() {
	int num_seconds_in_session = performance_variables[FINISH_TIME] - performance_variables[START_TIME];
	return static_cast<float>(num_seconds_in_session) / 60.0f;
}