#!/usr/bin/env bash
# @ Function
#       Using awk to print out all kernel have been installed.
#
# @ Usage:
#       ./check_installed_kernel.sh
#
# @ Author:
#       si1kdd

RED="\e[0;49;31m"
GREEN="\e[0;49;32m"
END="\e[0m"

function uefi() {
        echo -e "[!] Your system booted as ${RED}UEFI${END}"
        BOOT="UEFI"
}

function bios() {
        echo -e "[!] Your system booted as ${GREEN}BIOS${END}"
        BOOT="BIOS"
}

function check_boot() {
        [ -d /sys/firmware/efi  ] && uefi || bios
        echo "---------------------------------------"
}

function check_super_user() {
        if [ "$EUID" -ne 0 ]; then
                echo "[!] Please run as root or use 'sudo' command ..."
                echo "          This scripts need privilege. (And only tested on CentOS and Arch Linux)"
                exit -1
        fi
}

function show() {
        echo "The kernel you installed : "

        if [[ ! -z $BOOT && $BOOT == "UEFI" ]]; then
                awk -F\' '$1=="menuentry " {print "["i++ "] : " $2}' /etc/grub2-efi.cfg
        elif [[ ! -z $BOOT && $BOOT == "BIOS" ]]; then
                awk -F\' '/menuentry /  {print "["i++ "] : " $2}' /boot/grub/grub.cfg
        fi
}

# Start here
check_boot && check_super_user && show

# TODO: tested the legacy grub version on different platform (BSD and all Linux platform)
