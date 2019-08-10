#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define LOG_TAG "my_demo.c.hello"
#include <log/log.h>


int main(int argc, char *argv[])
{
	printf("hello android world!\n");
	ALOGD("main enter\n");

	int fd = open("/dev/input/event0", O_RDWR);
	if (fd < 0) {
		perror("OPEN");
		exit(1);
	}

	ALOGD("123\n");
	ALOGI("argc = %d\n", argc);
	ALOGW("warning\n");
	ALOGE("error\n");

	return 0;
}
