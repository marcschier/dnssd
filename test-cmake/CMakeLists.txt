cmake_minimum_required(VERSION 3.12)
project(test-dnssd-cmake)

find_package(DNSSD REQUIRED)

add_executable(test-dnssd-cmake main.cpp)
target_link_libraries(test-dnssd-cmake PRIVATE DNSSD::dnssd)

add_executable(test-dnssd-cmake-static main.cpp)
target_link_libraries(test-dnssd-cmake-static PRIVATE DNSSD::dnssd-static)
