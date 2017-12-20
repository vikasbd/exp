#!/bin/bash

if [ "$#" -eq 0 ]; then
    echo "Usage: $0 <server/client> <num_cores> <start_core#> <server_ip> <tcp/udp> <pktsize>" >&2
    exit 1
fi

re='^[0-9]+$'
if ! [[ $2 =~ $re ]]; then
    echo "Error: $2 is not a number" >&2
    exit 1
fi

if ! [[ $3 =~ $re ]]; then
    echo "Error: $3 is not a number" >&2
    exit 1
fi

if [ "$1" == "server" ]; then
    killall iperf3
    for ((i=0;i<$2;i++))
    do
    let "core = $i+$3"
        echo "Starting server $i on CPU $core"
        let "port = 5200 + $i"
        taskset -c $core iperf3 -s -p $port &
    done
fi

if [ "$1" == "client" ]; then
    if [ "$#" -ne 6 ]; then
    echo "Usage: $0 client <num_cores> <start_core#> <server_ip>" >&2
    exit 1
    fi

    for ((i=0;i<$2;i++))
    do
    let "core = $i+$3"
        echo "Starting client $i on CPU $core"
        let "port = 5200 + $i"
       # UDP
		if [ "$4" == "udp" ]; then
	    	taskset -c $core iperf3 -V -c $4 -p $port -u -t 60 -b 5g -l$5 &
		else
	       	taskset -c $core iperf3 -V -c $4 -p $port -t 60 -M$5 -N -l$5  &
        fi
    done
fi

wait
