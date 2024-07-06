# Simulation_Testbed

Testbed Host and Common VMs Configuration
==========================================

A virtualization software that helps running multiple operating systems in a single host.
In our Testbed we will use VirtualBox software. VirtualBox will the host the two VMs running Ubuntu Desktop Linux based OS environment.

For ease on managing the testbed files, we created a /research folder under /home directory, where all the following command/packages installation to be run inside using the the Ubuntu Desktop Terminal.

• git: Distributed version control system
– sudo apt-get install git

• libiec61850: Open-source library for the IEC61850 protocols
– git clone https://github.com/mz-automation/libiec61850/
– cd libiec61850
– make

• OpenPLC Simulink-Interfacelibiec61850: Interface between the OpenPLC Simulink driver and a Simulink model using UDP Send and Receive blocks
– git clone https://github.com/thiagoralves/OpenPLC_Simulink-Interface

• net-tools: networking utilities for Linux including ifconfig
– sudo apt install net-tools

• tshark: Terminal-based Wireshark
– sudo apt install tshark

• dbus: Simple interprocess messaging system to call multiple terminals
– sudo apt install dbus-x11


Testbed VM#1 Setup Configuration
==================================

This section will cover configuration applicable only to VM#1 where the following configuration/packages installation are required.

• VM#1 enp0s3 Interface IP Address: 23.23.23.254/24
– sudo ifconfig enp0s3 23.23.23.254 netmask 255.255.255.0 up

• MATLAB/Simulink: Block diagram environment used to design systems with multidomain models. List of features required when installing MATLAB:
– Distance Protection Relay
– DSP System Toolbox
– Instrument Control Toolbox
– Parallel Computing Toolbox
– Signal Processing Toolbox
– Simscape
– Simscape Electrical
– Simulink


Testbed VM#2 Setup Prerequisites
=====================================

This section will cover configuration applicable only to VM#2 where the following configuration/packages installation are required.

• VM#2 enp0s3 Interface IP Address: 23.23.23.253/24
– sudo apt install net-tools
– sudo ifconfig enp0s3 23.23.23.253 netmask 255.255.255.0 up

• OpenPLC Editor on Linux
• OpenPLC Runtime on Linux
– git clone https://github.com/thiagoralves/OpenPLC v3.git
– cd OpenPLC v3
– ./install.sh linux



