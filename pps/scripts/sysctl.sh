#! /bin/bash
sudo sysctl net.core.rmem_max=134217728
sudo sysctl net.core.wmem_max=134217728
sudo sysctl net.ipv4.tcp_wmem="10240 87380 134217728"
sudo sysctl net.ipv4.tcp_rmem="10240 87380 134217728"
