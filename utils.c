#include <stdio.h>

#include <arpa/inet.h>

#include "utils.h"

void print_mac(unsigned char mac[ETH_ALEN]) {
	printf("%.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",
		 mac[0], mac[1], mac[2],
		 mac[3], mac[4], mac[5]
	 );
}

void print_ip(unsigned char ip[4]) {
	printf("%s\n", inet_ntoa(*((struct in_addr*) ip)));
}
