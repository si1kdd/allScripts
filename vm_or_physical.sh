#!/usr/bin/env bash
# @ Function
#       A tiny shell script to confirm the environment, inside a Virtual Machine or not.
#
# @ Usage:
#       ./vm_or_phyiscal.sh
#
# @ Author:
#       si1kdd

check_super_user() {
        if [ "$EUID" -ne 0 ]; then
                echo "[!] Please run as root or use 'sudo' command ..."
                echo "          This scripts need privilege. (And only tested on CentOS and Arch Linux)"
                exit -1
        fi
}

check_super_user

PRODUCT_NAME=$(dmidecode -t system | egrep -i "manufacturer|product")

echo $PRODUCT_NAME
