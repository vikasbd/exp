all: rawperf

SRCS := src/common.c src/rawperf.c

CFLAGS := -g -std=c99
LIBS := -lpcap

rawperf: $(SRCS)
	mkdir -p bin
	gcc $(CFLAGS) $(SRCS) $(LIBS) -o bin/rawperf

clean:
	rm -f bin/rawperf
