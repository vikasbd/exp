#! /usr/bin/python

from scapy import *
from scapy.all import *

class PacketGenerator():
    def __init__(self, options):
        self.options = options
        self.pkts = []
        self.pcaps = []
        return

    def __generate(self, thread_id):
        thread_pkts = []
        for nf in range(self.options.nflows):
            pkt = Ether()/IP()
            pkt[Ether].dst = self.options.dmac
            pkt[IP].src = self.options.sip
            pkt[IP].dst = self.options.dip

            if self.options.tcp:
                l4 = TCP()
                l4.flags = 'A'
            else:
                l4 = UDP()
            l4.sport = self.options.sport +\
                       thread_id * self.options.nflows + nf
            l4.dport = self.options.dport +\
                       thread_id * self.options.nflows + nf

            pkt = pkt / l4 

            payload_size = self.options.size - len(pkt) - 4
            pkt = pkt / Raw(bytes('\x00') * payload_size)
            pkt = pkt / Raw(bytes('\xC0\x01\xD0\x0D'))
            #pkt.show2()
            #hexdump(pkt)
            thread_pkts.append(pkt)

        self.pkts.append(thread_pkts)
        pcapfile = self.GetPcapFile(thread_id)
        wrpcap(pcapfile, thread_pkts)

    def Cleanup(self):
        for p in self.pcaps:
            os.remove(p)
        return

    def GetPcapFile(self, thread_id):
        return self.pcaps[thread_id]

    def Generate(self):
        for t in range(self.options.threads):
            self.pcaps.append('thr%d.pcap' % t)
            self.__generate(t)
        return
