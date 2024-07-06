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
g++ simlink_VM2.cpp -o simlink_VM2 -pthread

#Collect GOOSE pcap from selected interface and dump to file
gnome-terminal -t GOOSE_PCAP -- tshark -i "$selectedInterface" -w /media/sf_simulation/VM2_GOOSE.pcapng -f "not udp" -F pcap

#Collect UDP pcap from selected interface and dump to file
gnome-terminal -t UDP_PCAP -- tshark -i "$selectedInterface" -w /media/sf_simulation/VM2_UDP.pcapng -f "udp" -F pcap

#Run simlink -- the interface among libiec61850, OpenPLC and MATLAB/Simulink
gnome-terminal -t OSI_VM2 -- ./simlink_VM2

#Running program Publisher from VM2
gnome-terminal -t libiec61850_PUB -- ./goose_publisher_VM2 "$selectedInterface"
