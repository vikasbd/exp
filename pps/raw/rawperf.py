#! /usr/bin/python
import argparse
import src.sender as sender
import src.receiver as receiver

parser = argparse.ArgumentParser(description='Generate Packets')
parser.add_argument('--intf', dest='intf',
                    default='eth0', help='Interface')
parser.add_argument('--sip', dest='sip',
                    default='10.0.0.1', help='SrcIP')
parser.add_argument('--dip', dest='dip',
                    default='10.0.0.2', help='DstIP')
parser.add_argument('--dmac', dest='dmac',
                    default='00:01:02:03:04:05', help='DstMac')
parser.add_argument('--sport', dest='sport',
                    default=1234, help='SrcPort')
parser.add_argument('--dport', dest='dport',
                    default=5678, help='DstPort')
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

GlobalOptions = parser.parse_args()
GlobalOptions.sport = int(GlobalOptions.sport)
GlobalOptions.dport = int(GlobalOptions.dport)
GlobalOptions.ctrlport = int(GlobalOptions.ctrlport)
GlobalOptions.size = int(GlobalOptions.size)

if GlobalOptions.sender:
    tester = sender.RawperfSender(GlobalOptions)
else:
    tester = receiver.RawperfReceiver(GlobalOptions)

tester.Start()
