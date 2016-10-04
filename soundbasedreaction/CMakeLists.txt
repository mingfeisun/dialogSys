cmake_minimum_required(VERSION 2.6.4 FATAL_ERROR)
project(soundbasedreaction)
find_package(qibuild)

set(_srcs
    main.cpp
    alsoundbasedreaction.h
    alsoundbasedreaction.cpp
    exec_shell.h
    exec_shell.cpp
)
qi_create_bin(soundbasedreaction ${_srcs})
qi_use_lib(soundbasedreaction ALCOMMON ALPROXIES ALAUDIO)
