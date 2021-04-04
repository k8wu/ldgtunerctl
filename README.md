



# LDG Tuner Control

## Description

This program controls several aspects of the LDG AT-1000ProII and AT-600ProII automatic tuners, including antenna switching, automatic or manual tuning, bypassing the tuner, and of course, executing a tuning operation when a small amount of RF power is applied.

The reason for this program's existence is that LDG offers a Windows executable for controlling the tuner, but has nothing similar available for Linux or any other operating system. Some amateur radio operators (myself included) use Linux primarily in the shack, so now we don't have to resort to running Wine or some other stopgap solution.

This program is built on C++ using Qt. It is hoped that it will eventually run on most flavors of Linux and *BSD. It is mainly developed on a FreeBSD system, but tested on FreeBSD, Linux, and macOS.


## Hardware

Aside from the tuner itself, an FTDI interface cable is required. Details can be found in this PDF file on LDG's website: [USB Tuner Cable Info](https://ldgelectronics.com/wp-content/uploads/2019/06/USB-Tuner-Cable-Info.pdf)


## Dependencies

* Qt 5.x or later
* libserialport
* libusb

### Linux (Debian and Debian-Style Systems)

You can install these on Debian or Debian-style Linux systems (Ubuntu, Mint, Devuan, etc.) by issuing this command:

	# sudo apt install git qtbase5-dev libserialport-dev

There may be some issues with `ld` not being able to locate some libraries. That will be resolved in a future commit.

### FreeBSD

On FreeBSD, you can use the following command to get the necessary packages installed:

	# pkg install git qt5 libserialport

Alternately, you can use ports to compile these packages yourself.

### macOS (Homebrew)

Using Homebrew, you can execute the following commands to install the dependencies:

	% brew install qt libserialport

This will install Qt 6.x, which should be fine, but if you want to force 5.x, use `qt@5` in the preceding command instead of `qt`.


## Compilation

This program is known to compile using `clang++` on FreeBSD and macOS, and using `g++` on Linux. You should be able to execute the following to get `ldgtunerctl` built and installed:

	$ git clone https://github.com/k8wu/ldgtunerctl
	$ mkdir build-ldgtunerctl && cd build-ldgtunerctl
	$ qmake ../ldgtunerctl
	$ make -j4 # change the number if you have more or fewer logical CPU cores available

For macOS, this process will generate a standard macOS application directory whose name ends in .app, so you can run it directly from Finder (optionally copying it to your `/Applications` directory). Please note that on ARM based Macs (M1, and likely other Apple silicon chips in the future), you may have to grant permission to run unsigned applications (if you can even run them at all). Addressing this issue will cost me $100/year, so I am saving that for when this application is "production ready" :)

For the other operating systems, there is no automated installation process yet. If you want it to be available outside your home directory (or wherever you built it), you can execute the following, since the program runs from a single file:

	# install -m 755 ldgtunerctl /usr/local/bin/ldgtunerctl


## Running the program

Currently, as stated above, you can run `ldgtunerctl` from anywhere in your filesystem. When you first run the program, it will present a list of serial ports and ask you to choose one:

![Serial port selection window](https://k8wu.me/images/ldgtunerctl-qt-serial_port_selection_window.png)

If it does not, it is likely that you do not have permission to read and/or write to serial ports. An easy way around this on the various systems is:

### Linux (Debian, etc.)

	# adduser $(id -un) dialout

### FreeBSD

	# pw groupmod dialer -m $(id -un)

### macOS

This step should not be required.


## Configuring the Program

If you select a serial port and `ldgtunerctl` is able to successfully communicate with and configure the tuner, it will save a configuration file in your user profile directory (usually `${HOME}/.config/ldgtunerctl.conf`). On subsequent startups, it will automatically read this file (absent of any command line options to the contrary) and attempt to use the same serial port that is specified in that configuration file. If you run into any issues, you can either delete that file, or specify a different one using the `-c` command line option.


## Basic Operation

If all goes well, you will be presented with the main program screen:

![Main window](https://k8wu.me/images/ldgtunerctl-qt-main_window.png)

It should be fairly self-explanatory how to use the various buttons on this screen. As you do things like toggle between the two antenna inputs, set Auto or Manual tuning mode, bypass the tuner (which can be turned off by setting the tuner to Auto mode), or tell the tuner to execute a Memory or Full tune, you will see the various statuses and button colors change to indicate what has been done.


## Command Line Options

Current command line options as of v0.1.0:

	-h, --help                    Displays help on commandline options.
	--help-all                    Displays help including Qt specific options.
	-v, --version                 Displays version information.
	--debug-print, -p             Write debug output to stdout
	--debug-file, -f <debugfile>  Write debug output to specified file
	--conf, -c <conffile>         Specify configuration file to use other than
																default

You can run `ldgtunerctl -h` to get the most current options for whatever version you are using.


## Bugs and Features

All programs are imperfect by design, and `ldgtunerctl` is no exception :) This program is under active development, and here are the features that are planned for the near future:
* Configuration dialog: This can be managed from the command line, but it would be nice to have a configuration button where settings can be changed while in the program.
* Meter polling: This is an undocumented feature in the microcontroller for the AT-1000ProII and AT-600ProII tuners, but it can - in theory - be used to get a rough idea of how much power is going through the tuner, how much is being reflected, and what band/frequency has been detected. Of course, we can use some of this data to calculate SWR, R+j|X| impedance figure, return loss, etc. However, I will have to implement this before I know more, and the other tuner projects that I was able to find will guide me on this (namely, [/Efpophis/LDGControl](https://github.com/Efpophis/LDGControl)).
* Windows support?
