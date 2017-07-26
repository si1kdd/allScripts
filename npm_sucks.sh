#!/usr/bin/env bash
# @ Function
#       When npm dependency sucks, use this lazy scripts.
#
# @ Usage:
#       ./npm_sucks.sh
#
# @ Author:
#       si1kdd

rm -rf node_modules
npm cache clean
npm install
