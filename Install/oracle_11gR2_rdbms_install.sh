#!/usr/bin/env bash
#
# Install oracle 11g R2 step
# Suppose use preinstall rpm and use OracleLinux
#
# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# Unfinished ... don't use it !
# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


# Need root account, check here.


HOST_NAME=$1

# On CentOS 7
# cd /etc/yum.repos.d && wget https://public-yum.oracle.com/public-yum-ol7.repo

# Use preinstall pacage to perform all prerequisites
yum install oracle-rdbms-server-11gR2-preinstall

# On CentOS 7, Download GPG key
# wget https://public-yum.oracle.com/RPM-GPG-KEY-oracle-ol7 -O GPG_KEY
# mv GPG_KEY /etc/pki/rpm-gpg/RPM-GPG-KEY-oracle

# Change HOSTNAME
# hostnamectl set-hostname si1kdd.oracle
hostnamectl set-hostname $HOST_NAME

# Add HOSTNAME to network configuration
#
# HOST_NAME     $HOST_NAME
vim /etc/sysconfig/network

# Add public-ip with hostname
#
# 127.0.0.1     $HOST_NAME
# <public-ip>   $HOST_NAME
vim /etc/hosts

# Add "session required pam_limits.so"
vim /etc/pam.d/login

# Disable SELINUX
sed -i s/SELINUX=enforcing/SELINUX=disabled/ /etc/selinux/config
setenforce Permissive

# Disable firewalld, will enable while 11g installed
systemctl stop firewalld
systemctl disable firewalld


# To be continued  ...
