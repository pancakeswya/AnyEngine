#!/bin/bash

# note: requires CMake 3.28 or newer

cd "${0%/*}"

cmake -G Xcode -DCMAKE_BUILD_TYPE=Release -DCMAKE_SYSTEM_NAME=visionOS -S .. -B ../build/visionOS
