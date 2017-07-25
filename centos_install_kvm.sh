#!/usr/bin/env bash
# @ Function
#       Just a script to install kvm stuff on RHEL/CentOS.
#
# @ Usage:
#       ./centos_install_kvm.sh
#
# @ Author:
#       si1kdd

DNF="dnf"
YUM="yum"
USE=""

function command_exists() {
        if ! [ -x "$(command -v $1)" ]; then
                return 1
        else
                return 0
        fi
}

function check_dnf_exists() {
        # check dnf exists or not.
        if command_exists $DNF; then
                echo "[*] dnf installed, using dnf."
                USE=$DNF
        elif command_exists $YUM; then
                echo "[*] yum installed, using yum."
                USE=$YUM
        else
                USE=""
                echo "[*] Seems 'dnf' and 'yum' doesn't exists, please check the \$PATH values ..."
        fi
}

function install_kvm() {
        check_dnf_exists
        if [[ ! -z $USE ]]; then
                $USE -y groupinstall "Virtualization"
                $uSE -y install qemu-img libvirt libvirt-python libvirt-client
                echo "[!] Setup daemon "
                echo "--------------------------------------------------"
                systemctl start libvirtd && systemctl enable libvirtd
        fi
        exit -1
        echo -n $USE
}

echo "[!] This script just work on RHEL/CentOS.         "
echo "[!] Please run this script in super user mode.    "
echo "--------------------------------------------------"

install_kvm
