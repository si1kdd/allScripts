#!/bin/bash
# @ Function
#       Check the file is empty or not.
#
# @ Usage:
#       ./check_empty_files.sh FILENAME
#
# @ Source:
#       https://www.cyberciti.biz/faq/linux-unix-script-check-if-file-empty-or-not/
#

file="$1"

[ $# -eq 0 ] && {
        echo "Usage: $0 filename";
        exit 1;
}

[ ! -f "$file" ] && {
        echo "Erro: $0 file not found.";
        exit 2;
}


if [ -s "$file" ]
then
        echo "[X] $file is not empty."

else
        echo "[-] $file is empty."
fi
