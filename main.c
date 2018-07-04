#include <errno.h>
#include <signal.h>
#include <stdio.h>

#include <net/ethernet.h>
#include <netinet/ether.h>

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

void handle_frame(size_t size, const char *buff) {
	printf("Read %li bytes / buff %li\n", size, sizeof(buff));

	if (size < ETHER_MIN_LEN) {
		printf("Invalid packet size: %li\n", size);
		return;
	}

	// parse the headers
	struct ethhdr *hdr = (struct ethhdr *) buff;

	char p_mac[18];
	pretty_mac(p_mac, hdr->h_source);
	printf("src mac: %s\n", p_mac);
	pretty_mac(p_mac, hdr->h_dest);
	printf("dest mac: %s\n", p_mac);
	printf("--------------\n");
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

	// Handle signal
	struct sigaction sa = {
		.sa_handler = sighandler,
	};
	sigaction(SIGINT, &sa, NULL);

	printf("Reading...\n");
	char buff[PKT_BUFFER_SZ];
	size_t pkt_read;
	int i = 0;
	while (tap_continue) {
		i++;
		printf("Loop %d\n", i);
		pkt_read = tun_read(tap_fd, buff, sizeof(buff));
		if (pkt_read == -1) {
			continue;
		}

		handle_frame(pkt_read, buff);
	}

	return tap_close(tap_fd);
}
