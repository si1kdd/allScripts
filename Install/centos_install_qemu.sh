#!/usr/bin/env bash

CORE=$(nproc --all)

#####
# First build
#####

first_build()
{
        yum install libfdt-devel \
                glib2-devel \
                pixman-devel \
                zlib-devel \
                libaio-devel \
                libcap-devel \
                libiscsi-devel \
                SDL2 SDL2-devel \

        git clone git://git.qemu-project.org/qemu.git
}

#####
# Update
#####
update()
{
        cd ./qemu && git pull
}


build()
{
        mkdir -p ./qemu/build/debug/native && cd ./qemu/build/debug/native
        ../../../configure --enable-debug --prefix=/usr/local
        make -j$CORE
}

if [[ $1 == 'build' ]]; then
        first_build && build
elif [[ $1 == 'update' ]]; then
        update && build
fi
