#TODO
do tests

#Cleanup

#Bugs
value1 written to DB overflows: server needs to control length (need to understand strings better)

#Improvements

#Weird stuff
if MAX_CONN_BACKLOG == THREAD_POOL_SIZE, weird things happen, server gets SIGPIPE sometimes:
when more that MAX_CONN_BACKLOG connections are open and the last client
that connected requests a service, we close that client, reopen it and
retry to request a service. Sometimes the server get a SIGPIPE