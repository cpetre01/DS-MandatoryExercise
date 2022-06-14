# DS-MandatoryExercise
Repo used for both Mandatory Exercises in Distributed Systems

    This branch is used for Mandatory Exercise 2

Project Structure:

    app: source code for client and server executables

    build: directory used to build the project; create it if it doesn't exist

    extern: directory that includes googletest; required for unittests; create it if it doesn't exist

    include: header files

        dbms: header files for DBMS library

            dbms.h: function prototypes used for DB management; server-side API

            dbmsUtil.h: function prototypes called internally in the dbms module

        keys.h: header for keys library; client-side API
        
        netUtil.h: header for netUtil library; contains function prototypes used to send and receive stuff; network API used by both server and client

        util.h: types, constants and function prototypes used throughout the project; useful stuff

    src: library source code and auxiliary source files

        dbms: source code for DBMS library
    
            dbms.c: source code for DB management; server-side API
    
            dbmsUtil.c: source code for the function prototypes defined in dbmsUtil.h
    
        keys.c: source code for keys library; client-side API
        
        netUtil.c: source code for netUtil library; network API
    
        util.c: source code for the function prototypes defined in util.h

    test: unittests with GoogleTest
        

Unittests with GoogleTest:

    keys (Client API): tests services; requires a running server
