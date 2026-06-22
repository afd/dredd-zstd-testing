#!/bin/bash

clang -O2 -I lib/common -I programs -I tests \
    programs/datagen.c programs/lorem.c tests/loremOut.c tests/datagencli.c \
    -o datagen
