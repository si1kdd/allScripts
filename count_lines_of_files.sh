#!/usr/bin/env bash
# @ Function
#       list number of lines of every "files" in this directory.
#
# @ Usage:
#       ./count_all_lines_files.sh DIRNAME
#
# @ Author:
#       si1kdd

find $1 -type f -print0 | wc -l --files0-from=-
