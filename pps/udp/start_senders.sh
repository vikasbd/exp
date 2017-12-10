handler()
{
    killall udpsender
}


trap handler SIGINT

for i in `seq 0 $MAX_CORES`
do
    echo "sudo taskset -c $i ./udpsender 172.31.13.221:400$i"
    taskset -c $i ./udpsender 172.31.13.221:400$i &
done

wait
