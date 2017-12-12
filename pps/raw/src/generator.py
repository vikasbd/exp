#! /usr/bin/python

from scapy import *
from scapy.all import *

class PacketGenerator():
    def __init__(self, options):
        self.options = options
        self.pkts = []
        return

    def Generate(self):
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
            l4.sport = self.options.sport + nf
            l4.dport = self.options.dport + nf

            pkt = pkt / l4 

            payload_size = self.options.size - len(pkt) - 4
            pkt = pkt / Raw(bytes('\x00') * payload_size)
            pkt = pkt / Raw(bytes('\xC0\x01\xD0\x0D'))
            #pkt.show2()
            #hexdump(pkt)
            self.pkts.append(pkt)

        wrpcap(self.options.pcap, self.pkts)

    def Cleanup(self):
        os.remove(self.options.pcap)
        return
