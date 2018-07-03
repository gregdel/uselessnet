#include "tap.h"

int tap_alloc(char *dev)
{
	struct ifreq ifr;
	int fd, err;

	fd = open(TAP_PATH, O_RDWR);
	if (fd < 0) {
		perror("tun: failed to open tun fd");
		return fd;
	}

	memset(&ifr, 0, sizeof(ifr));

	ifr.ifr_flags = IFF_TAP;
	if (*dev)
		strncpy(ifr.ifr_name, dev, IFNAMSIZ);

	err = ioctl(fd, TUNSETIFF, (void *) &ifr);
	if (err < 0) {
		perror("failed to create tun");
		close(fd);
		return err;
	}
	strcpy(dev, ifr.ifr_name);

	return fd;
}

int tap_close(int fd) {
	int err = close(fd);
	if (err < 0) {
		perror("tun: failed to close fd");
	}
	return err;
}

int tun_read(int fd, void *buf, size_t count) {
	int r = read(fd, buf, count);
	if (r < 0) {
		perror("tun: failed to read");
	}
	return r;
}