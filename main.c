#include <errno.h>
#include <signal.h>

#include "arp.h"
#include "frame.h"
#include "tap.h"
#include "utils.h"

#define PKT_BUFFER_SZ 1500

volatile sig_atomic_t tap_continue = 1;

void sighandler(int signo) {
	if (signo == SIGINT) {
		printf("Received sigint\n");
		tap_continue = 0;
	}
}

int main(void) {
	char tap_name[IFNAMSIZ] = {};

	int tap_fd = tap_alloc(tap_name);
	if (tap_fd < 0) {
		printf("Failed to create the tap device\n");
	}

	printf("Tap device %s created !\n", tap_name);

	// Handle signal
	struct sigaction sa = {
		.sa_handler = sighandler,
	};
	sigaction(SIGINT, &sa, NULL);

	printf("Reading...\n");
	unsigned char buff[PKT_BUFFER_SZ];
	while (tap_continue) {
		if (tun_read(tap_fd, buff, sizeof(buff)) == -1) {
			continue;
		}

		handle_frame(buff);
	}

	return tap_close(tap_fd);
}
