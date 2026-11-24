#include "ui.h"

void interface::init() {
	opengl_glfw_init();
	imgui_init();
}

void interface::opengl_glfw_init() {
	if (glfwInit() != GLFW_TRUE) return;

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	this->display_window = glfwCreateWindow(1280, 720, "typing_test_yes", nullptr, nullptr);
	if (!display_window) return;

	glfwMakeContextCurrent(display_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return;

	glfwSetTime(0);
}

void interface::imgui_init() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui_ImplGlfw_InitForOpenGL(this->display_window, true);
	ImGui_ImplOpenGL3_Init();
}

void interface::load_element_render_func(void(*func)(void)) {
	this->gui_element_func = func;
}

void interface::update() {
	opengl_glfw_update();
	imgui_update();
	render();
}

void interface::opengl_glfw_update() {
	glfwPollEvents();

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.3f, 0.3f, 0.3f, 0.5f);
}

void interface::imgui_update() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow(); // Show demo window! :)

	if (this->gui_element_func) gui_element_func();
}

void interface::render() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(display_window);
}

void interface::shutdown() {
	imgui_shutdown();
	opengl_glfw_shutdown();
}

void interface::opengl_glfw_shutdown() {
	glfwDestroyWindow(this->display_window);
	glfwTerminate();
}

void interface::imgui_shutdown() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

bool interface::glfw_window_should_close() {
	return glfwWindowShouldClose(this->display_window);
}

GLFWwindow* interface::get_display_window_ptr() {
	return this->display_window;
}