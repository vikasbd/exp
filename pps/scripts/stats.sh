#!/bin/bash

# set -fuxe -o pipefail

INTERVAL="1"  # update interval in seconds
if [ -z "$1" ]; then
        echo
        echo usage: $0 [network-interface]
        echo e.g. $0 eth0
        echo shows packets-per-second
        exit
fi

IF=$1
function print_pps
{
    TXPPS=$(echo "scale = 3; ($4 - $2)/1000000" | bc -l)
    RXPPS=$(echo "scale = 3; ($3 - $1)/1000000" | bc -l)
    TOTAL=$(echo "scale = 3; $TXPPS + $RXPPS" | bc -l)
    PPS=`echo "Tx $TXPPS Mpps, Rx $RXPPS Mpps, Total $TOTAL Mpps  |"`
}

function print_bps
{
   TBPS=`expr $4 - $2`
   RBPS=`expr $3 - $1`
   TXGbps=$(echo "scale = 3; ($TBPS * 8)/1073741824" | bc -l)
   RXGbps=$(echo "scale = 3; ($RBPS * 8)/1073741824" | bc -l)
   TOTAL=$(echo "scale = 3; $TXGbps + $RXGbps" | bc -l)
   BPS=`echo "Tx $TXGbps Gbps, Rx $RXGbps Gbps, Total $TOTAL Gbps"`
}

function print_pps_bps()
{
   BR1=`cat /sys/class/net/$IF/statistics/rx_bytes`
   BT1=`cat /sys/class/net/$IF/statistics/tx_bytes`
   PR1=`cat /sys/class/net/$IF/statistics/rx_packets`
   PT1=`cat /sys/class/net/$IF/statistics/tx_packets`
   sleep $INTERVAL
   BR2=`cat /sys/class/net/$IF/statistics/rx_bytes`
   BT2=`cat /sys/class/net/$IF/statistics/tx_bytes`
   PR2=`cat /sys/class/net/$IF/statistics/rx_packets`
   PT2=`cat /sys/class/net/$IF/statistics/tx_packets`
   print_pps "$PR1" "$PT1" "$PR2" "$PT2"
   print_bps "$BR1" "$BT1" "$BR2" "$BT2"
}

while true
do
    print_pps_bps
    echo $PPS $BPS
done
~
