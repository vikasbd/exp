ethtool -S ens5 > ethtool_start.log
echo "================ Start =================" > ifconfig.log
sudo ifconfig ens5 >> ifconfig.log
#sudo iperf3 -c 172.31.4.106 -P 10 -i 1 -t 60 -V -M -n -l 1500
#sudo iperf3 -c 172.31.4.106 -i 1 -t 3600 -V  -b 1000000000000 -P 64
sudo iperf3 -c 172.31.4.106 -P 10 -i 1 -t 60 -V -M -n -l 64 2>&1 | tee iperf.log
echo "================ End =================" >> ifconfig.log
sudo ifconfig ens5 >> ifconfig.log
ethtool -S ens5 > ethtool_end.log
