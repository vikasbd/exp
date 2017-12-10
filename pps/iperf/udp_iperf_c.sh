echo "================ Start =================" > ifconfig.log
sudo ifconfig ens5 >> ifconfig.log
sudo iperf3 -c 172.31.4.106 -u -b 1000m -P 10 -i 1 -t 60 -V -M -n -l 128
echo "================ End =================" >> ifconfig.log
sudo ifconfig ens5 >> ifconfig.log
