#!/bin/bash

# Get available interfaces
mapfile -t interfaces < <(ifconfig | grep '^[a-z]' | awk '{print $1}' | sed 's/://')

# Display available interfaces
echo "Available network interfaces:"
for i in "${!interfaces[@]}"; do
    echo "$((i+1)). ${interfaces[i]}"
done

# Ask user to select an interface
read -p "Select the interface you want to use (enter the number): " selectedIndex
selectedInterface="${interfaces[$((selectedIndex-1))]}"

echo "Using interface: $selectedInterface"

# Compile simlink.cpp
g++ simlink_VM1.cpp -o simlink_VM1 -pthread

#Collect GOOSE pcap from selected interface and dump to file
gnome-terminal -t GOOSE_PCAP -- tshark -i "$selectedInterface" -w /media/sf_simulation/VM1_EXT_UDP_GOOSE.pcapng -F pcap

#Collect UDP pcap from selected interface and dump to file
gnome-terminal -t UDP_PCAP -- tshark -i lo -w /media/sf_simulation/VM1_UDP_INT.pcapng -f "udp" -F pcap

# Run simlink -- the interface between libiec61850 and MATLAB/Simulink
gnome-terminal -t OSI_VM1 -- ./simlink_VM1

# Running program Subscriber from VM1
gnome-terminal -t libiec61850_SUB -- ./goose_subscriber_VM1 "$selectedInterface"

