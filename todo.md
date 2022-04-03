#TODO
do tests: keys-tests need work

#Cleanup

#Bugs
value1 written to DB overflows: server needs to control length (need to understand strings better)

Ctrl+C won't shut the server down;
sending SIGKILL is the way to shut it down for now;
if we don't destroy the cond vars, it works just fine;
destroying a cond var that a thread is blocked on is undefined behavior, hence things not working;
should we destroy the cond vars?

#Improvements

#Weird stuff
if MAX_CONN_BACKLOG == THREAD_POOL_SIZE, weird things happen, server gets SIGPIPE sometimes:
when more that MAX_CONN_BACKLOG connections are open and the last client
that connected requests a service, we close that client, reopen it and
retry to request a service. Sometimes the server get a SIGPIPE