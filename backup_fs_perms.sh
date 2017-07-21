#!/usr/bin/env bash
# @ Function
#       Copy the root / POSIX ACL setting, restore Unix file permissions.
#
# @ Usage:
#       ./backup_fs_perms.sh
#
# @ Author:
#       si1kdd

getfacl -R / > /tmp/perms.txt
cd / && setfacl --restore=/tmp/perms.txt

# TODO, check super user; restore FileSystem extended attributes.
# ......
