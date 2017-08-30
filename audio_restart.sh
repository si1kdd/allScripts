#!/usr/bin/env bash
# @ Function
#       Restart the audio features,
#       Default restart PulseAudio daemon to fix this problem.
#       Sometimes the audio would be buggy on Linux.
#
# @ Usage:
#       ./audio_restart.sh
#
# @ Author:
#       si1kdd

if command -v pulseaudio > /dev/null 2>&1; then
        echo "[!] pulseaudio not found, please install the audio tools ..."
        exit -1
fi

pulseaudio -k ; pulseaudio -D
