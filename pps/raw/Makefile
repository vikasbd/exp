all: rawperf

CFLAGS := -g
LIBS := -lpcap

rawperf: common.c rawperf.c
	mkdir -p bin
	gcc $(CFLAGS) common.c rawperf.c $(LIBS) -o bin/rawperf

clean:
	rm -f bin/rawperf
