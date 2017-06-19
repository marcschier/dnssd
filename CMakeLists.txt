#
# Cmake project settings
#
cmake_minimum_required(VERSION 2.8.11)

project(dnssd)
include(CMakeToolsHelpers OPTIONAL)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(_PROJECT_ROOT ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "Root" FORCE)

enable_testing()

#
# output paths
#
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
        set (CMAKE_C_FLAGS "--std=c99 ${CMAKE_C_FLAGS}")
        set (CMAKE_CXX_FLAGS "--std=c++11 ${CMAKE_CXX_FLAGS}")
    endif()
else()
    set (CMAKE_C_STANDARD 99)
    set (CMAKE_CXX_STANDARD 11)
endif()

    set(source_c_files
        src/DebugServices.c
        src/dnssd_clientlib.c
        src/dnssd_clientstub.c
        src/dnssd_ipc.c
        src/GenLinkedList.c
     )
if(WIN32)
    set(shared_c_files 
        src/dllmain.c
        src/dnssd.def
    )
    add_definitions(
        -D_WINDOWS=1
        -D_WINSOCK_DEPRECATED_NO_WARNINGS=1
        -DMDNS_DEBUGMSGS=0
        -DWIN32_LEAN_AND_MEAN=1
        -DNOT_HAVE_SA_LEN=1
        -DUSE_TCP_LOOPBACK=1
        -D_CRT_SECURE_NO_DEPRECATE=1
        -D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1
    )
else()
    # todo
endif()
    add_library(dnssd SHARED ${source_c_files} ${shared_c_files})
    set_property(TARGET dnssd PROPERTY POSITION_INDEPENDENT_CODE ON)
    target_include_directories(dnssd PUBLIC src)
if(WIN32)
    target_link_libraries(dnssd
        PRIVATE ws2_32 
        PRIVATE iphlpapi
        PRIVATE secur32 
        PRIVATE crypt32
    )
else()
    # todo
endif()
    add_library(dnssd-static STATIC ${source_c_files})
    set_property(TARGET dnssd-static PROPERTY POSITION_INDEPENDENT_CODE ON)
    target_include_directories(dnssd-static PUBLIC src)

    add_executable(dns-sd test/dns-sd.c test/ClientCommon.c)
    set_property(TARGET dns-sd PROPERTY POSITION_INDEPENDENT_CODE ON)
    target_include_directories(dns-sd PUBLIC src PUBLIC test)
    target_compile_definitions(dns-sd 
        PRIVATE _CONSOLE 
        PRIVATE NOT_HAVE_GETOPT 
        PRIVATE NOT_HAVE_SETLINEBUF
    )
    target_link_libraries(dns-sd PRIVATE dnssd)
    target_link_libraries(dns-sd
        PRIVATE ws2_32 
    )
