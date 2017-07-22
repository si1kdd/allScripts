#!/usr/bin/env bash
# @ Function
#       When npm dependency sucks, use this lazy scripts.
#
# @ Usage:
#       ./npm_sucks.sh $DIR
#
# @ Author:
#       si1kdd

rm -rf node_modules
npm cache clean
npm install
