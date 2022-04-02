#TODO
keys.c & maybe server.c need work (lot of debugging left to do)

#Cleanup

#Bugs
values written to DB overflow: server needs to control length (need to understand strings better)

server remains blocked on accept, so Ctrl+C won't shut it down;
sending SIGKILL is the way to shut it down for now

#Improvements

#weird stuff
if MAX_CONN_BACKLOG == THREAD_POOL_SIZE, weird things happen, server gets SIGPIPE sometimes:
when more that MAX_CONN_BACKLOG connections are open and the last clients
that connected requests a service, we close that client, reopen it and
retry to request a service. Sometimes the server get a SIGPIPE