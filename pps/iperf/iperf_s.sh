ethtool -S ens5 > ethtool_start.log
echo "================ Start =================" > ifconfig.log
sudo ifconfig ens5 >> ifconfig.log
sudo iperf3 -s > tee iperf.log
echo "================ End =================" >> ifconfig.log
sudo ifconfig ens5 >> ifconfig.log
ethtool -S ens5 > ethtool_end.log
