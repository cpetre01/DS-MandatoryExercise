# libraries

# keys dynamic library
add_library(${TARGET_KEYS} SHARED)
target_sources(${TARGET_KEYS} PRIVATE keys.c rpc/rpc_clnt.c rpc/rpc_xdr.c)
target_include_directories(${TARGET_KEYS} PUBLIC ../include)

# dbms library code
add_subdirectory(dbms)
