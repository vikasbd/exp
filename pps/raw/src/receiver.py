#! /usr/bin/python
import socket
import sys

from subprocess import Popen, PIPE
import src.generator as generator

class RawperfReceiver:
    def __init__(self, options):
        self.options = options
        self.pargs = pargs = ['./bin/rawperf', '-r', '-i', options.intf ]
        return

    def __setup(self):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        return

    def __run(self):
        proc = Popen(self.pargs)
        proc.communicate()
        return

    def __listen(self):
        print "Waiting for Sender....",
        sys.stdout.flush()
        self.sock.bind(('0.0.0.0', self.options.ctrlport))
        self.sock.listen(1)
        conn, addr = self.sock.accept()
        print "Connected."
        conn.send('READY')
        return

    def Start(self):
        self.__setup()
        self.__listen()
        self.__run()
        return


