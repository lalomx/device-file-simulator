#!/bin/bash

# mknod /Users/lalovelazquez/dev/device1 c 89 1

#socat PTY,link=$HOME/virtual-serial0 PTY,link=$HOME/virtual-serial1

mkfifo ../fifo