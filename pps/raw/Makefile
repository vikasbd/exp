all: rawperf

CFLAGS := -g
LIBS := -lpcap

rawperf: common.c rawperf.c
	gcc $(CFLAGS) common.c rawperf.c $(LIBS) -o rawperf

