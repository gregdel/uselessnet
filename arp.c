#include <stdio.h>

#include <arpa/inet.h>

#include "arp.h"
#include "utils.h"

static void handle_arp_request(struct arp_pkt *arp) {
	printf("sender mac: ");
	print_mac(arp->ar_sha);
	printf("requested IP: ");
	print_ip(arp->ar_tip);
}

void handle_arp(void *buff) {
	struct arp_pkt *arp = (struct arp_pkt *)(buff + ETH_HLEN);

	if (ntohs(arp->ar_hrd) != ARPHRD_ETHER) {
		printf("arp: invalid hardware type: %x\n", ntohs(arp->ar_hrd));
		return;
	}

	if (ntohs(arp->ar_pro) != ARP_PROTO) {
		printf("arp: invalid protocol type: %x\n", ntohs(arp->ar_pro));
		return;
	}

	switch (ntohs(arp->ar_op)) {
	case ARPOP_REQUEST:
		handle_arp_request(arp);
		break;
	default:
		printf("arp: invalid op code: %x\n", ntohs(arp->ar_op));
		return;
	}
}

