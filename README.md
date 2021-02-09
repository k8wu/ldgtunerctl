# LDG Tuner Control

## Description

This program will control several aspects of the LDG AT-1000ProII and AT-600ProII automatic tuners, including antenna switching, automatic or manual tuning, bypassing the tuner, and of course, executing a tuning operation when a small amount of RF power is applied.

The reason for this program's existence is that LDG offers a Windows executable for controlling the tuner, but has nothing similar available for Linux or any other operating system. Some amateur radio operators (myself included) use Linux primarily in the shack, so now we don't have to resort to running Wine or some other stopgap solution.

This program is built on C++ using FLTK. It should build using **gcc** (`g++`), **clang** (`clang++`), or any other C++11 compliant compiler on Linux. It is known to work on at least a Raspberry Pi 4 (which was the main development system), and should work on the Pi 2 and Pi 3 as well. Raspbian was tested, but it should work on any reasonably modern Linux system.

As for other operating systems: The program compiles on macOS if the `--std=c++11` flag is passed to `clang++`, but serial port selection does not function well on that platform, and it has not been tested beyond that. There has been no testing at all yet on FreeBSD, OpenBSD, or any other OS.

## Hardware

Aside from the tuner itself, an FTDI interface cable is required. Details can be found in this PDF file on LDG's website: [USB Tuner Cable Info](https://ldgelectronics.com/wp-content/uploads/2019/06/USB-Tuner-Cable-Info.pdf)


## Compilation

**LDG Tuner Control** requires both the libraries/binaries and development headers for FLTK. On Debian, Raspbian, Ubuntu, or any other Debian-style system, these can be installed by executing the following command after enabling the proper **deb-src** line(s) in **/etc/apt/sources.list**:
> sudo apt install libfltk1.3 libfltk1.3-dev

This program has not yet been shoehorned into `autoconf`, `cmake`, or any other such system yet. That day is coming, but for now, the following commands should get it built after either cloning or extracting the source:
> cd ldgtunerctl # or whatever your source directory is named
>
> g++ -Wall -o ldgtunerctl $(fltk-config --cxxflags) $(fltk-config --ldflags) src/classes/*.cxx src/main.cxx
>
> sudo install -m 755 ldgtunerctl /usr/local/bin/ldgtunerctl # change path if you don't want to use /usr/local/bin

## Running the program

*NOTE: It is highly recommended at this early stage in this program's development that you run it from a graphical terminal such as `xterm`, `lxterminal`, or `rxvt` instead of directly with your window manager.*

When you run the program, it will present a window with a list of serial ports, much like this:

![Serial port selection window](https://k8wu.me/images/ldgtunerctl-serial_port_selection_window.png)

You can choose the port where your tuner lives. If you do not know which one it is (or if `udev` is playing tricks on you upon each reboot), you can try each one on the list until you find one that works. If the program exits with an error sent to standard output about not being able to detect any devices, please ensure that your user is in the `dialout` group (for Debian-style systems):
> sudo adduser $(id -un) dialout

Then log out and back in for the change to take effect.

If everything works and your tuner is detected (i.e. it doesn't complain about not being able to sync or something), you will be presented with a screen that looks something like this:

![Main window](https://k8wu.me/images/ldgtunerctl-main_window.png)

This might not look like much, since the program doesn't have any information about the state of your tuner yet other than the fact that it exists and is available for control. The serial interface on the tuner does not support querying for information about the current tuner state. However, everything should work. If you toggle the antenna twice, it will be set to the same antenna that it was on when you ran the program. You can apply a few watts of RF on any band from 160m through 6m and execute a memory tune or a full tune. If you want, you can set the auto/manual mode, or even bypass the tuner entirely (i.e. for testing purposes, or if your antenna really does work at 1:1 SWR without a tuner).

## Bugs and features

All programs are imperfect by design, and **LDG Tuner Control** is no exception :) This program is under active development, and here are the features that are planned for the near future:
* Configuration (dialog/loading/saving - i.e. so it remembers which serial port you use, or if you want to change it later)
* Code cleanup in especially the serial and tuner interface classes
* macOS support (and maybe FreeBSD and OpenBSD as well)
