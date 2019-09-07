#!/bin/bash
set -e

if [ ! -d $ANDROID_BUILD_TOP/my_demo/interfaces ] ; then
  echo "Where is my_demo/interfaces?";
  exit 1;
fi

if [ ! -d $ANDROID_BUILD_TOP/system/libhidl/transport ] ; then
  echo "Where is system/libhidl/transport?";
  exit 1;
fi

PACKAGE=my_demo.hardware.led@1.0
LOC=$ANDROID_BUILD_TOP/my_demo/interfaces/led/1.0/default/
options="-rmy_demo.hardware:my_demo/interfaces \
        -randroid.hidl:system/libhidl/transport"

#make hidl-gen -j6

hidl-gen -o $LOC -Lc++-impl $options $PACKAGE

hidl-gen -o $LOC -Landroidbp-impl $options $PACKAGE

echo done!
