#pragma once

#include <linux/if_ether.h>

void pretty_mac(char *straddr, unsigned char mac[ETH_ALEN]);
