#!/bin/bash
cd "${0%/*}"

cmake -G Xcode -DCMAKE_BUILD_TYPE=Release -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_SYSROOT=iphoneos -B ../build/ios -S ..
