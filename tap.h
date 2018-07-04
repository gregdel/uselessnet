#pragma once

#include <sys/types.h>

#define TAP_PATH "/dev/net/tun"

int tap_alloc(char *dev);
int tap_close(int fd);
int tun_read(int fd, unsigned char *buf, size_t count);
