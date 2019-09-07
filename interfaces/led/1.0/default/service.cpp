
#define LOG_TAG "my_demo.hardware.led@1.0-service"
#include <log/log.h>
#include <hidl/HidlSupport.h>
#include <hidl/HidlTransportSupport.h>
#include <my_demo/hardware/led/1.0/ILed.h>
#include "Led.h"

using my_demo::hardware::led::V1_0::implementation::Led;
using android::sp;
using android::status_t;

int main(int /* argc */, char* /* argv */ []) {
    // This function must be called before you join to ensure the proper
    // number of threads are created. The threadpool will never exceed
    // size one because of this call.
    ::android::hardware::configureRpcThreadpool(1 /*threads*/, true /*willJoin*/);

    ALOGD("prepare for my_demo.hardware.led@1.0-service");
    sp led = new Led();
    const status_t status = led->registerAsService();
    if (status != ::android::OK) {
        ALOGE("Can't register service of Led");
        return 1; // or handle error
    }

    // Adds this thread to the threadpool, resulting in one total
    // thread in the threadpool. We could also do other things, but
    // would have to specify 'false' to willJoin in configureRpcThreadpool.
    ::android::hardware::joinRpcThreadpool();
    return 1; // joinRpcThreadpool should never return
}

