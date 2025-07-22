#!/bin/bash
cd "${0%/*}"

emcmake cmake -S .. -DCMAKE_BUILD_TYPE=Release -B ../build/web