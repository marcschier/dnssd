#
# Cmake project settings
#
cmake_minimum_required(VERSION 3.12)

project(dnssd)
include(CMakeToolsHelpers OPTIONAL)
include(CMakePackageConfigHelpers)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(_PROJECT_ROOT ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "Root" FORCE)

enable_testing()

#
# output paths
#
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

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

# Shared library
add_library(dnssd SHARED ${source_c_files} ${shared_c_files})
set_property(TARGET dnssd PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(dnssd
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    PUBLIC
        $<INSTALL_INTERFACE:include>
)

if(WIN32)
    target_link_libraries(dnssd
        PRIVATE ws2_32
        PRIVATE iphlpapi
        PRIVATE secur32
        PRIVATE crypt32
    )
    set_target_properties(dnssd PROPERTIES PREFIX "")
else()
    # todo
endif()

# Static library
add_library(dnssd-static STATIC ${source_c_files})
set_property(TARGET dnssd-static PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(dnssd-static
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    PUBLIC
        $<INSTALL_INTERFACE:include>
)

if(WIN32)
    target_sources(dnssd-static 
        PRIVATE
            src/dllmain.c
    )
    target_compile_definitions(dnssd-static
        PRIVATE
            mdns_STATIC
    )
    target_link_libraries(dnssd-static
        PUBLIC ws2_32
        PUBLIC iphlpapi
        PUBLIC secur32
        PUBLIC crypt32
    )
    set_target_properties(dnssd-static PROPERTIES PREFIX "")
else()
    # todo
endif()

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

# Create and install a config file for making the library useable from CMake
configure_package_config_file(
    cmake/DNSSDConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/DNSSDConfig.cmake"
    INSTALL_DESTINATION lib/cmake
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/DNSSDConfigVersion.cmake"
    VERSION 1.0.0 
    COMPATIBILITY SameMajorVersion
)

install(
    TARGETS dnssd dnssd-static
    EXPORT DNSSDTargets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(
    FILES
        src/dns_sd.h
    DESTINATION include
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/DNSSDConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/DNSSDConfigVersion.cmake"
    DESTINATION lib/cmake
)

install(EXPORT DNSSDTargets
        DESTINATION lib/cmake
        NAMESPACE DNSSD::
)
