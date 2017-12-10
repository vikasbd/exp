handler()
{
    killall udpreceiver
}

trap handler SIGINT

for i in `seq 0 $MAX_CORES`
do
    sudo taskset -c $i ./udpreceiver1 0.0.0.0:400$i > rx_$i.log &
done

wait
