#!/bin/bash
cd "${0%/*}"

cmake -G Xcode -DCMAKE_BUILD_TYPE=Release -B ../build/mac  -S ..