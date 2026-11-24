#include "governor.h"

governor System;

int main() {
	System.governor_init();

	System.app_init();
	
	while (!System.is_closing_command_true()) {
		System.app_update();
	}

	System.app_shutdown();
	System.governor_shutdown();

	return 0;
}