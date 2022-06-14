# libraries

# netUtils library
add_library(${TARGET_NET_UTIL} STATIC)
target_sources(${TARGET_NET_UTIL}
        PRIVATE netUtil.c
        PUBLIC  util.c
        )
target_include_directories(${TARGET_NET_UTIL} PUBLIC ../include)

# keys dynamic library
add_library(${TARGET_KEYS} SHARED)
target_sources(${TARGET_KEYS} PRIVATE keys.c)
target_link_libraries(${TARGET_KEYS} PRIVATE ${TARGET_NET_UTIL})
target_include_directories(${TARGET_KEYS} INTERFACE ../include)

# dbms library code
add_subdirectory(dbms)
