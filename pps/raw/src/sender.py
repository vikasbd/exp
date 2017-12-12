#! /usr/bin/python

from subprocess import Popen, PIPE
import src.generator as generator

class RawperfSender:
    def __init__(self, options):
        self.options = options
        self.pargs = pargs = ['./bin/rawperf', '-s',
                              '-i', self.options.intf,
                              '-f', 'pkt.pcap']
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        return

    def __connect(self):
        print("Connecting for Receiver.....",)
        self.sock.connect((self.options.dip, self.options.ctrlport))
        print("Done")
        mesg = self.sock.recv(128)
        print(mesg)
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


