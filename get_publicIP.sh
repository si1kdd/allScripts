#!/usr/bin/env bash
# @ Function
#       Check the gateway/outgoing machine IP, in 3 different way.
#
# @ Usage:
#       ./get_public_IP.sh
#
# @ Author:
#       si1kdd


echo "[!] By ipinfo"
wget -qO - http://ipinfo.io/ip

echo
echo "[!] By google ns1"
dig TXT +short o-o.myaddr.l.google.com @ns1.google.com

echo
echo "[!] By opendns"
host myip.opendns.com resolver1.opendns.com | grep address
