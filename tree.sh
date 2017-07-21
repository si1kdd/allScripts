#!/usr/bin/env bash
# @ Function
#       Implement the command "tree" features.
#
# @ Usage:
#       ./Tree.sh DIRECTORY_PATH
#
# @ Author:
#       si1kdd

ls -R $1 |
grep ":$" |
sed -e 's/:$//' -e 's/[^-][^\/]*\//--/g' -e 's/^/   /' -e 's/-/|/'
