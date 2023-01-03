#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<fcntl.h>
#include<poll.h>
#include<time.h>

#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define MAX_BUF 6


