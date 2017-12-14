#! /usr/bin/python
import socket
import sys

from subprocess import Popen, PIPE
import src.generator as generator
import src.glopts as glopts

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

    def __listen_data_ports(self):
        if self.options.tcp is False:
            return

        self.dsocks = []
        for t in range(self.options.threads):
            for f in range(self.options.nflows):
                sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                port = glopts.CalculateDstPort(t,f)
                sock.bind(('0.0.0.0', port))
                sock.listen(1)
                print "Listening on port: ", port
                self.dsocks.append(sock)
        return

    def __accept_data_connections(self):
        for sock in self.dsocks:
            conn, addr = self.sock.accept()
            print "Accepted data connection from: ", addr
        return

    def __control_handshake(self):
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
        self.__listen_data_ports()
        self.__control_handshake()
        self.__accept_data_connections()
        self.__run()
        return


