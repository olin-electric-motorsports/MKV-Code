#!/bin/sh
echo "Installing libraries"
sudo apt-get update
sudo apt-get install gcc-avr binutils-avr gdb-avr avr-libc avrdude python3 python3-pip
echo "Installing python dependencies"
sudo pip3 install -r requirements.txt
echo "Writing directory shortcuts"
