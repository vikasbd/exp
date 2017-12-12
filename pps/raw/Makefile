all: sender receiver

CFLAGS := -g
LIBS := -lpcap

sender:
	gcc $(CFLAGS) common.c sender.c $(LIBS) -o sender

receiver:
	gcc $(CFLAGS) common.c receiver.c $(LIBS) -o receiver
