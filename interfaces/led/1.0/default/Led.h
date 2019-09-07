// FIXME: your file license if you have one

#pragma once

#include <my_demo/hardware/led/1.0/ILed.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace my_demo {
namespace hardware {
namespace led {
namespace V1_0 {
namespace implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct Led : public ILed {
    // Methods from ::my_demo::hardware::led::V1_0::ILed follow.
    Led();
    ~Led();
    Return<int32_t> control(uint32_t enable) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

// FIXME: most likely delete, this is only for passthrough implementations
// extern "C" ILed* HIDL_FETCH_ILed(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace led
}  // namespace hardware
}  // namespace my_demo
