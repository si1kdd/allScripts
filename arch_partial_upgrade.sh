#!/usr/bin/env bash
# @ Function
#       Use 'pacman' to do the partial upgrade,
#       Yes, do this dangerous staff if you don't wanna to do the full system upgrade.
#
# @ Usage:
#       ./arch_partial_upgrade.sh THE_PACKAGES_YOU_WANNA_UPGRADE
#
# @ Author:
#       si1kdd
#
# @ Status:
#       Unfinished ...

set -e

msg_help() {
        echo "[*] Usage: ./$0 PACKAGE_NAME"
        exit -1
}

check_upgrade() {
        upgrade=$(pacman -Qu | wc -l)
        echo "[+] Now Total $upgrade packages could being upgrade."
        exit 1
}

check_super_user() {
        if [ "$EUID" -ne 0 ]; then
                echo "[!] Please run as root or use 'sudo' command ..."
                echo "          This scripts need privilege. (And only tested on CentOS and Arch Linux)"
                exit -1
        fi
}

if [[ -z $1 ]]; then
        msg_help
        exit -1
fi

exec_str='sudo pacman -S '
packages=($(pacman -Ss $1 | grep installed: | cut -d/ -f2 | awk '{print $1}'))

echo "[+] Total ${#packages[@]} packages would be upgrade:"
printf "\t"
for i in ${packages[@]}; do
        printf "${i} "
        exec_str=$exec_str' '${i}
done

echo
echo

check_super_user
# echo $exec_str
eval $exec_str

echo "[*] Done!"
check_upgrade

# TODO: may add new features.
