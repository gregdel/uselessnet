#pragma once

#include <linux/if_ether.h>

void print_mac(unsigned char mac[ETH_ALEN]);
void print_ip(unsigned char ip[4]);
