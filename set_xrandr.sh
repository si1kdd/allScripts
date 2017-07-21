#!/usr/bin/env bash
# @ Function
#       Setting up the monitor display resolution, default resolution is 1920x1080
#       Some really old monitor/screen can't auto configure on Linux.
#       Configuration using xrandr, just a simple scripts for lazy guy like me.
#
# @ Usage:
#       ./Xrandr YOUR_DEVICE_NAME
#
# @ Reference:
#       https://wiki.archlinux.org/index.php/xrandr
#
# @ Author:
#       si1kdd

if [[ -z "$1" ]]; then
        echo "[x] Check device first ....."
        xrandr -q
fi

if [[ ! -z "$1" ]]; then
        xrandr --newmode "1920x1080_60.00" 173.00 1920 2048 2248 2576 1080 1083 1088 1120 -hsync +vsync
        # 60Hz
        xrandr --addmode $1 "1920x1080_60.00"
fi
