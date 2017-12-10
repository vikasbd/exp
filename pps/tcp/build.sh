#!/bin/sh
set +e

clang -O3 -Wall -Wextra -Wno-unused-parameter \
    -ggdb -g -pthread \
    -o tcpreceiver1 tcpreceiver1.c \
    net.c

clang -O3 -Wall -Wextra -Wno-unused-parameter \
    -ggdb -g -pthread \
    -o tcpsender tcpsender.c \
    net.c
