#!/usr/bin/env bash
# @ Function
#       Show Disk and CPU temperature by using smartctl and sensors.
#
# @ Usage:
#       ./backup_fs_perms.sh
#
# @ Author:
#       si1kdd

if [[ -z `which smartctl` ]] || [[ -z `which sensors` ]]; then
        exit -1
        echo
        echo "[*] Please install 'smartctl' and 'sensors'"
fi

echo "[*] CPU:"
sensors | grep "Core"
echo "[*] Disk:"
smartctl -d ata -A /dev/sda | grep -i Temperature | awk '{print $10}'
