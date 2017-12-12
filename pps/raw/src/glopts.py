#! /usr/bin/python
import argparse
import sys

parser = argparse.ArgumentParser(description='Generate Packets')
parser.add_argument('--intf', dest='intf', default=None, help='Interface')
parser.add_argument('--sip', dest='sip', default=None, help='SrcIP')
parser.add_argument('--dip', dest='dip', default=None, help='DstIP')
parser.add_argument('--dmac', dest='dmac', default=None, help='DstMac')
parser.add_argument('--sport', dest='sport',
                    default=40001, help='SrcPort')
parser.add_argument('--dport', dest='dport',
                    default=50001, help='DstPort')
parser.add_argument('--ctrlport', dest='ctrlport',
                    default=5000, help='Control Port')
parser.add_argument('--size', dest='size', 
                    default=64, help='Packet Size')
parser.add_argument('--pcap', dest='pcap', 
                    default='pkt.pcap', help='Pcap file to be written')
parser.add_argument('--tcp', dest='tcp', 
                    action='store_true', help='TCP')
parser.add_argument('--sender', dest='sender', 
                    action='store_true', help='Run in sender mode.')
parser.add_argument('--receiver', dest='receiver', 
                    action='store_true', help='Run in receiver mode.')
parser.add_argument('--nflows', dest='nflows',
                    default=16, help='Number of Flows')

GlobalOptions = parser.parse_args()
GlobalOptions.sport = int(GlobalOptions.sport)
GlobalOptions.dport = int(GlobalOptions.dport)
GlobalOptions.ctrlport = int(GlobalOptions.ctrlport)
GlobalOptions.size = int(GlobalOptions.size)

error = False
if GlobalOptions.sender is False and GlobalOptions.receiver is False:
    print "Error: Sender or Receiver not specified."
    error = True
if GlobalOptions.intf is None:
    print "Error: Interface is not specified."
    error = True

if error is True:
    sys.exit(1)
