#include <signal.h>
#include <stdio.h>

#include "tap.h"

volatile sig_atomic_t tap_continue = 1;

void sighandler(int signo) {
	if (signo == SIGINT) {
		printf("Received sigint\n");
		tap_continue = 0;
	}
}

int main(void) {
	printf("Hello there, let's create a tap device !\n");

	char tap_name[IFNAMSIZ] = {};
	int tap_fd;

	tap_fd = tap_alloc(tap_name);
	if (tap_fd < 0) {
		printf("Failed to create the tap device\n");
	}

	printf("Tap device %s created !\n", tap_name);

	// catch sigint
	signal(SIGINT, sighandler);

	printf("Reading...\n");
	char buff[2000];
	int r;
	while (tap_continue) {
		r = tun_read(tap_fd, buff, sizeof(buff));
		printf("Read %d bytes\n", r);
	}

	return tap_close(tap_fd);
}
