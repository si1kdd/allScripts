#!/usr/bin/env bash
# @ Function
#       copy one file to all subdirectory.
#
# @ Usage:
#       ./cp_to_subfolder.sh FILE_NAME
#
# @ Resources:
#       https://www.linuxquestions.org/questions/linux-newbie-8/how-to-copy-a-file-to-all-subfolders-in-a-directory-using-a-single-command-820478/ 

FILE_COPY=$1
for i in *
do
        if [ -d "$i" ]
        then
                cp $FILE_COPY "$i"
        fi
done
