#!/bin/bash

INTERVAL="1"  # update interval in seconds
if [ -z "$1" ]; then
        echo
        echo usage: $0 [network-interface]
        echo e.g. $0 eth0
        echo shows packets-per-second
        exit
fi

IF=$1
function print_pps()
{
	R1=`cat /sys/class/net/$IF/statistics/rx_packets`
	T1=`cat /sys/class/net/$IF/statistics/tx_packets`
	sleep $INTERVAL
	R2=`cat /sys/class/net/$IF/statistics/rx_packets`
	T2=`cat /sys/class/net/$IF/statistics/tx_packets`
	#TXPPS=`expr $T2 - $T1`
	#RXPPS=`expr $R2 - $R1`
	#TOTAL=`expr $TXPPS + $RXPPS`
	TXPPS=$(echo "scale = 3; ($T2 - $T1)/1000000" | bc -l)
	RXPPS=$(echo "scale = 3; ($R2 - $R1)/1000000" | bc -l)
	TOTAL=$(echo "scale = 3; $TXPPS + $RXPPS" | bc -l)
	PPS=`echo "Tx $TXPPS Mpps, Rx $RXPPS Mpps, Total $TOTAL Mpps  |"`
}

function print_bps()
{
   R1=`cat /sys/class/net/$IF/statistics/rx_bytes`
   T1=`cat /sys/class/net/$IF/statistics/tx_bytes`
   sleep $INTERVAL
   R2=`cat /sys/class/net/$IF/statistics/rx_bytes`
   T2=`cat /sys/class/net/$IF/statistics/tx_bytes`
   TBPS=`expr $T2 - $T1`
   RBPS=`expr $R2 - $R1`
   TXGbps=$(echo "scale = 3; ($TBPS * 8)/1077936128" | bc -l)
   RXGbps=$(echo "scale = 3; ($RBPS * 8)/1077936128" | bc -l)
   TOTAL=$(echo "scale = 3; $TXGbps + $RXGbps" | bc -l)
   BPS=`echo "Tx $TXGbps Gbps, Rx $RXGbps Gbps, Total $TOTAL Gbps"`
}


while true
do
    print_pps
    print_bps
    echo $PPS $BPS
done

