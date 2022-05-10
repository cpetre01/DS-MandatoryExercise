# client and server apps

# client app
#add_executable(${TARGET_CLIENT})
#target_sources(${TARGET_CLIENT} PRIVATE client.c)
#target_link_libraries(${TARGET_CLIENT} PRIVATE ${TARGET_KEYS})

# server app
add_executable(${TARGET_SERVER})
target_sources(${TARGET_SERVER} PRIVATE example_server.c ../src/rpc/example_xdr.c ../src/rpc/example_svc.c)
target_link_libraries(${TARGET_SERVER}
        PRIVATE pthread
                ${TARGET_NET_UTILS}
                ${TARGET_DBMS}
        )
