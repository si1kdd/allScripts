#!/usr/bin/env bash
#
# @ Function
#       Just an stupid script to keep the notebook or rpi WIFI alive.
#
# @ Usage:
#       Use crontab to schedule this script and need to under root.
#       You can run it per 5 or 10 minute.
#	And suppose to use NetworkManager by default.
#
# @ Author:
#       si1kdd

HOST=www.google.com

if ! command -v nmcli > /dev/null 2>&1; then
        echo "[!] You don't have nmcli installed, please install it first ..."
        exit -1
fi

function wifi_check() {
	echo "[+] WIFI Checking ..."
	ping -c 1 -W 10 $HOST &> /dev/null
	if [ $? -eq 0 ]; then
		echo "[+] Network connected !"
		exit 1
	fi
}

wifi_check

# Be careful, if your have been setup WIFI auto connection.
# You can use nmcli or restart the network interface card.
# If you are using nmcli, you don't need the root privilege.

nmcli radio wifi off
# ip link set dev $NIC down
sleep 10
nmcli radio wifi on
# ip link set dev $NIC up
sleep 10

wifi_check

# The worse case is the reset the network interface.

# /etc/init.d/networking restart
# systemctl restart networking.service
systemctl restart NetworkManager.service
echo "[!] Restart NetworkManager"

wifi_check

echo "[X] Network is unreachable ..." 
