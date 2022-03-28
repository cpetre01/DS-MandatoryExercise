# DS-MandatoryExercise
Repo used for both Mandatory Exercises in Distributed Systems

    This branch is used for Mandatory Exercise 2

Project Structure:

    app: source code for client and server executables

    build: directory used to build the project; create it if it doesn't exist

    include: header files

        dbms: header files for DBMS library

            dbms.h: function prototypes used for DB management; server-side API

            dbmsUtils.h: function prototypes called internally in the dbms module

        keys.h: header for keys library; client-side API

        utils.h: types, constants and function prototypes used throughout the project; useful stuff

    src: library source code and auxiliary source files

        dbms: source code for DBMS library
    
            dbms.c: source code for DB management; server-side API
    
            dbmsUtils.c: source code for the function prototypes defined in dbmsUtils.h
    
        keys.c: source code for keys library; client-side API
    
        utils.c: source code for the function prototypes defined in utils.h

    test: tests

        db_tests.c: tests for DBMS functionality
