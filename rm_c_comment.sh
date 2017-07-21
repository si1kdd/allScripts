#!/usr/bin/env bash
# @ Function
#       Delete all C/C++ source code comment by using gcc.
#
# @ Usage:
#       ./Remove_C_comment.sh YOUR_SOURCE_FILE YOU_NEW_FILE
#
# @ Author:
#       si1kdd

gcc -fpreprocessed -P -dD -E $1 > $2
