#!/bin/bash
cd "${0%/*}"

cmake -G Xcode -DCMAKE_BUILD_TYPE=Release -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_SYSROOT=iphoneos -DCMAKE_OSX_DEPLOYMENT_TARGET=16.1 -B ../build/ios -S ..
