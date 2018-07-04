#pragma once

#include <linux/if_ether.h>

#define MAC_SZ ((ETH_ALEN * 2) + (ETH_ALEN - 1))

void pretty_mac(char *straddr, unsigned char mac[ETH_ALEN]);
