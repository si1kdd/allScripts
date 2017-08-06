#!/usr/bin/env bash
# @ Function
#       Using mysqldump backup MySQL tables.
#
# @ Usage:
#       default pass parameter to this scripts.
#       ./backup_mysql_db.sh < info.txt
#
# @ Author:
#       si1kdd
#
# @ Status:
#       Unfinished ...

DB_NAME=$1
DB_USERNAME=$2
DB_USER_PASSWD=$3
BACKUP_DB_PATH=$4

DATE=`date '+%Y%m%d-%H%M'`
DATA_DUMP=$BACKUP_DB_PATH-$DATE.sql

echo "[*] Delete Old backup data ... "
sleep 10
DATE_N=`date -d "-30 day" +%Y%m%d`
rm -r $BACKUP_DB_PATH-$DATE_N*

echo "[*] Backup data ... "
sleep 5
mysqldump $1 -u $2 -p $3 > $DATA_DUMP

echo "[*] Compress data ... "
sleep 5
# Should support encrytion ?!
tar -cvf $BACKUP_DB_PATH$1-$DATE.tar.gz $DATA_DUMP

echo "[*] Delete Dump data ... "
rm -r $DATA_DUMP


# TODO. better Intereaction and support different DB.
