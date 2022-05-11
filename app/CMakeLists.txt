# client and server apps

# client app
add_executable(${TARGET_CLIENT})
target_sources(${TARGET_CLIENT} PRIVATE client.c ../src/utils.c)
target_link_libraries(${TARGET_CLIENT} PRIVATE ${TARGET_KEYS})

# server app
add_executable(${TARGET_SERVER})
target_sources(${TARGET_SERVER} PRIVATE server.c ../src/rpc/rpc_xdr.c ../src/rpc/rpc_svc.c)
target_link_libraries(${TARGET_SERVER}
        PRIVATE pthread
                ${TARGET_DBMS}
        )
