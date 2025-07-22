#!/bin/bash
cd "${0%/*}"/

cmake -B "../build/`uname`" -DCMAKE_BUILD_TYPE=Release -S ..