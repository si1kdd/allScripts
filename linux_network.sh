#!/usr/bin/env bash
# @ Function
#       This script would contain lots of command that 
#       related to network interface on Linux.
#
# @ Usage:
#       ./linux_network.sh
#
# @ Author:
#       si1kdd
#
# @ Status:
#       Unfinished ...

dns_list() {
        echo "[*] Print Out DNS Server Query Status ... "

        # Suppose all Linux distro would use systemd as init system ...
        systemd-resolve --status
}

dns_list

# TODO: keep on update the functions.
