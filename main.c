#include <errno.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>
#include <linux/if.h>
#include <linux/if_arp.h>
#include <net/ethernet.h>

#include "arp.h"
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

void handle_frame(size_t size, unsigned char *buff) {
	/* printf("Read %li bytes\n", size); */

	/* if (size < ETHER_MIN_LEN) { */
	if (size < 0) {
		printf("Invalid packet size: %li\n", size);
		return;
	}

	// parse the headers
	struct ethhdr *hdr = (struct ethhdr *) buff;

	switch(ntohs(hdr->h_proto)) {
		case ETHERTYPE_IPV6:
		      /* printf("IPv6 frame, skipping\n"); */
		      return;
		case ETHERTYPE_ARP:
		      printf("ARP frame !\n");
		      handle_arp(buff);
		      break;
		case ETHERTYPE_IP:
		      /* printf("IP frame !"); */
		      break;
		default:
		      printf("src mac: ");
		      print_mac(hdr->h_source);
		      printf("dest mac: ");
		      print_mac(hdr->h_dest);
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

	// Handle signal
	struct sigaction sa = {
		.sa_handler = sighandler,
	};
	sigaction(SIGINT, &sa, NULL);

	printf("Reading...\n");
	unsigned char buff[PKT_BUFFER_SZ];
	size_t pkt_read;
	while (tap_continue) {
		pkt_read = tun_read(tap_fd, buff, sizeof(buff));
		if (pkt_read == -1) {
			continue;
		}

		handle_frame(pkt_read, buff);
	}

	return tap_close(tap_fd);
}
