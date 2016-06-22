cmake_minimum_required(VERSION 2.8)
project(test_dialog)

find_package(qibuild)
set(_srcs
    exec_shell.h
    exec_shell.cpp
    dialogtext.h
    dialogtext.cpp
    converter.cpp
    converter.h
    wakeup.cpp
    wakeup.h
    main.cpp)

qi_create_bin(test_dialog ${_srcs})
qi_use_lib(test_dialog ALCOMMON ALPROXIES msc)

