#!/bin/bash

HEX_DIR="build/zephyr/"
MNT_DIR="/mnt/jlink/"
DEFAULT_ID=1000

alt_mount() {
if [ -z "$1" ]; then
        echo -e "No argument given.\n"
        alt_commands
else
        echo "Mounting drive $1 at /mnt/jlink/"
        if [ ! -d "${MNT_DIR}" ]; then
                echo -e "\nError: ${MNT_DIR} does not exist"
        else
                mount -t drvfs $1: -o umask=0022,gid=${DEFAULT_ID},uid=${DEFAULT_ID} ${MNT_DIR} > /dev/null 2>&1
                if [ $? -eq 0 ]; then
                        echo "Mounted jlink successfully"
                else
                        echo "Error: double check if drive $1: exists and if this command is run as root"
                fi
        fi
fi
}

alt_help() {
        echo "Alt_flash is an alternative tool/script for programming jlink devices."
        alt_commands
}

alt_commands() {
        echo "Available commands:"
        echo -e "\t<default>\t<none>\t\t\tProgram a jlink device with ${HEX_DIR}zephyr.hex"
        echo -e "\t-m\t\t<windows drive letter>\tMount the jlink device at ${MNT_DIR}"
        echo -e "\t-h\t\t<none>\t\t\tDisplay this message"
}

alt_flash() {
if grep -qs '/mnt/jlink ' /proc/mounts; then
        echo "Copying to jlink.."
        cp ${HEX_DIR}zephyr.hex ${MNT_DIR} > /dev/null 2>&1
        if [ $? -eq 0 ]; then
                echo "Successfully copied the .hex file to the jlink device"
        else
                echo "Error: copying to jlink device unsuccessful"
        fi

else
        echo "Jlink device not mounted."
        echo "Try running alt_flash -h for help."
fi
}

case $1 in
        -m)
                alt_mount $2
                ;;
        -h)
                alt_help
                ;;
        *)
                alt_flash
                ;;
esac