#include <netinet/ether.h>

#include "utils.h"

void pretty_mac(char *straddr, unsigned char mac[ETH_ALEN]) {
	struct ether_addr *eaddr = (struct ether_addr *)mac;
	ether_ntoa_r(eaddr, straddr);
}
