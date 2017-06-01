#
# Cmake project settings
#
cmake_minimum_required(VERSION 2.8.11)

project(ua-lds)
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
    if(MSVC)
        if(not ${link_dynamic_msvcrt})
            foreach(flag_var
                    CMAKE_CXX_FLAGS 
                    CMAKE_C_FLAGS
                    CMAKE_CXX_FLAGS_DEBUG 
                    CMAKE_C_FLAGS_DEBUG
                    CMAKE_CXX_FLAGS_RELEASE 
                    CMAKE_C_FLAGS_RELEASE
                    CMAKE_CXX_FLAGS_MINSIZEREL 
                    CMAKE_C_FLAGS_MINSIZEREL
                    CMAKE_CXX_FLAGS_RELWITHDEBINFO 
                    CMAKE_C_FLAGS_RELWITHDEBINFO
                )
                if(${flag_var} MATCHES "/MD")
                    string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
                endif(${flag_var} MATCHES "/MD")
            endforeach(flag_var)
            set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}" 
                CACHE STRING "MSVC C Debug MT flags " FORCE)    
            set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}" 
                CACHE STRING "MSVC CXX Debug MT flags " FORCE)
            set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}" 
                CACHE STRING "MSVC C Release MT flags " FORCE)
            set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}" 
                CACHE STRING "MSVC CXX Release MT flags " FORCE)
            set (CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL}" 
                CACHE STRING "MSVC C Debug MT flags " FORCE)    
            set (CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL}" 
                CACHE STRING "MSVC C Release MT flags " FORCE)
            set (CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}" 
                CACHE STRING "MSVC CXX Debug MT flags " FORCE)    
            set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}" 
                CACHE STRING "MSVC CXX Release MT flags " FORCE)
        endif()
    endif()
    
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
   # target_compile_definitions(dnssd
   #     PRIVATE _WINDOWS=1
   #     PRIVATE _WINSOCK_DEPRECATED_NO_WARNINGS=1
   #     PRIVATE MDNS_DEBUGMSGS=0
   #     PRIVATE WIN32_LEAN_AND_MEAN=1
   #     PRIVATE NOT_HAVE_SA_LEN=1
   #     PRIVATE USE_TCP_LOOPBACK=1
   #     PRIVATE _CRT_SECURE_NO_DEPRECATE=1
   #     PRIVATE _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1
   #     PUBLIC mdns_EXPORTS=1
   # )
    target_link_libraries(dnssd
        PRIVATE ws2_32 
        PRIVATE iphlpapi
        PRIVATE secur32 
        PRIVATE crypt32 
        PRIVATE schannel
    )
else()
    # todo
endif()
    add_library(dnssd-static STATIC ${source_c_files})
    set_property(TARGET dnssd-static PROPERTY POSITION_INDEPENDENT_CODE ON)
    target_include_directories(dnssd-static PUBLIC src)
