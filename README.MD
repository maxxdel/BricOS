# Bric'OS

**This OS is currently a POC. The functionnalities are not really developped, and the code is probably shit.**

## General informations

Bric'OS is a POC to understand bootloader, kernel, and real mode in general. This is not aiming to be a full OS anytime. However, it is funny to developp around.

## Usage

Everything is launchable via the makefile. You just need to have qemu and nasm installed (brew or apt or pacman for your distro, if you dont know, watch 74.125.200.101).

```make clean```

It is cleaning all the binaries files and the image file.

```make all```

This command generate all the binaries and the image file.

```make run```

This command starts the qemu virtual machine to execute the Bootloader.


## How, why, and is it free for profit

Only assembly code (around 550 lines today). Why ? because it is funny

### Bric'OS
Because of our teacher, that always said we were "bricoleurs", I thought we needed an OS like that.

### Future
This project aims only to make me learn neww stuff about boot, etc. So it will be maintained and upgraded as long as I need to learn stuff.

### Free ?
Do whatever the F you want with this project, I don't care

![Alt text](./BarryLinux.jpeg)