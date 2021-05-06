# MKV-Code
MKV-Code

## Getting Libraries

Before you can compile/flash boards, you will need to install some AVR libraries
and a configuration file on your system. There are two methods for doing this:
with an Ansible playbook or with shell scripts.

### Ansible Setup

The Ansible playbook in `packages.yml` can be used to install the libraries and
configuration you need. The playbook is *idempotent*, which means that you can
run it as many times as you want and it will generally not repeat steps that it
has already done on your system.

To get started, you will need to install Ansible on your system:

```bash
$ sudo apt install ansible
```

Then, you need to run the Ansible playbook **from this directory** like this:

```bash
$ ansible-playbook -K packages.yml
```

### Shell Script Setup

Note that this method is an alternative to the above, so you only need to do one
of them. If you have already run the Ansible playbook, you can skip this
section.

First run the `setup.sh` script to get all the defualt AVR libraries.
```bash
$ sudo bash setup.sh
```

Then you need to add an additional AVR library for the specific MCU we use (ATmega16m1)
```bash
$ bash getDir.sh
```

## Compiling and Flashing Boards
To flash your C code onto an ATmega, you need to compile that code into a certain type that the ATmega can read. Sounds compilcated! Luckily, we have a nice series of scripts that does it all for you!
```bash
$ python3 make.py
```
Then just enter what board you want to flash and select if you want to flash it or just compile it.
If you're flashing the board for the first time make sure to set the fuse bits on the MCU by typing "fuses" when prompted like so:
```bash
$ Flash (y/n) or Set Fuses(fuses):fuses
```
Voila! Happy programming 
