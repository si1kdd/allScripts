#!/usr/bin/env bash
# @ Function
#       Just an stupid script to keep the notebook or rpi WIFI alive.
#
# @ Usage:
#       use crontab under root.
#       You can run it per 5 or 10 minute.
#
# @ Author:
#       si1kdd
#
# @ Status:
#       Unfinished ...

HOST=www.google.com

if ! command -v nmcli > /dev/null 2>&1; then
        echo "[!] You don't have nmcli installed, please install it first ..."
        exit -1
fi

echo "[+] WIFI Checking ..."

ping -c 1 -W 10 $HOST &> /dev/null
if [ $? -eq 0 ]; then
        echo "[+] Network connected !"
        exit 1
else
        # Be careful, if your have been setup WIFI auto reconnect.
        # You can use nmcli or restart the network interface card.
        # If you use nmcli, you don't need the root privilege.

        nmcli radio wifi off
        # ip link set dev $NIC down
        sleep 10
        nmcli radio wifi on
        # ip link set dev $NIC up
        sleep 10

        ping -c 1 -W 10 $HOST &> /dev/null

        if [ $? -eq 0 ]; then
                echo "[+] WIFI Reconnected !"
                exit
        else
                /etc/init.d/networking restart
                # systemctl restart networking.service
                ping -c 1 -W 10 $HOST &> /dev/null
        fi
fi

