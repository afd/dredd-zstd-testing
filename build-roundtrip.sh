#!/bin/bash

clang-17 dredd-zstd-testing/roundtrip.c -I lib build-zstd/lib/libzstd.a -o roundtrip
