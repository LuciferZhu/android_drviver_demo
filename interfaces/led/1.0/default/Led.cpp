// FIXME: your file license if you have one

#define LOG_TAG "my_demo.hardware.led@1.0-impl"
#include <log/log.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "Led.h"

namespace my_demo {
namespace hardware {
namespace led {
namespace V1_0 {
namespace implementation {

static const char THE_DEVICE[] = "/sys/devices/platform/leds/leds/user_led3/brightness";

// Methods from ::my_demo::hardware::led::V1_0::ILed follow.
Led::Led() {
    ALOGD("%s() ++++++++", __func__);
}

Led::~Led() {
    ALOGD("%s() ++++++++", __func__);
}

Return<int32_t> Led::control(uint32_t enable) {
    char ctrl_str[4];
    int ret = 0;
    ALOGD("%s() ++++++++ enable=%d", __func__, enable);

    int fd = open(THE_DEVICE, O_RDWR);
    if (fd < 0) {
        ALOGE("open: %s", strerror(errno));
        ret = -1;
        goto err_open;
    }

    memset(ctrl_str, '\0', sizeof(ctrl_str));
    if (enable)
        strcpy(ctrl_str, "255");
    else
        strcpy(ctrl_str, "0");

    ALOGD("ctrl_str='%s'", ctrl_str);
    ret = write(fd, ctrl_str, strlen(ctrl_str)+1);
    if (ret < 0) {
        ALOGE("write: %s", strerror(errno));
        ret = -1;
        goto err_write;
    }

    close(fd);
    return 0;

err_write:
    close(fd);
err_open:
    return int32_t {ret};
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

}  // namespace implementation
}  // namespace V1_0
}  // namespace led
}  // namespace hardware
}  // namespace my_demo
