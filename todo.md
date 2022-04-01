#TODO
keys.c & maybe server.c need work (lot of debugging left to do)

#Cleanup
get rid of all mqueue stuff

#Bugs
values written to DB overflow: server needs to control length

server remains blocked on accept, so Ctrl+C won't shut it down;
sending SIGKILL is the way to shut it down for now

#Improvements
don't send key back (get_value service)
