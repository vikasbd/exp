#! /usr/bin/python
import socket
import sys

from subprocess import Popen, PIPE
import src.generator as generator

class RawperfSender:
    def __init__(self, options):
        self.options = options
        self.__validate_options()
        self.pargs = pargs = ['./bin/rawperf', '-s',
                              '-i', self.options.intf,
                              '-f', 'pkt.pcap']
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        return

    def __validate_options(self):
        error = False
        if self.options.sip is None:
            print "Error: SIP is not specified."
            error = True
        if self.options.dip is None:
            print "Error: DIP is not specified."
            error = True
        if self.options.dmac is None:
            print "Error: DMAC is not specified."
            error = True
        if error is True:
            sys.exit(1)
        return

    def __connect(self):
        print "Connecting for Receiver.....",
        sys.stdout.flush()
        try:
            self.sock.connect((self.options.dip, self.options.ctrlport))
        except:
            print "Failed."
            print "Error: Receiver not ready."
            sys.exit(1)
        print "Connected"
        mesg = self.sock.recv(128)
        return

    def __generate(self):
        self.gen = generator.PacketGenerator(self.options)
        self.gen.Generate()
        return

    def __run(self):
        proc = Popen(self.pargs)
        proc.communicate()
        return

    def __cleanup(self):
        self.gen.Cleanup()
        return

    def Start(self):
        self.__generate()
        self.__connect()
        self.__run()
        self.__cleanup()
        return


