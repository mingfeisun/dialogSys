cmake_minimum_required(VERSION 2.8)
project(wakeUp)
find_package(qibuild)

set(_srcs
    wakeup.cpp
    wakeup.h
    add_to_local.cpp)

qi_create_lib(wakeUp SHARED ${_srcs} SUBFOLDER naoqi)
set_target_properties(wakeUp PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
qi_use_lib(wakeUp ALCOMMON ALPROXIES BOOST)
