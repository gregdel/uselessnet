#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <linux/if.h>
#include <linux/if_tun.h>

#define TAP_PATH "/dev/net/tun"

int tap_alloc(char *dev);
int tap_close(int fd);
int tun_read(int fd, void *buf, size_t count);
