#! /bin/python
import argparse
import sys
from subprocess import Popen, PIPE

parser.add_argument('--size', dest='size', default=64, help='Packet Size')
parser.add_argument('--tcp', dest='tcp', action='store_true', help='Use TCP instead of UDP')
parser.add_argument('--ncores', dest='ncores', default=16, help='Number of Cores')


glopts = parser.parse_args()
glopts.size = int(glopts.size)
glopts.ncores = int(glopts.ncores)

for i in range(glopts.ncores):
    port = 5200 + i
    pargs = [ 'taskset', '-c', i, 'iperf3', '-s', '-p', port ]
    proc = Popen(pargs)

raw_input("Press Enter to start Clients...")

for i in range(glopts.ncores):
    port = 5200 + i
    pargs = [ 'taskset', '-c', i, 'iperf3', '-c', '-p', port, '-t', 60, '-l', glopts.size ]

    proc = Popen(pargs)


