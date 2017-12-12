#! /usr/bin/python
import src.sender as sender
import src.receiver as receiver

from src.glopts import GlobalOptions as GlobalOptions

if GlobalOptions.sender:
    tester = sender.RawperfSender(GlobalOptions)
else:
    tester = receiver.RawperfReceiver(GlobalOptions)

tester.Start()
