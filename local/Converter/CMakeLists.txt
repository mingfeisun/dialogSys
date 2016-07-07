cmake_minimum_required(VERSION 2.8)
project(Converter)
find_package(qibuild)

set(_srcs
    converter.cpp
    converter.h
    add_to_local.cpp)
qi_create_lib(Converter SHARED ${_srcs} SUBFOLDER naoqi)
qi_use_lib(Converter ALCOMMON ALPROXIES BOOST)
