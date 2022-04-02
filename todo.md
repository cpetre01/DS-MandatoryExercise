#TODO
keys.c & maybe server.c need work (lot of debugging left to do)

#Cleanup

#Bugs
values written to DB overflow: server needs to control length

server remains blocked on accept, so Ctrl+C won't shut it down;
sending SIGKILL is the way to shut it down for now

one service call works, but the second doesn't
(client connection remains open, might be why)

#Improvements
