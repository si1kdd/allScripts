#!/usr/bin/env bash
# @ Function
#       Check the all ip information of my machine.
#
# @ Usage:
#       ./get_myIP.sh
#
# @ Author:
#       si1kdd

color_gre1='\e[1;32m'
color_gre2='\e[0;32m'
color_blu1='\e[1;34m'
color_blu2='\e[0;34m'
color_pur='\e[1;35m'
color_end='\e[0m'

echo -e "------------------------------------------------------"
my_local_ip=`ip a \
                | grep 'scope global' \
                | grep -Eo '[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}/[0-9]{1,2}'`
echo -e "[+] Local IP:$color_blu1 $my_local_ip $color_end"
echo
echo -e "[+] $color_blu2 IP information: (By akamai server) $color_end"
my_public_ip=`dig whoami.akamai.net. @ns1-1.akamaitech.net. +short`

if ! command -v whois > /dev/null 2>&1; then
        echo -e "[!] $color_pur Please install 'whois' or 'dig' command first ! $color_end"
fi
whois $my_public_ip | grep -Ei 'inetnum|netrange|netname|descr|cidr|organization|orgname|country'
echo -e "[+] Public IP:$color_gre1 $my_public_ip $color_end"
echo -e "------------------------------------------------------"

